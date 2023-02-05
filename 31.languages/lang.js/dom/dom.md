### network
+ AJAX:
    + XMLHttpRequest, new FormData(), { loadstart, progress, error, abort, load, loadend }
    + Fetch
+ WebSocket: duplex communication
+ WebRTC: p2p communication

### DOM Levels History
+ DOM1: html, xml
+ DOM2: Core, Views, Events, Style, DOM2-HTML
+ DOM3: XPath
+ DOM Object: XML, XPath, XSTL, E4X

### open a new tab, then write some message
```
let new_win = window.open();
new_win.document.write("message");
```

```
static getUrlParams() {
    return [...new URLSearchParams(location.search).entries()].reduce((prev, [k, v]) => {prev[k] = v; return prev}, {});
}
```

+ document.URL

### 跳转网页
+ 增加history的
    + window.location.assign(url);
    + location.href = 'http://example.org';
    + window.open(url, '_self');
+ 不增加history的
    + location.replace('');

### DOM { Add Del Set Get }
+ appendChild, insertBefore
+ removeChild
+ replaceChild
+ `childNodes[i]`, childNodes.firsChild

### Event Types
+ UI: onabort, onerror,  onselect, onchange
    + all element has { onload, onunload }
    + window { onresize, onscroll }
+ focus: `<input>`
    + onfocus, onfocusin
    + onblur, onfocusout
+ mouse: [ mousedown > mouseup > click > mousedown > mouseup > click > dblclick ]
    + onclick
    + ondblclick, not supported in mobile phone
    + onmousedown, onmouseup
    + onmouseenter, onmouseleave, onmousemove, onmouseout, onmouseover
    + (event.pageX, event.pageY) means position
    + onmousewheel
    + [ event.shift, event.ctrl, event.alt, event.meta ] four key
+ keyboard, `<input>`
    + onkeyup, onkeydown, onkeypress
    + event.keyCode, event.charCode
+ mutation: (for all DOM)
    + DOMSubtreeModified
    + DOMNodeInserted, DOMNodeInsertedIntoDocument
    + DOMNodeRemoved, DOMNodeRemovedFromDocument
    + DOMAttrModified
    + DOMCharacterDataModified
+ another:
    + readstatechange, orientationchange
+ drag:
    + begin drag: dragstart > drag > dragend
    + drag to it: dragenter > dragover > dragleave/drop

### Event Operation Api
+ createEvent
+ addEventListener
+ removeEventListner
+ preventDefault

### clipboardData
+ getData()
+ setData()
+ clearData()


### 4 tech for share data
+ LocalStorage
+ Cookies,        每次请求都会post给服务端, 有大小限制
+ sessionStorage, 本质上只能存字符串
+ PostMessage