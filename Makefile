project := minty
summary := A repo of fun.

STD := c++2a

db.user := $(project)
db.name := $(project)

repo := librepo
$(repo).type := static

core := libcore
$(core).type := static
$(core).deps := $(repo)

internal := repo core
internal.libs := $(addprefix lib,$(internal))

targets := $(internal.libs)

define common.libs
 $(internal)
 ext++
 fmt
 fstore
 netcore
 pqxx
 pq
 timber
endef

test.deps = $(internal.libs)
define test.libs
 $(common.libs)
 gtest
 gtest_main
endef

include mk/db.mk
include mkbuild/base.mk
