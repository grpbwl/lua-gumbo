local Buffer = require "gumbo.buffer"
local Indent = require "gumbo.indent"

-- This has had much less attention than the other two serializers and is
-- inherently much harder to do properly. Consider it experimental for now.

-- TODO:
-- * Collapse newlines around inline elements and short block elements.
-- * Handle <style>, <script> and <pre> elements properly.
-- * Implement a "minified" mode.
-- * Implement escaping for special characters in tag names (e.g. '=')?

-- Set of void elements
-- whatwg.org/specs/web-apps/current-work/multipage/syntax.html#void-elements
local void = {
    area = true,
    base = true,
    br = true,
    col = true,
    embed = true,
    hr = true,
    img = true,
    input = true,
    keygen = true,
    link = true,
    menuitem = true,
    meta = true,
    param = true,
    source = true,
    track = true,
    wbr = true
}

local escmap = {
    ["&"] = "&amp;",
    ["<"] = "&lt;",
    [">"] = "&gt;"
}

local function escape(text)
    return text:gsub("[&<>]", escmap)
end

local function wrap(text, indent)
    local limit = 78
    local indent_width = #indent
    local pos = 1 - indent_width
    local function reflow(sep, start, word, stop)
        if stop - pos > limit then
            pos = start - indent_width
            return "\n" .. indent .. word
        else
            return " " .. word
        end
    end
    local str = text:gsub("(%s+)()(%S+)()", reflow)
    return indent .. str .. "\n"
end

local function to_html(node, buffer)
    local buf = buffer or Buffer()
    local indent = Indent()
    local level = 0
    local function serialize(node)
        if node.type == "element" then
            local tag = node.tag
            buf:write(indent[level], "<", tag)
            for index, name, value in node.attr:iter() do
                if value == "" then
                    buf:write(' ', name)
                else
                    buf:write(" ", name, '="', value:gsub('"', "&quot;"), '"')
                end
            end
            buf:write(">")
            local length = #node
            if length > 0 then -- recurse into child nodes
                buf:write("\n")
                level = level + 1
                for i = 1, length do
                    serialize(node[i])
                end
                level = level - 1
                if not void[tag] then
                    buf:write(indent[level], "</", tag, ">\n")
                end
            else
                if not void[tag] then
                    buf:write("</", tag, ">\n")
                else
                    buf:write("\n")
                end
            end
        elseif node.type == "text" then
            buf:write(wrap(escape(node.text), indent[level]))
        elseif node.type == "comment" then
            buf:write(indent[level], "<!--", node.text, "-->\n")
        elseif node.type == "document" then
            if node.has_doctype == true then
                buf:write("<!doctype ", node.name, ">\n")
            end
            for i = 1, #node do
                serialize(node[i])
            end
        end
    end
    serialize(node)
    return tostring(buf)
end

return to_html
