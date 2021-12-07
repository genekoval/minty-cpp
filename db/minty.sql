DROP SCHEMA IF EXISTS minty CASCADE;
CREATE SCHEMA minty;

--{{{( Views )

CREATE VIEW object_ref_view AS
SELECT
    object_id,
    count(objects) +
    count(previews) +
    count(icons) +
    count(avatars) +
    count(banners)
    AS reference_count
FROM data.object_ref
LEFT JOIN (
    SELECT object_id
    FROM data.object
) objects USING (object_id)
LEFT JOIN (
    SELECT preview_id AS object_id
    FROM data.object
) previews USING (object_id)
LEFT JOIN (
    SELECT icon AS object_id
    FROM data.site
) icons USING (object_id)
LEFT JOIN (
    SELECT avatar AS object_id
    FROM data.tag
) avatars USING (object_id)
LEFT JOIN (
    SELECT banner AS object_id
    FROM data.tag
) banners USING (object_id)
GROUP BY object_id;

CREATE VIEW post_object_ref_view AS
SELECT
    object_id,
    count(post_objects.object_id) AS reference_count
FROM data.object
LEFT JOIN data.post_object post_objects USING (object_id)
GROUP BY object_id;

CREATE VIEW post_search_view AS
SELECT
    post_id,
    title,
    description,
    date_created,
    date_modified,
    array_agg(tag_id) AS tags
FROM data.post
LEFT JOIN data.post_tag USING (post_id)
GROUP BY post_id;

CREATE VIEW site_ref_view AS
SELECT
    site_id,
    count(sources) AS reference_count
FROM data.site
LEFT JOIN data.source sources USING (site_id)
GROUP BY site_id;

CREATE VIEW source_ref_view AS
SELECT
    source_id,
    count(object) + count(tag) AS reference_count
FROM data.source
LEFT JOIN data.object object USING (source_id)
LEFT JOIN data.tag_source tag USING (source_id)
GROUP BY source_id;

CREATE VIEW source_view AS
SELECT
    source_id,
    resource,
    site_id,
    scheme,
    host,
    icon
FROM data.source
JOIN data.site USING (site_id);

CREATE VIEW tag_name_view AS
SELECT
    tag_id,
    name,
    aliases
FROM (
    SELECT
        tag_id,
        value AS name
    FROM data.tag_name
    WHERE main = true
) AS main
LEFT JOIN (
    SELECT tag_id,
    array_agg(value ORDER BY value) AS aliases
    FROM data.tag_name
    WHERE main = false
    GROUP BY tag_id
) AS alias USING (tag_id);

CREATE VIEW tag_preview AS
SELECT
    tag_id,
    name,
    avatar
FROM data.tag
LEFT JOIN (
    SELECT
        tag_id,
        value AS name
    FROM data.tag_name
    WHERE main = true
) AS main USING (tag_id);

CREATE VIEW tag_view AS
SELECT
    tag_id,
    name,
    aliases,
    description,
    avatar,
    banner,
    count(post_id) AS post_count,
    date_created
FROM data.tag
LEFT JOIN tag_name_view USING (tag_id)
LEFT JOIN data.post_tag USING (tag_id)
GROUP BY tag_id, name, aliases;

CREATE VIEW object_view AS
SELECT
    object_id,
    preview_id,
    source_id,
    resource,
    site_id,
    scheme,
    host,
    icon
FROM data.object
LEFT JOIN source_view USING (source_id);

CREATE VIEW post_preview AS
SELECT
    post_id,
    title,
    preview_id,
    coalesce(comment_count, 0) AS comment_count,
    coalesce(object_count, 0) AS object_count,
    post.date_created
FROM data.post
LEFT JOIN (
    SELECT
        post_id,
        count(comment_id) comment_count
    FROM data.post_comment
    GROUP BY post_id
) comments USING (post_id)
LEFT JOIN (
    SELECT
        post_id,
        count(object_id) AS object_count
    FROM data.post_object
    GROUP BY post_id
) objects USING (post_id)
LEFT JOIN (
    SELECT DISTINCT ON (post_id)
        post_id,
        preview_id
    FROM data.object
    JOIN data.post_object USING (object_id)
    WHERE preview_id IS NOT NULL
    ORDER BY post_id, sequence
) previews USING (post_id);

--}}}

--{{{( Types )

CREATE TYPE object_preview AS (
    object_id       uuid,
    preview_id      uuid
);

CREATE TYPE post_update AS (
    post_id         integer,
    new_data        text,
    date_modified   timestamptz
);

CREATE TYPE tag_name AS (
    name            text,
    aliases         text[]
);

CREATE TYPE tag_name_update AS (
    name            text,
    aliases         text[],
    old_name        text
);

CREATE TYPE tag_text AS (
    tag_id          integer,
    names           text[]
);

--}}}

--{{{( Functions )

CREATE FUNCTION create_comment(
    a_post_id       integer,
    a_parent_id     integer,
    a_content       text
) RETURNS SETOF data.post_comment AS $$
BEGIN
    RETURN QUERY
    INSERT INTO data.post_comment (
        post_id,
        parent_id,
        indent,
        content
    ) VALUES (
        a_post_id,
        a_parent_id,
        (
            SELECT coalesce((
                SELECT indent + 1
                FROM data.post_comment
                WHERE comment_id = a_parent_id
            ), 0)
        ),
        a_content
    ) RETURNING *;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION create_object(
    a_object_id     uuid,
    a_preview_id    uuid,
    a_source_id     integer
) RETURNS void AS $$
BEGIN
    PERFORM create_object_refs(ARRAY[a_object_id, a_preview_id]);

    INSERT INTO data.object (
        object_id,
        preview_id,
        source_id
    ) VALUES (
        a_object_id,
        a_preview_id,
        a_source_id
    ) ON CONFLICT (object_id) DO UPDATE SET
        preview_id = a_preview_id,
        source_id = a_source_id;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION create_object_refs(
    a_objects       uuid[]
) RETURNS void AS $$
BEGIN
    WITH object_table AS (
        SELECT unnest AS object_id
        FROM unnest(a_objects)
    )
    INSERT INTO data.object_ref (object_id)
    SELECT object_id
    FROM object_table
    WHERE object_id IS NOT NULL
    ON CONFLICT DO NOTHING;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION create_post(
    a_title         text,
    a_description   text,
    a_objects       uuid[],
    tags            integer[]
) RETURNS SETOF post_search_view AS $$
DECLARE
    l_post_id       integer;
BEGIN
    WITH new_post AS (
        INSERT INTO data.post (
            title,
            description
        ) VALUES (
            nullif(a_title, ''),
            nullif(a_description, '')
        ) RETURNING *
    )
    SELECT INTO l_post_id
        post_id
    FROM new_post;

    PERFORM insert_post_objects(l_post_id, a_objects);

    WITH tag_table AS (
        SELECT unnest AS tag_id
        FROM unnest(tags)
    )
    INSERT INTO data.post_tag (
        post_id,
        tag_id
    )
    SELECT
        l_post_id,
        tag_table.tag_id
    FROM tag_table;

    RETURN QUERY
    SELECT *
    FROM post_search_view
    WHERE post_id = l_post_id;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION create_post_objects(
    a_post_id       integer,
    a_objects       uuid[],
    a_position      integer
) RETURNS SETOF object_preview AS $$
BEGIN
    UPDATE data.post_object
    SET sequence = sequence + cardinality(a_objects)
    WHERE post_id = a_post_id AND sequence > a_position;

    PERFORM insert_post_objects(a_post_id, a_objects, a_position);

    RETURN QUERY
    SELECT
        object.object_id,
        object.preview_id
    FROM (
        SELECT unnest AS object_id
        FROM unnest(a_objects)
    ) objects
    JOIN data.object USING (object_id);
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION create_post_tag(
    a_post_id integer,
    a_tag_id integer
) RETURNS void AS $$
BEGIN
    INSERT INTO data.post_tag (post_id, tag_id)
    VALUES (a_post_id, a_tag_id);
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION create_site(
    a_scheme        text,
    a_host          text,
    a_icon          uuid
) RETURNS SETOF data.site AS $$
BEGIN
    PERFORM create_object_refs(ARRAY[a_icon]);

    RETURN QUERY
    INSERT INTO data.site (
        scheme,
        host,
        icon
    ) VALUES (
        a_scheme,
        a_host,
        a_icon
    ) RETURNING *;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION create_source(
    a_site_id       integer,
    a_resource      text
) RETURNS SETOF source_view AS $$
BEGIN
    INSERT INTO data.source (
        site_id,
        resource
    ) VALUES (
        a_site_id,
        a_resource
    ) ON CONFLICT DO NOTHING;

    RETURN QUERY
    SELECT *
    FROM source_view
    WHERE site_id = a_site_id AND resource = a_resource;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION create_tag(
    name            text
) RETURNS integer AS $$
DECLARE
    id              integer;
BEGIN
    WITH new_tag AS (
        INSERT INTO data.tag DEFAULT VALUES
        RETURNING *
    )
    SELECT INTO id
        tag_id
    FROM new_tag;

    INSERT INTO data.tag_name (
        tag_id,
        value,
        main
    ) VALUES (
        id,
        name,
        true
    );

    RETURN id;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION create_tag_alias(
    a_tag_id        integer,
    a_alias         text
) RETURNS SETOF tag_name AS $$
BEGIN
    INSERT INTO data.tag_name (tag_id, value)
    VALUES (a_tag_id, a_alias)
    ON CONFLICT DO NOTHING;

    RETURN QUERY
    SELECT t.name, t.aliases
    FROM tag_name_view t
    WHERE tag_id = a_tag_id;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION create_tag_source(
    a_tag_id        integer,
    a_source_id     integer
) RETURNS void AS $$
BEGIN
    INSERT INTO data.tag_source (
        tag_id,
        source_id
    ) VALUES (
        a_tag_id,
        a_source_id
    ) ON CONFLICT DO NOTHING;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION delete_post(
    a_post_id       integer
) RETURNS void AS $$
BEGIN
    DELETE FROM data.post
    WHERE post_id = a_post_id;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION delete_post_object(
    a_post_id       integer,
    a_object_id     uuid
) RETURNS void AS $$
BEGIN
    UPDATE data.post_object
    SET sequence = sequence - 1
    WHERE post_id = a_post_id AND sequence > (
        SELECT sequence
        FROM data.post_object
        WHERE post_id = a_post_id AND object_id = a_object_id
    );

    DELETE FROM data.post_object
    WHERE post_id = a_post_id AND object_id = a_object_id;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION delete_post_objects(
    a_post_id       integer,
    a_objects       uuid[]
) RETURNS timestamptz AS $$
DECLARE
    l_object_id       uuid;
BEGIN
    FOR l_object_id IN
        SELECT object_id
        FROM (
            SELECT unnest AS object_id
            FROM unnest(a_objects)
        ) objects
        JOIN data.post_object USING (object_id)
        ORDER BY sequence DESC
    LOOP
        PERFORM delete_post_object(a_post_id, l_object_id);
    END LOOP;

    RETURN read_post_date_modified(a_post_id);
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION delete_post_objects_ranges(
    a_post_id       integer,
    ranges          int4range[]
) RETURNS timestamptz AS $$
DECLARE
    range           int4range;
BEGIN
    FOR range IN
        SELECT unnest AS range
        FROM unnest(ranges)
        ORDER BY range DESC
    LOOP
        PERFORM delete_post_objects_range(a_post_id, range);
    END LOOP;

    RETURN read_post_date_modified(a_post_id);
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION delete_post_objects_range(
    a_post_id       integer,
    range           int4range
) RETURNS void AS $$
BEGIN
    DELETE FROM data.post_object
    WHERE
        post_id = a_post_id AND
        sequence >= lower(range) AND
        sequence < upper(range);

    UPDATE data.post_object
    SET sequence = sequence - (upper(range) - lower(range))
    WHERE sequence >= upper(range);
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION delete_post_tag(
    a_post_id       integer,
    a_tag_id        integer
) RETURNS void AS $$
BEGIN
    DELETE FROM data.post_tag
    WHERE post_id = a_post_id AND tag_id = a_tag_id;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION delete_tag(
    a_tag_id        integer
) RETURNS void AS $$
BEGIN
    DELETE FROM data.tag
    WHERE tag_id = a_tag_id;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION delete_tag_alias(
    a_tag_id        integer,
    a_alias         text
) RETURNS SETOF tag_name AS $$
BEGIN
    DELETE FROM data.tag_name
    WHERE tag_id = a_tag_id
        AND main = false
        AND value = a_alias;

    RETURN QUERY
    SELECT t.name, t.aliases
    FROM tag_name_view t
    WHERE tag_id = a_tag_id;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION delete_tag_source(
    a_tag_id        integer,
    a_source_id     integer
) RETURNS void AS $$
BEGIN
    DELETE FROM data.tag_source
    WHERE tag_id = a_tag_id AND source_id = a_source_id;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION insert_post_objects(
    a_post_id       integer,
    a_objects       uuid[],
    a_position      integer DEFAULT 0
) RETURNS void AS $$
BEGIN
    WITH objects AS (
        SELECT
            ordinality,
            unnest AS object_id
        FROM unnest(a_objects) WITH ordinality
    )
    INSERT INTO data.post_object (post_id, object_id, sequence)
    SELECT
        a_post_id,
        objects.object_id,
        objects.ordinality + a_position
    FROM objects;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION move_post_object(
    a_post_id       integer,
    a_old_index     integer,
    a_new_index     integer
) RETURNS timestamptz AS $$
BEGIN
    UPDATE data.post_object
    SET sequence = -1
    WHERE post_id = a_post_id AND sequence = a_old_index;

    UPDATE data.post_object
    SET sequence = sequence + (
        SELECT
            CASE
                WHEN a_new_index < a_old_index THEN 1
                WHEN a_new_index > a_old_index THEN -1
            END
    )
    WHERE post_id = a_post_id AND sequence::integer <@ int4range(
        least(a_old_index, a_new_index),
        greatest(a_old_index, a_new_index),
        '[]'
    );

    UPDATE data.post_object
    SET sequence = a_new_index
    WHERE post_id = a_post_id AND sequence = -1;

    RETURN read_post_date_modified(a_post_id);
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION move_post_objects(
    a_post_id       integer,
    a_objects       uuid[],
    a_destination   uuid
) RETURNS timestamptz AS $$
BEGIN
    PERFORM delete_post_objects(a_post_id, a_objects);
    PERFORM create_post_objects(
        a_post_id,
        a_objects,
        (
            CASE
                WHEN a_destination IS NULL THEN (
                    SELECT max(sequence)
                    FROM data.post_object
                    WHERE post_id = a_post_id
                )
                ELSE (
                    SELECT sequence - 1
                    FROM data.post_object
                    WHERE post_id = a_post_id AND object_id = a_destination
                )
            END
        )
    );

    RETURN read_post_date_modified(a_post_id);
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION prune() RETURNS void AS $$
BEGIN
    PERFORM prune_post_objects();
    PERFORM prune_sources();
    PERFORM prune_sites();
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION prune_objects() RETURNS SETOF uuid AS $$
BEGIN
    RETURN QUERY
    DELETE FROM data.object_ref obj USING object_ref_view ref
    WHERE obj.object_id = ref.object_id AND ref.reference_count = 0
    RETURNING obj.object_id;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION prune_post_objects() RETURNS void AS $$
BEGIN
    DELETE FROM data.object obj USING post_object_ref_view ref
    WHERE obj.object_id = ref.object_id AND ref.reference_count = 0;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION prune_sites() RETURNS void AS $$
BEGIN
    DELETE FROM data.site site USING site_ref_view ref
    WHERE site.site_id = ref.site_id AND ref.reference_count = 0;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION prune_sources() RETURNS void AS $$
BEGIN
    DELETE FROM data.source source USING source_ref_view ref
    WHERE source.source_id = ref.source_id AND ref.reference_count = 0;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION read_comments(
    a_post_id       integer
) RETURNS SETOF data.post_comment AS $$
BEGIN
    RETURN QUERY
    SELECT *
    FROM data.post_comment
    WHERE post_id = a_post_id
    ORDER BY
        indent,
        parent_id,
        date_created DESC;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION read_post(
    a_post_id       integer
) RETURNS SETOF data.post AS $$
BEGIN
    RETURN QUERY
    SELECT *
    FROM data.post
    WHERE post_id = a_post_id;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION read_post_date_modified(
    a_post_id       integer
) RETURNS timestamptz AS $$
DECLARE
    l_date_modified timestamptz;
BEGIN
    SELECT date_modified INTO l_date_modified
    FROM data.post
    WHERE post_id = a_post_id;

    RETURN l_date_modified;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION read_posts(
    a_posts         integer[]
) RETURNS SETOF post_preview AS $$
BEGIN
    RETURN QUERY
    SELECT
        post_id,
        title,
        preview_id,
        comment_count,
        object_count,
        date_created
    FROM (
        SELECT
            ordinality,
            unnest AS post_id
        FROM unnest(a_posts) WITH ORDINALITY
    ) posts
    JOIN post_preview USING (post_id)
    ORDER BY ordinality;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION read_post_search()
RETURNS SETOF post_search_view AS $$
BEGIN
    RETURN QUERY
    SELECT *
    FROM post_search_view;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION read_post_objects(
    a_post_id       integer
) RETURNS SETOF object_preview AS $$
BEGIN
    RETURN QUERY
    SELECT
        o.object_id,
        o.preview_id
    FROM data.object o
    JOIN data.post_object USING (object_id)
    WHERE post_id = a_post_id
    ORDER BY sequence;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION read_post_tags(
    a_post_id       integer
) RETURNS SETOF tag_preview AS $$
BEGIN
    RETURN QUERY
    SELECT tag_id, name, avatar
    FROM tag_preview
    JOIN data.post_tag USING (tag_id)
    WHERE post_id = a_post_id
    ORDER BY name;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION read_site(
    a_scheme        text,
    a_host          text
) RETURNS integer AS $$
DECLARE
    result          integer;
BEGIN
    SELECT INTO result site_id
    FROM data.site
    WHERE host = a_host AND scheme = a_scheme;

    RETURN result;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION read_tag(
    a_tag_id    integer
) RETURNS SETOF tag_view AS $$
BEGIN
    RETURN QUERY
    SELECT *
    FROM tag_view
    WHERE tag_id = a_tag_id;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION read_tag_previews(
    a_tags      integer[]
) RETURNS SETOF tag_preview AS $$
BEGIN
    RETURN QUERY
    SELECT
        tag_id,
        name,
        avatar
    FROM (
        SELECT
            ordinality,
            unnest AS tag_id
        FROM unnest(a_tags) WITH ORDINALITY
    ) tags
    JOIN tag_preview USING (tag_id)
    ORDER BY ordinality;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION read_tag_sources(
    a_tag_id        integer
) RETURNS SETOF source_view AS $$
BEGIN
    RETURN QUERY
    SELECT
        source_id,
        resource,
        site_id,
        scheme,
        host,
        icon
    FROM data.tag_source
    JOIN source_view USING (source_id)
    WHERE tag_id = a_tag_id
    ORDER BY host;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION read_tag_text()
RETURNS SETOF tag_text AS $$
BEGIN
    RETURN QUERY
    SELECT
        t.tag_id,
        array_agg(value)
    FROM data.tag_name t
    GROUP BY t.tag_id;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION read_object(
    a_object_id     uuid
) RETURNS SETOF object_view AS $$
BEGIN
    RETURN QUERY
    SELECT *
    FROM object_view
    WHERE object_id = a_object_id;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION read_object_posts(
    a_object_id     uuid
) RETURNS SETOF post_preview AS $$
BEGIN
    RETURN QUERY
    SELECT
        post_id,
        title,
        preview_id,
        comment_count,
        object_count,
        date_created
    FROM post_preview
    JOIN data.post_object USING (post_id)
    WHERE object_id = a_object_id
    ORDER BY date_created DESC;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION update_comment(
    a_comment_id    integer,
    a_content       text
) RETURNS void AS $$
BEGIN
    UPDATE data.post_comment
    SET content = a_content
    WHERE comment_id = a_comment_id;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION update_post_description(
    a_post_id       integer,
    a_description   text
) RETURNS SETOF post_update AS $$
BEGIN
    RETURN QUERY
    UPDATE data.post
    SET description = nullif(a_description, '')
    WHERE post_id = a_post_id
    RETURNING
        post_id,
        description as new_data,
        date_modified;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION update_post_title(
    a_post_id       integer,
    a_title         text
) RETURNS SETOF post_update AS $$
BEGIN
    RETURN QUERY
    UPDATE data.post
    SET title = nullif(a_title, '')
    WHERE post_id = a_post_id
    RETURNING
        post_id,
        title as new_data,
        date_modified;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION update_tag_description(
    a_tag_id        integer,
    a_description   text
) RETURNS text AS $$
DECLARE
    result          text;
BEGIN
    WITH updated AS (
        UPDATE data.tag
        SET description = nullif(a_description, '')
        WHERE tag_id = a_tag_id
        RETURNING description
    )
    SELECT INTO result description
    FROM updated;

    RETURN result;
END;
$$ LANGUAGE plpgsql;

/*
 * Updates a tag's main name.
 * If the new name already exists as an alias, the alias and the main name are
 * swapped. Otherwise, the main name is replaced with the new value.
 */
CREATE FUNCTION update_tag_name(
    -- The tag for which to update the main name.
    a_tag_id        integer,
    -- The new main name.
    a_name          text
) RETURNS SETOF tag_name_update AS $$
DECLARE
    l_old_name      text;
BEGIN
    IF EXISTS (
        SELECT FROM data.tag_name
        WHERE tag_id = a_tag_id AND value = a_name AND main = false
    ) THEN
        UPDATE data.tag_name
        SET main = false
        WHERE tag_id = a_tag_id AND main = true;

        UPDATE data.tag_name
        SET main = true
        WHERE tag_id = a_tag_id AND value = a_name;
    ELSE
        SELECT INTO l_old_name value
        FROM data.tag_name
        WHERE tag_id = a_tag_id AND main = true;

        UPDATE data.tag_name
        SET value = a_name
        WHERE tag_id = a_tag_id AND main = true;
    END IF;

    RETURN QUERY
    SELECT t.name, t.aliases, l_old_name
    FROM tag_name_view t
    WHERE tag_id = a_tag_id;
END;
$$ LANGUAGE plpgsql;

--}}}

--{{{( Trigger Functions )

CREATE FUNCTION update_date_modified() RETURNS trigger AS $$
BEGIN
    NEW.date_modified = NOW();
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION update_post_date_modified() RETURNS trigger AS $$
BEGIN
    IF (TG_OP = 'DELETE') THEN
        UPDATE data.post
        SET date_modified = NOW()
        WHERE post_id IN (SELECT DISTINCT post_id FROM old_table);
    ELSEIF (TG_OP = 'INSERT' OR TG_OP = 'UPDATE') THEN
        UPDATE data.post
        SET date_modified = NOW()
        WHERE post_id IN (SELECT DISTINCT post_id FROM new_table);
    END IF;

    RETURN NULL;
END;
$$ LANGUAGE plpgsql;

--}}}

--{{{( Triggers )

CREATE TRIGGER update_post_date_modified BEFORE UPDATE ON data.post
FOR EACH ROW EXECUTE FUNCTION update_date_modified();

CREATE TRIGGER update_post_date_modified_object_delete
AFTER DELETE ON data.post_object
REFERENCING OLD TABLE AS old_table
FOR EACH STATEMENT EXECUTE FUNCTION update_post_date_modified();

CREATE TRIGGER update_post_date_modified_object_insert
AFTER INSERT ON data.post_object
REFERENCING NEW TABLE AS new_table
FOR EACH STATEMENT EXECUTE FUNCTION update_post_date_modified();

CREATE TRIGGER update_post_date_modified_object_update
AFTER UPDATE ON data.post_object
REFERENCING NEW TABLE AS new_table
FOR EACH STATEMENT EXECUTE FUNCTION update_post_date_modified();

--}}}
