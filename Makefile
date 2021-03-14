include mk/graphics.mk

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

net := libnet
$(net).type := static

conf := libconf
$(conf).type := static

internal := repo core server net conf
internal.libs := $(addprefix lib,$(internal))

define common.libs
 $(internal)
 $(graphics.libs)
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

client := lib$(project)
$(client).type := shared
$(client).deps := $(net)
define $(client).libs
 netcore
endef

cli := $(project)
$(cli).type := executable
$(cli).deps := $(client) $(conf)
define $(cli).libs
 $(project)
 commline
 conf
 netcore
 timber
 yaml-cpp
endef

install := $(cli) $(client) $(daemon)
targets := $(install) $(internal.libs)

test.deps = $(internal.libs)
define test.libs
 $(common.libs)
 gtest
 gtest_main
endef

include mk/db.mk
include mkbuild/base.mk

confdir = $(prefix)/etc/$(project)

$(obj)/$(core)/preview/image.o: CXXFLAGS += $(graphics.flags)

$(obj)/$(cli)/main.o: CXXFLAGS +=\
 -DNAME='"$(cli)"'\
 -DVERSION='"$(version)"'\
 -DDESCRIPTION='"$(summary)"'\
 -DCONFDIR='"$(confdir)"'

$(obj)/$(daemon)/main.o: CXXFLAGS +=\
 -DNAME='"$(daemon)"'\
 -DVERSION='"$(version)"'\
 -DDESCRIPTION='"$(project) server"'\
 -DCONFDIR='"$(confdir)"'
