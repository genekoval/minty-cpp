--{{{( Tables )

CREATE TABLE site (
    site_id         SERIAL PRIMARY KEY,
    scheme          text NOT NULL,
    host            text UNIQUE NOT NULL,
    icon            uuid
);

CREATE TABLE source (
    source_id       SERIAL PRIMARY KEY,
    resource        text NOT NULL,
    site_id         integer NOT NULL REFERENCES site ON DELETE NO ACTION,

    UNIQUE (site_id, resource)
);

CREATE TABLE object (
    object_id       uuid PRIMARY KEY,
    preview_id      uuid,
    source_id       integer REFERENCES source ON DELETE NO ACTION
);

CREATE TABLE post (
    post_id         SERIAL PRIMARY KEY,
    title           text,
    description     text,
    date_created    timestamptz NOT NULL DEFAULT NOW(),
    date_modified   timestamptz NOT NULL DEFAULT NOW()
);

CREATE TABLE post_object (
    post_id         integer NOT NULL REFERENCES post ON DELETE CASCADE,
    object_id       uuid NOT NULL REFERENCES object ON DELETE CASCADE,
    sequence        smallint NOT NULL,
    date_added      timestamptz NOT NULL DEFAULT NOW(),

    PRIMARY KEY (post_id, object_id)
);

CREATE TABLE post_comment (
    comment_id      SERIAL PRIMARY KEY,
    post_id         integer NOT NULL REFERENCES post ON DELETE CASCADE,
    parent_id       integer REFERENCES post_comment ON DELETE NO ACTION,
    indent          integer NOT NULL,
    content         text NOT NULL,
    date_created    timestamptz NOT NULL DEFAULT NOW()
);

CREATE TABLE tag (
    tag_id          SERIAL PRIMARY KEY,
    description     text,
    avatar          uuid,
    banner          uuid,
    date_created    timestamptz NOT NULL DEFAULT NOW()
);

CREATE TABLE tag_name (
    tag_id          integer NOT NULL REFERENCES tag ON DELETE CASCADE,
    value           text NOT NULL,
    main            boolean NOT NULL DEFAULT false,

    PRIMARY KEY (tag_id, value)
);

CREATE TABLE post_tag (
    post_id         integer NOT NULL REFERENCES post ON DELETE CASCADE,
    tag_id          integer NOT NULL REFERENCES tag ON DELETE CASCADE,

    PRIMARY KEY (post_id, tag_id)
);

CREATE TABLE tag_source (
    tag_id      integer NOT NULL REFERENCES tag ON DELETE CASCADE,
    source_id       integer NOT NULL REFERENCES source ON DELETE NO ACTION,

    PRIMARY KEY (tag_id, source_id)
);

--}}}

--{{{( Views )

CREATE VIEW source_view AS
SELECT
    source_id,
    resource,
    site_id,
    scheme,
    host,
    icon
FROM source
JOIN site USING (site_id);

CREATE VIEW tag_name_view AS
SELECT
    tag_id,
    name,
    aliases
FROM (
    SELECT
        tag_id,
        value AS name
    FROM tag_name
    WHERE main = true
) AS main
LEFT JOIN (
    SELECT tag_id,
    array_agg(value ORDER BY value) AS aliases
    FROM tag_name
    WHERE main = false
    GROUP BY tag_id
) AS alias USING (tag_id);

CREATE VIEW tag_preview AS
SELECT
    tag_id,
    name,
    avatar
FROM tag
LEFT JOIN (
    SELECT
        tag_id,
        value AS name
    FROM tag_name
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
FROM tag
LEFT JOIN tag_name_view USING (tag_id)
LEFT JOIN post_tag USING (tag_id)
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
FROM object
LEFT JOIN source_view USING (source_id);

CREATE VIEW post_preview AS
SELECT
    post_id,
    title,
    preview_id,
    coalesce(comment_count, 0) AS comment_count,
    coalesce(object_count, 0) AS object_count,
    post.date_created
FROM post
LEFT JOIN (
    SELECT
        post_id,
        count(comment_id) comment_count
    FROM post_comment
    GROUP BY post_id
) comments USING (post_id)
LEFT JOIN (
    SELECT
        post_id,
        count(object_id) AS object_count
    FROM post_object
    GROUP BY post_id
) objects USING (post_id)
LEFT JOIN (
    SELECT DISTINCT ON (post_id)
        post_id,
        preview_id
    FROM object
    JOIN post_object USING (object_id)
    WHERE preview_id IS NOT NULL
    ORDER BY post_id, sequence
) previews USING (post_id);

--}}}

--{{{( Functions )

CREATE FUNCTION create_comment(
    a_post_id       integer,
    a_parent_id     integer,
    a_content       text
) RETURNS SETOF post_comment AS $$
BEGIN
    RETURN QUERY
    INSERT INTO post_comment (
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
                FROM post_comment
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
    INSERT INTO object (
        object_id,
        preview_id,
        source_id
    ) VALUES (
        a_object_id,
        a_preview_id,
        a_source_id
    ) ON CONFLICT DO NOTHING;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION create_post(
    a_title         text,
    a_description   text,
    a_objects       uuid[],
    tags            integer[]
) RETURNS integer AS $$
DECLARE
    l_post_id       integer;
BEGIN
    WITH new_post AS (
        INSERT INTO post (
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
    INSERT INTO post_tag (
        post_id,
        tag_id
    )
    SELECT
        l_post_id,
        tag_table.tag_id
    FROM tag_table;

    RETURN l_post_id;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION create_post_objects(
    a_post_id       integer,
    a_objects       uuid[],
    a_position      integer
) RETURNS TABLE (
    object_id       uuid,
    preview_id      uuid
) AS $$
BEGIN
    UPDATE post_object
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
    JOIN object USING (object_id);
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION create_post_tag(
    a_post_id       integer,
    a_tag_id        integer
) RETURNS void AS $$
BEGIN
    INSERT INTO post_tag(post_id, tag_id)
    VALUES (a_post_id, a_tag_id);
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION create_site(
    a_scheme        text,
    a_host          text,
    a_icon          uuid
) RETURNS SETOF site AS $$
BEGIN
    RETURN QUERY
    INSERT INTO site (
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
    INSERT INTO source (
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
        INSERT INTO tag DEFAULT VALUES
        RETURNING *
    )
    SELECT INTO id
        tag_id
    FROM new_tag;

    INSERT INTO tag_name (
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
) RETURNS TABLE (
    name            text,
    aliases         text[]
) AS $$
BEGIN
    INSERT INTO tag_name(tag_id, value)
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
    INSERT INTO tag_source (
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
    DELETE FROM post
    WHERE post_id = a_post_id;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION delete_post_objects(
    a_post_id       integer,
    ranges          int4range[]
) RETURNS void AS $$
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
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION delete_post_objects_range(
    a_post_id       integer,
    range           int4range
) RETURNS void AS $$
BEGIN
    DELETE FROM post_object
    WHERE
        post_id = a_post_id AND
        sequence >= lower(range) AND
        sequence < upper(range);

    UPDATE post_object
    SET sequence = sequence - (upper(range) - lower(range))
    WHERE sequence >= upper(range);
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION delete_post_tag(
    a_post_id       integer,
    a_tag_id        integer
) RETURNS void AS $$
BEGIN
    DELETE FROM post_tag
    WHERE post_id = a_post_id AND tag_id = a_tag_id;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION delete_tag(
    a_tag_id        integer
) RETURNS void AS $$
BEGIN
    DELETE FROM tag
    WHERE tag_id = a_tag_id;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION delete_tag_alias(
    a_tag_id        integer,
    a_alias         text
) RETURNS TABLE (
    name            text,
    aliases         text[]
) AS $$
BEGIN
    DELETE FROM tag_name
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
    DELETE FROM tag_source
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
    INSERT INTO post_object (post_id, object_id, sequence)
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
) RETURNS void AS $$
BEGIN
    UPDATE post_object
    SET sequence = -1
    WHERE post_id = a_post_id AND sequence = a_old_index;

    UPDATE post_object
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

    UPDATE post_object
    SET sequence = a_new_index
    WHERE post_id = a_post_id AND sequence = -1;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION read_comments(
    a_post_id       integer
) RETURNS SETOF post_comment AS $$
BEGIN
    RETURN QUERY
    SELECT *
    FROM post_comment
    WHERE post_id = a_post_id
    ORDER BY
        indent,
        parent_id,
        date_created DESC;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION read_tag(
    a_tag_id    integer
) RETURNS SETOF tag_view AS $$
BEGIN
    RETURN QUERY
    SELECT *
    FROM
    tag_view
    WHERE tag_id = a_tag_id;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION read_tag_previews_all()
RETURNS SETOF tag_preview AS $$
BEGIN
    RETURN QUERY
    SELECT *
    FROM tag_preview
    ORDER BY name;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION read_tag_posts(
    a_tag       integer
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
    JOIN post_tag USING (post_id)
    WHERE tag_id = a_tag
    ORDER BY date_created DESC;
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
    FROM tag_source
    JOIN source_view USING (source_id)
    WHERE tag_id = a_tag_id
    ORDER BY host;
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
    JOIN post_object USING (post_id)
    WHERE object_id = a_object_id
    ORDER BY date_created DESC;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION read_post(
    a_post_id       integer
) RETURNS SETOF post AS $$
BEGIN
    RETURN QUERY
    SELECT *
    FROM post
    WHERE post_id = a_post_id;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION read_post_objects(
    a_post_id       integer
) RETURNS TABLE (
    object_id       uuid,
    preview_id      uuid
) AS $$
BEGIN
    RETURN QUERY
    SELECT
        o.object_id,
        o.preview_id
    FROM object o
    JOIN post_object USING (object_id)
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
    JOIN post_tag USING (tag_id)
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
    FROM site
    WHERE host = a_host AND scheme = a_scheme;

    RETURN result;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION update_post_description(
    a_post_id       integer,
    a_description   text
) RETURNS text AS $$
DECLARE result      text;
BEGIN
    WITH updated AS (
        UPDATE post
        SET description = nullif(a_description, '')
        WHERE post_id = a_post_id
        RETURNING description
    )
    SELECT INTO result description
    FROM updated;

    RETURN result;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION update_post_title(
    a_post_id       integer,
    a_title         text
) RETURNS text AS $$
DECLARE result      text;
BEGIN
    WITH updated AS (
        UPDATE post
        SET title = nullif(a_title, '')
        WHERE post_id = a_post_id
        RETURNING title
    )
    SELECT INTO result title
    FROM updated;

    RETURN result;
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
        UPDATE tag
        SET description = nullif(a_description, '')
        WHERE tag_id = a_tag_id
        RETURNING description
    )
    SELECT INTO result description
    FROM updated;

    RETURN result;
END;
$$ LANGUAGE plpgsql;

-- Updates a tag's main name.
--
-- If the new name already exists as an alias, the alias and the main name are
-- swapped. Otherwise, the main name is replaced with the new value.
CREATE FUNCTION update_tag_name(
    -- The tag for which to update the main name.
    a_tag_id        integer,
    -- The new main name.
    a_name          text
) RETURNS TABLE (
    name            text,
    aliases         text[]
) AS $$
BEGIN
    IF EXISTS (
        SELECT FROM tag_name
        WHERE tag_id = a_tag_id AND value = a_name AND main = false
    ) THEN
        UPDATE tag_name
        SET main = false
        WHERE tag_id = a_tag_id AND main = true;

        UPDATE tag_name
        SET main = true
        WHERE tag_id = a_tag_id AND value = a_name;
    ELSE
        UPDATE tag_name
        SET value = a_name
        WHERE tag_id = a_tag_id AND main = true;
    END IF;

    RETURN QUERY
    SELECT t.name, t.aliases
    FROM tag_name_view t
    WHERE tag_id = a_tag_id;
END;
$$ LANGUAGE plpgsql;

--}}}

--{{{( Trigger Functions )

CREATE FUNCTION notify_delete_tag() RETURNS trigger AS $$
BEGIN
    PERFORM pg_notify(lower(TG_OP || '_' || TG_TABLE_NAME), OLD.tag_id::text);
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION notify_delete_tag_name() RETURNS trigger AS $$
BEGIN
    -- Avoid sending a notification if this was invoked due to a
    -- delete cascade.
    IF EXISTS (SELECT FROM tag WHERE tag_id = OLD.tag_id) THEN
        PERFORM pg_notify(
            lower(TG_OP || '_' || TG_TABLE_NAME),
            json_build_object(
                'id', OLD.tag_id,
                'name', OLD.value
            )::text
        );
    END IF;

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION notify_insert_tag_name() RETURNS trigger AS $$
BEGIN
    PERFORM pg_notify(
        lower(TG_OP || '_' || TG_TABLE_NAME),
        json_build_object(
            'id', NEW.tag_id,
            'name', NEW.value
        )::text
    );

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION notify_update_tag_name() RETURNS trigger AS $$
BEGIN
    IF OLD.value != NEW.value THEN
        PERFORM pg_notify(
            lower(TG_OP || '_' || TG_TABLE_NAME),
            json_build_object(
                'id', NEW.tag_id,
                'old', OLD.value,
                'new', NEW.value
            )::text
        );
    END IF;

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

--}}}

--{{{( Triggers )

CREATE TRIGGER notify_delete_tag AFTER DELETE ON tag
FOR EACH ROW EXECUTE FUNCTION notify_delete_tag();

CREATE TRIGGER notify_delete_tag_name AFTER DELETE ON tag_name
FOR EACH ROW EXECUTE FUNCTION notify_delete_tag_name();

CREATE TRIGGER notify_insert_tag_name AFTER INSERT ON tag_name
FOR EACH ROW EXECUTE FUNCTION notify_insert_tag_name();

CREATE TRIGGER notify_update_tag_name AFTER UPDATE ON tag_name
FOR EACH ROW EXECUTE FUNCTION notify_update_tag_name();

--}}}
