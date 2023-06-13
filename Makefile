include mk/graphics.mk

project := minty
summary := A repo of fun.

STD := c++20

db.user := $(project)
db.name := $(project)

core := libcore
$(core).type := static

server := libserver
$(server).type := static

conf := libconf
$(conf).type := static

internal := core server conf
internal.libs := $(addprefix lib,$(internal))

common.libs := \
 $(internal) \
 $(graphics.libs) \
 $(project) \
 avcodec \
 avformat \
 avutil \
 conftools \
 crypto \
 elasticsearch \
 ext++ \
 http \
 fmt \
 fstore \
 harvest \
 netcore \
 pg++ \
 ssl \
 swscale \
 timber \
 uri \
 uuid++ \
 yaml-cpp \
 zipline

daemon = $(project)d
$(daemon).type = executable
$(daemon).deps := $(internal.libs)
$(daemon).libs := \
 $(common.libs) \
 commline \
 dbtools \
 dmon

client := lib$(project)
$(client).type := shared
$(client).libs := \
 ext++ \
 fstore \
 netcore \
 uri \
 uuid++

install := $(client) $(daemon)
targets := $(install) $(internal.libs)

install.directories = \
 $(include)/minty \
 share/minty

files = \
 $(include) \
 $(src) \
 mk \
 share \
 Makefile \
 VERSION

test.deps = $(internal.libs)
test.libs := \
 $(common.libs) \
 gtest \
 gmock \
 pthread

include mkbuild/base.mk

defines.develop = TEST TIMBER_TIMER_ACTIVE
defines.release = NDEBUG TIMBER_MAX_LEVEL=info

confdir = $(prefix)/etc/$(project)
test.config = .test.conf.yaml

$(obj)/$(conf)/settings.env.test.o: CXXFLAGS += -DTEST_CONFIG='"$(test.config)"'

$(obj)/$(core)/preview/image.%: CXXFLAGS += $(graphics.flags)

$(obj)/$(daemon)/main.o: CXXFLAGS +=\
 -DNAME='"$(daemon)"'\
 -DVERSION='"$(version)"'\
 -DDESCRIPTION='"$(project) server"'\
 -DCONFDIR='"$(confdir)"'

.PHONY: db edit.config init migrate start

edit.config:
	$(EDITOR) $(confdir)/minty.yml

init:
	$($(daemon)) init --config $(test.config)

migrate:
	$($(daemon)) migrate --config $(test.config)

start:
	$($(daemon))
