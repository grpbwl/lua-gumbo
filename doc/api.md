DOM API
-------

### `Document`

Implements [`Node`] and [`ParentNode`].

`documentElement`
:   The root [`Element`] of the document (i.e. the `<html>` element).

`head`
:   The `<head>` [`Element`] of the document.

`body`
:   The `<body>` [`Element`] of the document.

`title`
:   A string containing the document's title (initially, the text contents of
    the `<title>` element in the document markup).

`forms`
:   A [`HTMLCollection`] of all `<form>` elements in the document.

`images`
:   A [`HTMLCollection`] of all `<img>` elements in the document.

`links`
:   A [`HTMLCollection`] of all `<a>` and `<area>` elements in the
    document that have a value for the `href` attribute.

`scripts`
:   A [`HTMLCollection`] of all `<script>` elements in the document.

`doctype`
:   TODO ([MDN](#documenttype))

`getElementById()`
:   TODO ([MDN](https://developer.mozilla.org/en-US/docs/Web/API/Document.getElementById))

`getElementsByTagName()`
:   TODO ([MDN](https://developer.mozilla.org/en-US/docs/Web/API/document.getElementsByTagName))

`getElementsByClassName()`
:   TODO ([MDN](https://developer.mozilla.org/en-US/docs/Web/API/Document.getElementsByClassName))

`createElement()`
:   TODO ([MDN](https://developer.mozilla.org/en-US/docs/Web/API/document.createElement))

`createTextNode()`
:   TODO ([MDN](https://developer.mozilla.org/en-US/docs/Web/API/document.createTextNode))

`createComment()`
:   TODO ([MDN](https://developer.mozilla.org/en-US/docs/Web/API/document.createComment))

`adoptNode()`
:   TODO ([MDN](https://developer.mozilla.org/en-US/docs/Web/API/document.adoptNode))

### `Element`

Implements [`Node`], [`ParentNode`], [`ChildNode`] and
[`NonDocumentTypeChildNode`].

`localName`
:   TODO

`attributes`
:   TODO ([MDN](https://developer.mozilla.org/en-US/docs/Web/API/Element.attributes))

`namespaceURI`
:   TODO

`tagName`
:   TODO ([MDN](https://developer.mozilla.org/en-US/docs/Web/API/Element.tagName))

`id`
:   TODO ([MDN](https://developer.mozilla.org/en-US/docs/Web/API/Element.id))

`className`
:   TODO ([MDN](https://developer.mozilla.org/en-US/docs/Web/API/Element.className))

`innerHTML`
:   TODO ([MDN](https://developer.mozilla.org/en-US/docs/Web/API/Element.innerHTML))

`outerHTML`
:   TODO ([MDN](https://developer.mozilla.org/en-US/docs/Web/API/Element.outerHTML))

`getElementsByTagName()`
:   TODO ([MDN](https://developer.mozilla.org/en-US/docs/Web/API/Element.getElementsByTagName))

`getElementsByClassName()`
:   TODO ([MDN](https://developer.mozilla.org/en-US/docs/Web/API/Element.getElementsByClassName))

`hasAttributes()`
:   TODO ([MDN](https://developer.mozilla.org/en-US/docs/Web/API/Element.hasAttributes))

`hasAttribute()`
:   TODO ([MDN](https://developer.mozilla.org/en-US/docs/Web/API/Element.hasAttribute))

`getAttribute()`
:   TODO ([MDN](https://developer.mozilla.org/en-US/docs/Web/API/Element.getAttribute))

`setAttribute()`
:   TODO ([MDN](https://developer.mozilla.org/en-US/docs/Web/API/Element.setAttribute))

`removeAttribute()`
:   TODO ([MDN](https://developer.mozilla.org/en-US/docs/Web/API/Element.removeAttribute))

### `Text`

Implements [`Node`], [`ChildNode`] and [`NonDocumentTypeChildNode`].

`data`
:   TODO ([MDN](https://developer.mozilla.org/en-US/docs/Web/API/CharacterData#Properties))

`length`
:   TODO ([MDN](https://developer.mozilla.org/en-US/docs/Web/API/CharacterData#Properties))

### `Comment`

Implements [`Node`], [`ChildNode`] and [`NonDocumentTypeChildNode`].

`data`
:   TODO ([MDN](https://developer.mozilla.org/en-US/docs/Web/API/CharacterData#Properties))

`length`
:   TODO ([MDN](https://developer.mozilla.org/en-US/docs/Web/API/CharacterData#Properties))

### `DocumentType`

Implements [`Node`] and [`ChildNode`].

`name`
:   TODO

`publicId`
:   TODO

`systemId`
:   TODO

### `Node`

`childNodes`
:   TODO ([MDN](https://developer.mozilla.org/en-US/docs/Web/API/Node.childNodes))

`parentNode`
:   TODO ([MDN](https://developer.mozilla.org/en-US/docs/Web/API/Node.parentNode))

`parentElement`
:   TODO ([MDN](https://developer.mozilla.org/en-US/docs/Web/API/Node.parentElement))

`ownerDocument`
:   TODO ([MDN](https://developer.mozilla.org/en-US/docs/Web/API/Node.ownerDocument))

`nodeType`
:   TODO ([MDN](https://developer.mozilla.org/en-US/docs/Web/API/Node.nodeType))

`nodeName`
:   TODO ([MDN](https://developer.mozilla.org/en-US/docs/Web/API/Node.nodeName))

`firstChild`
:   TODO ([MDN](https://developer.mozilla.org/en-US/docs/Web/API/Node.firstChild))

`lastChild`
:   TODO ([MDN](https://developer.mozilla.org/en-US/docs/Web/API/Node.lastChild))

`previousSibling`
:   TODO ([MDN](https://developer.mozilla.org/en-US/docs/Web/API/Node.previousSibling))

`nextSibling`
:   TODO ([MDN](https://developer.mozilla.org/en-US/docs/Web/API/Node.nextSibling))

`nodeValue`
:   TODO ([MDN](https://developer.mozilla.org/en-US/docs/Web/API/Node.nodeValue))

`textContent`
:   TODO ([MDN](https://developer.mozilla.org/en-US/docs/Web/API/Node.textContent))

`hasChildNodes()`
:   TODO ([MDN](https://developer.mozilla.org/en-US/docs/Web/API/Node.hasChildNodes))

`contains()`
:   TODO ([MDN](https://developer.mozilla.org/en-US/docs/Web/API/Node.contains))

`appendChild()`
:   TODO ([MDN](https://developer.mozilla.org/en-US/docs/Web/API/Node.appendChild))

`insertBefore()`
:   TODO ([MDN](https://developer.mozilla.org/en-US/docs/Web/API/Node.insertBefore))

`removeChild()`
:   TODO ([MDN](https://developer.mozilla.org/en-US/docs/Web/API/Node.removeChild))

### `ParentNode`

`children`
:   TODO ([MDN](https://developer.mozilla.org/en-US/docs/Web/API/ParentNode.children))

`childElementCount`
:   TODO ([MDN](https://developer.mozilla.org/en-US/docs/Web/API/ParentNode.childElementCount))

`firstElementChild`
:   TODO ([MDN](https://developer.mozilla.org/en-US/docs/Web/API/ParentNode.firstElementChild))

`lastElementChild`
:   TODO ([MDN](https://developer.mozilla.org/en-US/docs/Web/API/ParentNode.lastElementChild))

### `ChildNode`

`remove()`
:   TODO ([MDN](https://developer.mozilla.org/en-US/docs/Web/API/ChildNode.remove))

### `Attr`

`name`
:   The attribute's name.

`value`
:   The attribute's value.

`escapedValue`
:   The attribute's value, escaped according to the [rules][escapingString]
    in the [HTML fragment serialization algorithm].

    Ampersand (`&`) characters in `value` become `&amp;`, double quote (`"`)
    characters become `&quot;` and non-breaking spaces (`U+00A0`) become
    `&nbsp;`.

    *This property is an extension; not a part of any specification.*

### `HTMLCollection`

*TODO*


[`Document`]: #document
[`Element`]: #element
[`Attr`]: #attr
[`Node`]: #node
[`ParentNode`]: #parentnode
[`ChildNode`]: #childnode
[`NonDocumentTypeChildNode`]: https://developer.mozilla.org/en-US/docs/Web/API/NonDocumentTypeChildNode
[`HTMLCollection`]: #htmlcollection
[escapingString]: http://www.w3.org/TR/html5/syntax.html#escapingString
[HTML fragment serialization algorithm]: http://www.w3.org/TR/html5/syntax.html#html-fragment-serialization-algorithm