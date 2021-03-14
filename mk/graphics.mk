graphics = GraphicsMagick++-config
graphics.flags = $(shell $(graphics) --cppflags)
graphics.libs = $(subst -l,,$(shell $(graphics) --libs))
