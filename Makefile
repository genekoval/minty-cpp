project := minty
summary := A repo of fun.

STD := c++2a

db.user := $(project)
db.name := $(project)

repo := librepo
$(repo).type := static

core := libcore
$(core).type := static

server := libserver
$(server).type := static

internal := repo core server
internal.libs := $(addprefix lib,$(internal))

define common.libs
 $(internal)
 ext++
 fmt
 fstore
 netcore
 pqxx
 pq
 timber
 yaml-cpp
endef

daemon = $(project)d
$(daemon).type = executable
$(daemon).deps := $(internal.libs)
define $(daemon).libs
 $(common.libs)
 commline
endef

install := $(daemon)
targets := $(install) $(internal.libs)

test.deps = $(internal.libs)
define test.libs
 $(common.libs)
 gtest
 gtest_main
endef

include mk/db.mk
include mkbuild/base.mk

$(obj)/$(daemon)/main.o: CXXFLAGS +=\
 -DNAME='"$(daemon)"'\
 -DVERSION='"$(version)"'\
 -DDESCRIPTION='"$(summary)"'\
 -DCONFDIR='"$(prefix)/etc/minty"'
