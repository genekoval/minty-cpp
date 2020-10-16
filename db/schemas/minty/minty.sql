--{{{( Tables )

CREATE TABLE site (
    site_id         SERIAL PRIMARY KEY,
    name            text UNIQUE NOT NULL,
    homepage        text UNIQUE NOT NULL,
    thumbnail_id    uuid
);

CREATE TABLE source (
    source_id       SERIAL PRIMARY KEY,
    url             text NOT NULL,
    site_id         integer NOT NULL REFERENCES site ON DELETE NO ACTION
);

CREATE TABLE object (
    object_id       uuid PRIMARY KEY,
    preview_id      uuid,
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
    active          boolean NOT NULL DEFAULT true,
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
    parent_path     ltree NOT NULL,
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
    source_id       integer PRIMARY KEY REFERENCES source ON DELETE NO ACTION,
    creator_id      integer NOT NULL REFERENCES creator ON DELETE CASCADE
);

--}}}

--{{{( Views )

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
    LEFT JOIN source USING (source_id)
    LEFT JOIN site USING (site_id);

--}}}

--{{{( Functions )
CREATE FUNCTION add_object(
    object_id       uuid,
    preview_id      uuid,
    site_id         integer,
    source_url      text
) RETURNS SETOF object_view AS $$
DECLARE
    source_id       integer;
BEGIN
    IF site_id IS NOT NULL THEN
        WITH new_source AS (
            INSERT INTO source (
                site_id,
                url
            ) VALUES (
                site_id,
                source_url
            ) RETURNING *
        )
        SELECT INTO source_id
            ns.source_id
        FROM new_source ns;
    END IF;

    INSERT INTO object (
        object_id,
        preview_id,
        source_id
    ) VALUES (
        object_id,
        preview_id,
        source_id
    );

    RETURN QUERY
    SELECT *
    FROM object_view ov
    WHERE ov.object_id = add_object.object_id;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION create_creator(
    name            text
) RETURNS integer AS $$
DECLARE
    creator_id      integer;
BEGIN
    WITH new_creator AS (
        INSERT INTO creator DEFAULT VALUES
        RETURNING *
    )
    SELECT INTO creator_id
        id
    FROM new_creator;

    INSERT INTO creator_name (
        creator_id,
        value,
        main
    ) VALUES (
        creator_id,
        name,
        true
    );

    RETURN creator_id;
END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION create_post(
    p_description   text,
    objects         uuid[],
    p_creator       integer,
    tags            integer[]
) RETURNS integer AS $$
DECLARE
    post_id         integer;
BEGIN
    WITH new_post AS (
        INSERT INTO post (
            description
        ) VALUES (
            NULLIF(p_description, '')
        ) RETURNING *
    )
    SELECT INTO post_id
        id
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
    )
    SELECT
        post_id,
        object_table.obj,
        object_table.ordinality
    FROM object_table;

    IF p_creator IS NOT NULL THEN
        INSERT INTO post_creator (
            post_id,
            creator_id
        ) VALUES (
            post_id,
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
        post_id,
        tag_table.tag
    FROM tag_table;

    RETURN post_id;
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
--}}}

--{{{( Triggers )
CREATE FUNCTION create_comment_path() RETURNS TRIGGER AS $$
DECLARE
    path ltree;
BEGIN
    IF NEW.parent_id IS NULL THEN
        NEW.parent_path = 'root'::ltree;
    ELSE
        SELECT INTO path
            parent_path || id::text
        FROM post_comment
        WHERE id = NEW.parent_id;

        IF path IS NULL THEN
            RAISE EXCEPTION 'Invalid parent_id %', NEW.parent_id;
        END IF;

        NEW.parent_path = path;
    END IF;

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER comment_path_tgr
    BEFORE INSERT ON post_comment
    FOR EACH ROW EXECUTE PROCEDURE create_comment_path();
--}}}
