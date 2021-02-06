--{{{( Tables )

CREATE TABLE site (
    site_id         SERIAL PRIMARY KEY,
    name            text UNIQUE NOT NULL,
    homepage        text UNIQUE NOT NULL,
    thumbnail_id    uuid
);

CREATE TABLE source (
    source_id       SERIAL PRIMARY KEY,
    url             text UNIQUE NOT NULL,
    site_id         integer NOT NULL REFERENCES site ON DELETE NO ACTION
);

CREATE TABLE object (
    object_id       uuid PRIMARY KEY
);

CREATE TABLE object_preview (
    object_id       uuid PRIMARY KEY,
    preview_id      uuid NOT NULL
);

CREATE TABLE object_source (
    object_id       uuid PRIMARY KEY,
    source_id       integer REFERENCES source ON DELETE NO ACTION
);

CREATE TABLE tag (
    tag_id          SERIAL PRIMARY KEY,
    name            text UNIQUE NOT NULL,
    color           text NOT NULL,
    date_created    timestamptz NOT NULL DEFAULT NOW()
);

CREATE TABLE post (
    post_id         SERIAL PRIMARY KEY,
    description     text,
    date_created    timestamptz NOT NULL DEFAULT NOW(),
    date_modified   timestamptz NOT NULL DEFAULT NOW()
);

CREATE TABLE post_object (
    post_id         integer NOT NULL REFERENCES post ON DELETE CASCADE,
    object_id       uuid NOT NULL REFERENCES object ON DELETE CASCADE,
    sequence        int NOT NULL,
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

CREATE TABLE post_tag (
    post_id         integer NOT NULL REFERENCES post ON DELETE CASCADE,
    tag_id          integer NOT NULL REFERENCES tag ON DELETE CASCADE,

    PRIMARY KEY (post_id, tag_id)
);

CREATE TABLE creator (
    creator_id      SERIAL PRIMARY KEY,
    bio             text,
    avatar          uuid,
    banner          uuid,
    date_added      timestamptz NOT NULL DEFAULT NOW()
);

CREATE TABLE creator_name (
    creator_id      integer NOT NULL REFERENCES creator ON DELETE CASCADE,
    value           text,
    main            boolean NOT NULL DEFAULT false,

    PRIMARY KEY (creator_id, value)
);

CREATE TABLE post_creator (
    post_id         integer NOT NULL REFERENCES post ON DELETE CASCADE,
    creator_id      integer NOT NULL REFERENCES creator ON DELETE CASCADE,

    PRIMARY KEY (post_id, creator_id)
);

CREATE TABLE creator_source (
    creator_id      integer NOT NULL REFERENCES creator ON DELETE CASCADE,
    source_id       integer NOT NULL REFERENCES source ON DELETE NO ACTION,

    PRIMARY KEY (creator_id, source_id)
);

--}}}

--{{{( Views )

CREATE VIEW source_view AS
SELECT
    source_id,
    url,
    site_id,
    name,
    homepage,
    thumbnail_id
FROM source
JOIN site USING (site_id);

CREATE VIEW creator_name_view AS
SELECT
    creator_id,
    name,
    aliases
FROM (
    SELECT
        creator_id,
        value AS name
    FROM creator_name
    WHERE main = true
) AS main
LEFT JOIN (
    SELECT creator_id,
    array_agg(value ORDER BY value) AS aliases
    FROM creator_name
    WHERE main = false
    GROUP BY creator_id
) AS alias USING (creator_id);

CREATE VIEW creator_preview AS
SELECT
    creator_id,
    name,
    avatar
FROM creator
LEFT JOIN (
    SELECT
        creator_id,
        value AS name
    FROM creator_name
    WHERE main = true
) AS main USING (creator_id);

CREATE VIEW creator_view AS
SELECT
    creator_id,
    name,
    aliases,
    bio,
    avatar,
    banner,
    array_agg(source_id) AS sources,
    count(post_id) AS post_count,
    date_added
FROM creator
LEFT JOIN creator_name_view USING (creator_id)
LEFT JOIN creator_source USING (creator_id)
LEFT JOIN post_creator USING (creator_id)
GROUP BY creator_id, name, aliases;

CREATE VIEW object_view AS
SELECT
    object_id,
    preview_id,
    source_id,
    url,
    site_id,
    name,
    homepage,
    thumbnail_id
FROM object
    LEFT JOIN object_preview USING (object_id)
    LEFT JOIN object_source USING (object_id)
    LEFT JOIN source USING (source_id)
    LEFT JOIN site USING (site_id);

CREATE VIEW post_view AS
SELECT
    post_id,
    description,
    date_created,
    date_modified,
    array_agg(object_id ORDER BY sequence) AS objects,
    array_agg(DISTINCT tag_id) AS tags,
    array_agg(DISTINCT creator_id) AS creators
FROM post
LEFT JOIN post_object USING (post_id)
LEFT JOIN post_tag USING (post_id)
LEFT JOIN post_creator USING (post_id)
GROUP BY post_id;

--}}}

--{{{( Functions )

CREATE FUNCTION add_object(
    a_object_id     uuid
) RETURNS uuid AS $$
BEGIN
    INSERT INTO object (
        object_id
    ) VALUES (
        a_object_id
    ) ON CONFLICT DO NOTHING;

    RETURN a_object_id;
END;
$$ LANGUAGE plpgsql;

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

CREATE FUNCTION create_creator(
    name            text
) RETURNS integer AS $$
DECLARE
    id              integer;
BEGIN
    WITH new_creator AS (
        INSERT INTO creator DEFAULT VALUES
        RETURNING *
    )
    SELECT INTO id
        creator_id
    FROM new_creator;

    INSERT INTO creator_name (
        creator_id,
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

CREATE FUNCTION create_creator_aliases(
    a_creator       integer,
    a_aliases       text[]
) RETURNS void AS $$
BEGIN
    INSERT INTO creator_name (
        creator_id,
        value
    )
    SELECT
        a_creator,
        unnest(a_aliases);
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION create_creator_source(
    a_creator_id    integer,
    a_site_id       integer,
    a_source_url    text
) RETURNS void AS $$
BEGIN
    INSERT INTO creator_source (
        creator_id,
        source_id
    ) VALUES (
        a_creator_id,
        (
            SELECT source_id
            FROM create_source(a_site_id, a_source_url)
        )
    ) ON CONFLICT DO NOTHING;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION create_post(
    p_description   text,
    objects         uuid[],
    p_creator       integer,
    tags            integer[]
) RETURNS integer AS $$
DECLARE
    l_post_id       integer;
BEGIN
    WITH new_post AS (
        INSERT INTO post (
            description
        ) VALUES (
            NULLIF(p_description, '')
        ) RETURNING *
    )
    SELECT INTO l_post_id
        post_id
    FROM new_post;

    WITH object_table AS (
        SELECT
            ordinality,
            unnest AS obj
        FROM unnest(objects) WITH ORDINALITY
    )
    INSERT INTO post_object (
        post_id,
        object_id,
        sequence
    ) SELECT
        l_post_id,
        add_object(object_table.obj),
        object_table.ordinality
    FROM object_table;

    IF p_creator IS NOT NULL THEN
        INSERT INTO post_creator (
            post_id,
            creator_id
        ) VALUES (
            l_post_id,
            p_creator
        );
    END IF;

    WITH tag_table AS (
        SELECT unnest AS tag
        FROM unnest(tags)
    )
    INSERT INTO post_tag (
        post_id,
        tag_id
    )
    SELECT
        l_post_id,
        tag_table.tag
    FROM tag_table;

    RETURN l_post_id;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION create_tag(
    name            text,
    color           text
) RETURNS SETOF tag AS $$
BEGIN
    RETURN QUERY
    INSERT INTO tag (
        name,
        color
    ) VALUES (
        name,
        color
    ) RETURNING *;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION create_site(
    name            text,
    homepage        text,
    thumbnail_id    uuid
) RETURNS SETOF site AS $$
BEGIN
    RETURN QUERY
    INSERT INTO site (
        name,
        homepage,
        thumbnail_id
    ) VALUES (
        name,
        homepage,
        thumbnail_id
    ) RETURNING *;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION create_source(
    site            integer,
    source_url      text
) RETURNS SETOF source AS $$
BEGIN
    INSERT INTO source (
        site_id,
        url
    ) VALUES (
        site,
        source_url
    ) ON CONFLICT (url) DO NOTHING;

    RETURN QUERY
    SELECT *
    FROM source
    WHERE url = source_url;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION read_creator(
    a_creator_id    integer
) RETURNS SETOF creator_view AS $$
BEGIN
    RETURN QUERY
    SELECT *
    FROM
    creator_view
    WHERE creator_id = a_creator_id;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION read_creator_previews_all()
RETURNS SETOF creator_preview AS $$
BEGIN
    RETURN QUERY
    SELECT *
    FROM creator_preview
    ORDER BY name;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION read_creator_posts(
    a_creator       integer
) RETURNS SETOF post AS $$
BEGIN
    RETURN QUERY
    SELECT
        post_id,
        description,
        date_created,
        date_modified
    FROM post
    JOIN post_creator USING (post_id)
    WHERE creator_id = a_creator
    ORDER BY date_created DESC;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION read_creator_previews(
    a_creators      integer[]
) RETURNS SETOF creator_preview AS $$
BEGIN
    RETURN QUERY
    SELECT *
    FROM creator_preview
    WHERE creator_id = any(a_creators)
    ORDER BY name;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION read_object(
    object          uuid
) RETURNS SETOF object_view AS $$
BEGIN
    RETURN QUERY
    SELECT *
    FROM object_view
    WHERE object_id = object;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION read_objects(
    a_objects       uuid[]
) RETURNS SETOF object_view AS $$
BEGIN
    RETURN QUERY
    SELECT *
    FROM (SELECT unnest(a_objects) AS object_id) objects
    JOIN object_view USING (object_id);
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION read_post(
    a_post_id       integer
) RETURNS SETOF post_view AS $$
BEGIN
    RETURN QUERY
    SELECT *
    FROM post_view
    WHERE post_id = a_post_id;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION read_sources(
    a_sources       integer[]
) RETURNS SETOF source_view AS $$
BEGIN
    RETURN QUERY
    SELECT *
    FROM source_view
    WHERE source_id = any(a_sources);
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION read_tags(
    a_tags          integer[]
) RETURNS SETOF tag AS $$
BEGIN
    RETURN QUERY
    SELECT *
    FROM tag
    WHERE tag_id = any(a_tags)
    ORDER BY name;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION update_object_preview(
    object          uuid,
    preview         uuid
) RETURNS VOID AS $$
BEGIN
    INSERT INTO object_preview (
        object_id,
        preview_id
    ) VALUES (
        object,
        preview
    ) ON CONFLICT (object_id) DO UPDATE
    SET preview_id = EXCLUDED.preview_id;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION update_object_source(
    object          uuid,
    site            integer,
    source_url      text
) RETURNS VOID AS $$
BEGIN
    INSERT INTO object_source (
        object_id,
        source_id
    ) VALUES (
        object,
        (
            SELECT source_id
            FROM create_source(site, source_url)
        )
    ) ON CONFLICT (object_id) DO UPDATE
    SET source_id = EXCLUDED.source_id;
END;
$$ LANGUAGE plpgsql;

--}}}
