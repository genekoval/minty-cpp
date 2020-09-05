project := minty
summary := A repo of fun.

STD := c++2a

db.user := $(project)
db.name := $(project)

include mk/db.mk
include mkbuild/base.mk
