ifeq ($(db.user),)
 $(error database user not set)
endif

ifeq ($(db.name),)
 $(error database name not set)
endif

db ?= psql

db.superuser ?= postgres

db.create ?= createdb --username=$(db.superuser)
db.create.user ?= createuser --username=$(db.superuser)

db.drop ?= dropdb --username=$(db.superuser) --if-exists
db.drop.user ?= dropuser --username=$(db.superuser) --if-exists

db.root := db

db.schemas.root := $(db.root)/schemas
db.schemas := $(wildcard $(db.schemas.root)/*)
db.schemas.checkpoints := $(addprefix .mkbuild/,$(db.schemas))

db.connect := $(db) --username=$(db.user) --dbname=$(db.name)
db.command := $(db.connect) --echo-queries --command

db.connect.super := $(db) --username=$(db.superuser) --dbname=$(db.name)

CLEAN += .mkbuild

.mkbuild/$(db.schemas.root)/%: $(db.schemas.root)/%
	@mkdir -p $(dir $@)
	$(eval schema := $(notdir $<))
	@$(db.command) "DROP SCHEMA IF EXISTS $(schema) CASCADE"
	@$(db.command) "CREATE SCHEMA $(schema)"
	@$(db.connect.super) --file=$</extensions.sql
	@set -e; \
		$(db.connect) \
			--set=ON_ERROR_STOP=1 \
			--command "SET search_path = $(schema)" \
			--file=$</$(schema).sql; \
		touch $@

db: $(db.schemas.checkpoints)

db.connect:
	@$(db.connect)

db.create:
	@echo Initializing database...
	$(db.create.user) $(db.user)
	$(db.create) --owner=$(db.user) $(db.name)

db.drop:
	@echo Dropping database...
	$(db.drop) $(db.name)
	$(db.drop.user) $(db.user)
