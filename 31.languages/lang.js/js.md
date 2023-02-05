### XMLHttpRequest
+ xhr.readState 的取值
    + 0: XMLHttpRequest.UNSENT,
    + 1: XMLHttpRequest.OPENED,
    + 2: XMLHttpRequest.HEADERS_RECEIVED,
    + 3: XMLHttpRequest.LOADING,
    + 4: XMLHttpRequest.DONE

### WebSocket API, duplex Communication with Server
+ var socket = new WebSocket()
+ `socket.send({});`
+ url
    + wws:// === https://
    = ws:// === http://
+ socket.readState
    + 0: WebSocket.CONNECTING,
    + 1: WebSocket.OPEN,
    + 2: WebSocket.CLOSING,
    + 3: WebSocket.CLOSED
+ event:
    + onmessage
    + onerror
    + onopen
    + onclose

### prototype and inherit
+ js only supports **implement-inherit**
    + OO languages supports **interface-inherit** and **implement-inherit**
    + **interface-inherit** needs interface sign, which js does not have
+ 最底层的 Object, 是没有`<prototype>`的, 但有prototype
+ 对象实例有`<prototype>`, 一个指针, 指向父级prototype
    + 每一个prototype必然有 constructor() 方法
+ function本质是一个Function实例
    + Function继承Object, 多了一个 prototype属性
    + 即Function的prototype中的`<prototype>`指向 Object.prototype
+ 实例化时, 对象的`<prototype>` 会指向 类的 prototype, 然后再进行 constructor
+ instanceof的伪实现, `__proto__`在某些浏览器中可以访问 `<prototype>`
```js
function mInstanceof(o, t) {
    return !!(o && t && o.__proto__ && t.prototype
        && (o.__proto__ === t.prototype
            ? true
            : mInstanceof(o.__proto__, t)));
}
```
+ 直接修改prototype的属性和方法, 所有对象中的同名key值会共享
+ set/get对象的key, 先找对象本身, 找不到再找 `<prototype>`
    + 因此, 无法通过对象  直接修改 prototype (`__proto__`另作讨论)
+ es5的丑陋继承 `clsSub.prototype = new clsSuper(); clsSub.prototype.constructor = clsSub; `
    + 但是实际应用中, clsSuper 的构造函数 容易发生side-effect
    + 此处的 new 是一次不必要的消耗
+ es5较好的继承
```js
function clsSuper() { }
function clsSub() { }
clsSub.prototype = Object(clsSuper.prototype);
clsSub.prototype.constructor = clsSub;
```

### prevent modification
+ `Object.preventExtensions(o);` 使得不能add属性和方法
+ `Object.seal(o);` 使得不能 add/del 属性和方法
+ `Object.freeze(o);` 使得不能 add/del/ set 属性和方法

### 类型检查
+ 尽量多用 instanceof 而避免与 null 比较

### libraries
+ underscore
+ lodash (superset of underscore)