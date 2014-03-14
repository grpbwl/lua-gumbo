CC            = gcc
REQCFLAGS     = -std=c99 -pedantic -fpic
CFLAGS       ?= -g -O2 -Wall -Wextra -Wswitch-enum -Wwrite-strings \
                -Wcast-qual -Wshadow
CFLAGS       += $(REQCFLAGS)
LUA           = lua
MKDIR         = mkdir -p
INSTALL       = install -p -m 0644
INSTALLX      = install -p -m 0755
RM            = rm -f
LIBTOOL       = libtool --tag=CC --silent
LTLINK        = $(LIBTOOL) --mode=link
LTCOMPILE     = $(LIBTOOL) --mode=compile
PKGCONFIG     = pkg-config --silence-errors
TIME          = $(or $(shell which time), $(error $@)) -f '%es, %MKB'
TOHTML        = $(LUA) test/serialize.lua html
TOTABLE       = $(LUA) test/serialize.lua table
BENCHFILE     = test/2MiB.html

SERIALIZERS   = gumbo/serialize/table.lua gumbo/serialize/html.lua \
                gumbo/serialize/html5lib.lua

GUMBO_CFLAGS  = $(shell $(PKGCONFIG) --cflags gumbo)
GUMBO_LDFLAGS = $(or $(shell $(PKGCONFIG) --libs gumbo), -lgumbo)
GUMBO_INCDIR  = $(shell $(PKGCONFIG) --variable=includedir gumbo)
GUMBO_HEADER  = $(or $(GUMBO_INCDIR), /usr/include)/gumbo.h

# This uses pkg-config to set LUA_CFLAGS, LUA_CMOD_DIR and LUA_LMOD_DIR
include findlua.mk

# Ensure the tests only load modules from within the current directory
export LUA_PATH = ./?.lua
export LUA_CPATH = ./?.so

all: gumbo.so

ifndef USE_LIBTOOL
gumbo.so: gumbo.o
	$(CC) $(LDFLAGS) $(GUMBO_LDFLAGS) -o $@ $<
else
gumbo.so: .libs/libluagumbo.so.0.0.0
	cp $< $@
endif

gumbo.o: gumbo.c compat.h
	$(CC) $(CFLAGS) $(LUA_CFLAGS) $(GUMBO_CFLAGS) -c -o $@ $<

.libs/libluagumbo.so.0.0.0: libluagumbo.la

libluagumbo.la: gumbo.lo
	$(LTLINK) $(CC) $(GUMBO_LDFLAGS) -rpath $(LUA_CMOD_DIR) -o $@ $<

gumbo.lo: gumbo.c compat.h
	$(LTCOMPILE) $(CC) $(CFLAGS) $(LUA_CFLAGS) $(GUMBO_CFLAGS) -c $<

README.html: README.md
	markdown -f +toc -T -o $@ $<

test/1MiB.html: test/4KiB.html
	@$(RM) $@
	@for i in `seq 1 256`; do cat $< >> $@; done

test/%MiB.html: test/1MiB.html
	@$(RM) $@
	@for i in `seq 1 $*`; do cat $< >> $@; done

# Some static instances of the above pattern rule, just for autocompletion
test/2MiB.html test/3MiB.html test/4MiB.html test/5MiB.html:

tags: gumbo.c $(GUMBO_HEADER) Makefile
	ctags --c-kinds=+p $^

githooks: .git/hooks/pre-commit

.git/hooks/pre-commit: Makefile
	printf '#!/bin/sh\n\nmake -s check || exit 1' > $@
	chmod +x $@

dist: lua-gumbo-$(shell git rev-parse --verify --short master).tar.gz

lua-gumbo-%.tar.gz lua-gumbo-%.zip: force
	git archive --prefix=lua-gumbo-$*/ -o $@ $*

gumbo-%-1.rockspec: rockspec.in
	sed 's/%VERSION%/$*/' $< > $@

test/html5lib-tests/%:
	git submodule init
	git submodule update

install: all
	$(MKDIR) '$(DESTDIR)$(LUA_CMOD_DIR)'
	$(MKDIR) '$(DESTDIR)$(LUA_LMOD_DIR)/gumbo/serialize'
	$(INSTALLX) gumbo.so '$(DESTDIR)$(LUA_CMOD_DIR)/'
	$(INSTALL) gumbo/util.lua '$(DESTDIR)$(LUA_LMOD_DIR)/gumbo/'
	$(INSTALL) $(SERIALIZERS) '$(DESTDIR)$(LUA_LMOD_DIR)/gumbo/serialize/'

uninstall:
	$(RM) '$(DESTDIR)$(LUA_CMOD_DIR)/gumbo.so'
	$(RM) -r '$(DESTDIR)$(LUA_LMOD_DIR)/gumbo'

check: all
	$(TOTABLE) test/t1.html | diff -u2 test/t1.table -
	$(TOHTML) test/t1.html | diff -u2 test/t1.out.html -
	$(TOHTML) test/t1.html | $(TOHTML) | diff -u2 test/t1.out.html -
	$(LUA) test/misc.lua

check-html5lib: all | test/html5lib-tests/tree-construction
	@$(LUA) test/runner.lua $|/*.dat

check-valgrind: LUA = valgrind -q --leak-check=full --error-exitcode=1 lua
check-valgrind: check

check-all: check check-html5lib

check-compat:
	$(MAKE) -sB check LUA=lua CC=gcc
	$(MAKE) -sB check LUA=luajit CC=gcc LUA_PC=luajit
	$(MAKE) -sB check LUA=lua CC=clang
	$(MAKE) -sB check LUA=lua CC=tcc CFLAGS=-Wall

bench: all $(BENCHFILE)
	@echo 'Parsing $(BENCHFILE)...'
	@$(TIME) $(LUA) -e 'require("gumbo").parse_file("$(BENCHFILE)")'

bench-html bench-table: bench-%: all test/serialize.lua $(BENCHFILE)
	@echo 'Parsing and serializing $(BENCHFILE) to $*...'
	@$(TIME) $(LUA) test/serialize.lua $* $(BENCHFILE) /dev/null

clean:
	$(RM) gumbo.so gumbo.o gumbo.lo libluagumbo.la test/*MiB.html
	$(RM) lua-gumbo-*.tar.gz lua-gumbo-*.zip gumbo-*.rockspec
	$(RM) -r .libs


.PHONY: all install uninstall check check-html5lib check-valgrind githooks
.PHONY: check-all check-compat dist bench bench-html bench-table clean force
.DELETE_ON_ERROR:
