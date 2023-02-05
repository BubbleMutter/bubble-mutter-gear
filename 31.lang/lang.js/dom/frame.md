### frame 中
+ 每个frame都有自己独立的 window
    + frame.contentWindow
+ top 指向root window
+ parent 指向父级window

+ `window.location = '';` vs `window.open(); `
+ window.location 同于 document.location

### iframe can't access parent.*
+ inside an iframe, can't access `parent` its properties and methods
+ to tell iframe parent.origin, using postMessage
### 总结得不错的 Blog,  讨论关于多个 iframe间的问题
+ https://javascript.info/cross-window-communication#accessing-an-iframe-contents

### not very well
+ showModalDialog
+ web Workers

### Browser Will Blocked window.open()
+ browser will blocked window.open() on ajax
+ so we can window.open() get an handler before ajax</br>
  than we set mwin.document.location.href
+ if ajax timeout or error, we also need close window