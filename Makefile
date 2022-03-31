include mk/graphics.mk

project := minty
summary := A repo of fun.

STD := c++20

db.user := $(project)
db.name := $(project)

repo := librepo
$(repo).type := static

core := libcore
$(core).type := static

server := libserver
$(server).type := static

conf := libconf
$(conf).type := static

internal := repo core server conf
internal.libs := $(addprefix lib,$(internal))

define common.libs
 $(internal)
 $(graphics.libs)
 avcodec
 avformat
 avutil
 conftools
 entix
 ext++
 fmt
 fstore
 harvest
 netcore
 pqxx
 pq
 swscale
 timber
 uri
 yaml-cpp
endef

daemon = $(project)d
$(daemon).type = executable
$(daemon).deps := $(internal.libs)
define $(daemon).libs
 $(common.libs)
 commline
 dbtools
 dmon
endef

client := lib$(project)
$(client).type := shared
define $(client).libs
 netcore
endef

cli := $(project)
$(cli).type := executable
$(cli).deps := $(client) $(conf)
define $(cli).libs
 $(project)
 commline
 conftools
 conf
 fmt
 fstore
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
 gmock
 pthread
endef

include mkbuild/base.mk

define defines.add
 $(addprefix -D, $(1))
endef

defines.debug = TEST TIMBER_TIMER_ACTIVE
defines.release = TIMBER_MAX_LEVEL=info

ifeq ($(environment),$(environment.develop))
 CXXFLAGS += $(call defines.add,$(defines.debug))
else
 CXXFLAGS += $(call defines.add,$(defines.release))
endif

confdir = $(prefix)/etc/$(project)
test.config = .test.conf.yaml

$(obj)/$(conf)/settings.test.env.o: CXXFLAGS += -DTEST_CONFIG='"$(test.config)"'

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

$(obj)/$(daemon)/db/db.o: CXXFLAGS += -DSQLDIR='"$(shell pwd)/db"'

.PHONY: edit.config init migrate start

edit.config:
	$(EDITOR) $(confdir)/minty.yml

init:
	$($(daemon)) init --skip-index --config $(test.config)

migrate:
	$($(daemon)) migrate --config $(test.config)

start:
	$($(daemon))
