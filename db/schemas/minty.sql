--{{{( Tables )

CREATE TABLE object (
    id              uuid PRIMARY KEY,
    preview         uuid
);

CREATE TABLE site (
    id              SERIAL PRIMARY KEY,
    name            text NOT NULL,
    homepage        text NOT NULL,
    thumbnail       uuid REFERENCES object ON DELETE NO ACTION
);

CREATE TABLE source (
    id              SERIAL PRIMARY KEY,
    site_id         integer NOT NULL REFERENCES site ON DELETE NO ACTION,
    url             text NOT NULL
);

CREATE TABLE tag (
    id              SERIAL PRIMARY KEY,
    name            text NOT NULL,
    color           text NOT NULL,
    date_created    timestamptz NOT NULL DEFAULT NOW()
);

CREATE TABLE post (
    id              SERIAL PRIMARY KEY,
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
    id              SERIAL PRIMARY KEY,
    post_id         integer NOT NULL REFERENCES post ON DELETE CASCADE,
    parent_comment  integer REFERENCES post_comment ON DELETE NO ACTION,
    content         text NOT NULL,
    date_created    timestamptz NOT NULL DEFAULT NOW()
);

CREATE TABLE post_tag (
    post_id         integer NOT NULL REFERENCES post ON DELETE CASCADE,
    tag_id          integer NOT NULL REFERENCES tag ON DELETE CASCADE,

    PRIMARY KEY (post_id, tag_id)
);

CREATE TABLE creator (
    id              SERIAL PRIMARY KEY,
    bio             text,
    avatar          uuid REFERENCES object ON DELETE NO ACTION,
    banner          uuid REFERENCES object ON DELETE NO ACTION,
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

CREATE TABLE object_source (
    object_id       uuid PRIMARY KEY REFERENCES object ON DELETE CASCADE,
    source_id       integer NOT NULL REFERENCES source ON DELETE NO ACTION
);

--}}}
