/*
 Lua bindings for the Gumbo HTML5 parsing library.
 Copyright (c) 2013, Craig Barnes

 Permission to use, copy, modify, and/or distribute this software for any
 purpose with or without fee is hereby granted, provided that the above
 copyright notice and this permission notice appear in all copies.

 THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <lua.h>
#include <lauxlib.h>
#include <gumbo.h>

static const struct {
    const unsigned int flag;
    const char *name;
} flag_map[] = {
    {GUMBO_INSERTION_BY_PARSER, "insertion_by_parser"},
    {GUMBO_INSERTION_IMPLICIT_END_TAG, "implicit_end_tag"},
    {GUMBO_INSERTION_IMPLIED, "insertion_implied"},
    {GUMBO_INSERTION_CONVERTED_FROM_END_TAG, "converted_from_end_tag"},
    {GUMBO_INSERTION_FROM_ISINDEX, "insertion_from_isindex"},
    {GUMBO_INSERTION_FROM_IMAGE, "insertion_from_image"},
    {GUMBO_INSERTION_RECONSTRUCTED_FORMATTING_ELEMENT, "reconstructed_formatting_element"},
    {GUMBO_INSERTION_ADOPTION_AGENCY_CLONED, "adoption_agency_cloned"},
    {GUMBO_INSERTION_ADOPTION_AGENCY_MOVED, "adoption_agency_moved"},
    {GUMBO_INSERTION_FOSTER_PARENTED, "foster_parented"}
};

#define add_literal(L, k, v) ( \
    lua_pushliteral(L, v), \
    lua_setfield(L, -2, k) \
)

static inline void add_string(lua_State *L, const char *k, const char *v) {
    lua_pushstring(L, v);
    lua_setfield(L, -2, k);
}

static inline void add_integer(lua_State *L, const char *k, const int v) {
    lua_pushinteger(L, v);
    lua_setfield(L, -2, k);
}

static inline void add_boolean(lua_State *L, const char *k, const bool v) {
    lua_pushboolean(L, v);
    lua_setfield(L, -2, k);
}

static void add_attributes(lua_State *L, const GumboVector *attrs) {
    const unsigned int length = attrs->length;
    if (length > 0) {
        lua_createtable(L, length, length);
        lua_getfield(L, LUA_REGISTRYINDEX, "gumbo.attributes");
        lua_setmetatable(L, -2);
        add_integer(L, "length", length);
        for (unsigned int i = 0; i < length; i++) {
            const GumboAttribute *attr = (const GumboAttribute *)attrs->data[i];
            add_string(L, attr->name, attr->value);
            lua_createtable(L, 0, 6);
            add_string(L, "name", attr->name);
            add_string(L, "value", attr->value);
            add_integer(L, "line", attr->name_start.line);
            add_integer(L, "column", attr->name_start.column);
            add_integer(L, "offset", attr->name_start.offset);
            switch (attr->attr_namespace) {
            case GUMBO_ATTR_NAMESPACE_NONE:
                add_boolean(L, "namespace", false);
                break;
            case GUMBO_ATTR_NAMESPACE_XLINK:
                add_literal(L, "namespace", "xlink");
                break;
            case GUMBO_ATTR_NAMESPACE_XML:
                add_literal(L, "namespace", "xml");
                break;
            case GUMBO_ATTR_NAMESPACE_XMLNS:
                add_literal(L, "namespace", "xmlns");
                break;
            default:
                luaL_error(L, "Error: invalid attribute namespace");
            }
            lua_rawseti(L, -2, i+1);
        }
        lua_setfield(L, -2, "attr");
    }
}

static char *strnlower(const char *string, const size_t length) {
    char *lower = malloc(length + 1);
    if (!lower) return NULL;
    for (size_t i = 0; i < length; i++) {
        const char c = string[i];
        if (c <= 'Z' && c >= 'A') {
            lower[i] = c + 32;
        } else {
            lower[i] = c;
        }
    }
    lower[length] = '\0';
    return lower;
}

static void add_tag(lua_State *L, const GumboElement *element) {
    if (element->tag_namespace == GUMBO_NAMESPACE_SVG) {
        GumboStringPiece original_tag = element->original_tag;
        gumbo_tag_from_original_text(&original_tag);
        const char *normalized = gumbo_normalize_svg_tagname(&original_tag);
        if (normalized) {
            add_string(L, "tag", normalized);
            return;
        }
    }
    if (element->tag == GUMBO_TAG_UNKNOWN) {
        GumboStringPiece original_tag = element->original_tag;
        gumbo_tag_from_original_text(&original_tag);
        char *lower = strnlower(original_tag.data, original_tag.length);
        if (lower) {
            lua_pushlstring(L, lower, original_tag.length);
            free(lower);
        } else {
            luaL_error(L, "Error: out of memory");
        }
    } else {
        lua_pushstring(L, gumbo_normalized_tagname(element->tag));
    }
    lua_setfield(L, -2, "tag");
}

static void add_tag_namespace(lua_State *L, const GumboNamespaceEnum ns) {
    switch (ns) {
    case GUMBO_NAMESPACE_HTML:
        add_literal(L, "tag_namespace", "html");
        break;
    case GUMBO_NAMESPACE_MATHML:
        add_literal(L, "tag_namespace", "math");
        break;
    case GUMBO_NAMESPACE_SVG:
        add_literal(L, "tag_namespace", "svg");
        break;
    default:
        luaL_error(L, "Error: invalid tag namespace");
    }
}

static void add_parseflags(lua_State *L, const GumboParseFlags flags) {
    static const unsigned int nflags = sizeof(flag_map) / sizeof(flag_map[0]);
    if (flags != GUMBO_INSERTION_NORMAL) {
        lua_createtable(L, 0, 1);
        for (unsigned int i = 0; i < nflags; i++) {
            if ((flags & flag_map[i].flag) != 0) {
                add_boolean(L, flag_map[i].name, true);
            }
        }
        lua_setfield(L, -2, "parse_flags");
    }
}

static void create_text_node(lua_State *L, const GumboText *text) {
    lua_createtable(L, 0, 5);
    add_string(L, "text", text->text);
    add_integer(L, "line", text->start_pos.line);
    add_integer(L, "column", text->start_pos.column);
    add_integer(L, "offset", text->start_pos.offset);
}

static void add_quirks_mode(lua_State *L, const GumboQuirksModeEnum qm) {
    switch (qm) {
    case GUMBO_DOCTYPE_NO_QUIRKS:
        lua_pushliteral(L, "no-quirks");
        break;
    case GUMBO_DOCTYPE_QUIRKS:
        lua_pushliteral(L, "quirks");
        break;
    case GUMBO_DOCTYPE_LIMITED_QUIRKS:
        lua_pushliteral(L, "limited-quirks");
        break;
    default:
        luaL_error(L, "Error: invalid quirks mode");
        return;
    }
    lua_setfield(L, -2, "quirks_mode");
}

// Forward declaration, to allow mutual recursion
static void push_node(lua_State *L, const GumboNode *node);

static void add_children(lua_State *L, const GumboVector *children) {
    const unsigned int length = children->length;
    for (unsigned int i = 0; i < length; i++) {
        push_node(L, (const GumboNode *)children->data[i]);
        lua_rawseti(L, -2, i + 1);
    }
}

static void push_node(lua_State *L, const GumboNode *node) {
    luaL_checkstack(L, 10, "element nesting too deep");
    switch (node->type) {
    case GUMBO_NODE_DOCUMENT: {
        const GumboDocument *document = &node->v.document;
        lua_createtable(L, document->children.length, 7);
        add_literal(L, "type", "document");
        add_string(L, "name", document->name);
        add_string(L, "public_identifier", document->public_identifier);
        add_string(L, "system_identifier", document->system_identifier);
        add_boolean(L, "has_doctype", document->has_doctype);
        add_quirks_mode(L, document->doc_type_quirks_mode);
        add_children(L, &document->children);
        return;
    }
    case GUMBO_NODE_ELEMENT: {
        const GumboElement *element = &node->v.element;
        lua_createtable(L, element->children.length, 7);
        lua_getfield(L, LUA_REGISTRYINDEX, "gumbo.element");
        lua_setmetatable(L, -2);
        add_literal(L, "type", "element");
        add_tag(L, element);
        add_tag_namespace(L, element->tag_namespace);
        add_integer(L, "line", element->start_pos.line);
        add_integer(L, "column", element->start_pos.column);
        add_integer(L, "offset", element->start_pos.offset);
        add_parseflags(L, node->parse_flags);
        add_attributes(L, &element->attributes);
        add_children(L, &element->children);
        return;
    }
    case GUMBO_NODE_TEXT:
        create_text_node(L, &node->v.text);
        add_literal(L, "type", "text");
        return;
    case GUMBO_NODE_COMMENT:
        create_text_node(L, &node->v.text);
        add_literal(L, "type", "comment");
        return;
    case GUMBO_NODE_CDATA:
        create_text_node(L, &node->v.text);
        add_literal(L, "type", "cdata");
        return;
    case GUMBO_NODE_WHITESPACE:
        create_text_node(L, &node->v.text);
        add_literal(L, "type", "whitespace");
        return;
    default:
        luaL_error(L, "Error: invalid node type");
    }
}

static int parse(lua_State *L) {
    size_t length;
    const char *input = luaL_checklstring(L, 1, &length);
    GumboOptions options = kGumboDefaultOptions;
    options.tab_stop = luaL_optint(L, 2, 8);
    GumboOutput *output = gumbo_parse_with_options(&options, input, length);
    if (output) {
        push_node(L, output->document);
        lua_rawgeti(L, -1, output->root->index_within_parent + 1);
        lua_setfield(L, -2, "root");
        gumbo_destroy_output(&options, output);
        return 1;
    } else {
        lua_pushnil(L);
        lua_pushliteral(L, "Failed to parse");
        return 2;
    }
}

static inline void require(lua_State *L, const char *modname) {
    lua_getglobal(L, "require");
    lua_pushstring(L, modname);
    lua_call(L, 1, 1);
    lua_setfield(L, LUA_REGISTRYINDEX, modname);
}

int luaopen_cgumbo(lua_State *L) {
    require(L, "gumbo.element");
    require(L, "gumbo.attributes");
    lua_createtable(L, 0, 1);
    lua_pushcfunction(L, parse);
    lua_setfield(L, -2, "parse");
    return 1;
}
