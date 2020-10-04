project := minty
summary := A repo of fun.

STD := c++2a

db.user := $(project)
db.name := $(project)

repo := librepo
$(repo).type := static

internal := repo
internal.libs := $(addprefix lib,$(internal))

targets := $(internal.libs)

define core.libs
 $(internal)
 ext++
 pqxx
 pq
endef

test.deps = $(internal.libs)
define test.libs
 $(core.libs)
 gtest
 gtest_main
endef

include mk/db.mk
include mkbuild/base.mk
