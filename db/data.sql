DROP SCHEMA IF EXISTS data CASCADE;
CREATE SCHEMA data;

SET search_path TO data;

CREATE TABLE object_ref (
    object_id       uuid PRIMARY KEY
);

CREATE TABLE site (
    site_id         SERIAL PRIMARY KEY,
    scheme          text NOT NULL,
    host            text UNIQUE NOT NULL,
    icon            uuid REFERENCES object_ref ON DELETE NO ACTION
);

CREATE TABLE source (
    source_id       SERIAL PRIMARY KEY,
    resource        text NOT NULL,
    site_id         integer NOT NULL REFERENCES site ON DELETE NO ACTION,

    UNIQUE (site_id, resource)
);

CREATE TABLE object (
    object_id       uuid PRIMARY KEY REFERENCES object_ref ON DELETE NO ACTION,
    preview_id      uuid REFERENCES object_ref ON DELETE NO ACTION,
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
    avatar          uuid REFERENCES object_ref ON DELETE NO ACTION,
    banner          uuid REFERENCES object_ref ON DELETE NO ACTION,
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
    tag_id          integer NOT NULL REFERENCES tag ON DELETE CASCADE,
    source_id       integer NOT NULL REFERENCES source ON DELETE NO ACTION,

    PRIMARY KEY (tag_id, source_id)
);
