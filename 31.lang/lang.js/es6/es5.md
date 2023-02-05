+ lodash: 几乎所有方法都是 immutable的
    + 原生的方法 mutable/immutable各种混淆
+ 函数式编程: ramda, eweda
+ parasitic 寄生构造函数模式
    + 极大的弊端就是 对象方法 被重复实例化!!!
+ durable objects 稳妥对象

## 闭包: 一个函数 有权访问另一个函数作用域中的变量
+ 一般地, *闭包函数* 需要更多内存 包含 其*创建函数*作用域中的变量
    + 而且, 每调用一次*闭包函数*就要 多一份这些变量的副本

## Global
+ ECMA-262 内置对象, 相当于标准库
+ 标准对象, 标准错误, 标准方法: 他们都是 function类型
+ 标准类型: Function, Object, Array, String, Number, Date, RegExp,  Boolean
+ 标准错误: Error, EvalError, RangeError, ReferenceError, SyntaxError, TypeError, URIError
+ 标准方法:
    + parseInt, parseFloat, eval
    + encodeURI, decodeURI, encodeURIComponent, decodeURIComponent
    + isNaN, isInfinite
    + setTimeout, setInterval
+ 标准方法不可以`new`, ?如何实现的?, 因为所有一般的function()都可以new
+ window中新增的
    + alert, confirm, prompt

## 原生类型`typeof`
+ 只有: function, object, number, string, boolean, undefined
+ 数组也是object
+ NaN, Infinity 是 number

## String
+ 获取子字符串: substr(), substring(), slice()
+ RegExp相关方法: match, search, replace, split
+ charAt(), charCodeAt(), fromCharCode()

## Object
### Object Properties 的行为描述
```js
var obj = {};
Object.defineProperty(obj, 'key', {
    configurable: false,
    writatble: false,
    value: 'value',
    enumerable: false
});
```
### setter, getter的本质实现

## function
+ 不同于 `Function`
### prototype, 实现class的核心
+ 语法原意: 该函数所有实例都共享的 **属性和方法**集
+ 误区, 把 prototype 看作 static
+ prototype.constructor(), 即构造函数, 默认存在
    + 由于共享一个constructor(), 所以才能实现 new 对象的效果
+ 浏览器中有时叫 `__proto__`, 有些浏览器不让访问
+ prototype是每一个function的一个property
+ instanceof 的实现猜想
    + `obj.constructor === cls.prototype.constructor`
+ construtor的enumeratable是false
### es6 class
+ 本质上是不存在 class的, `typeof 一个class` 返回也是 `function`
+ class的 static变量, 分两种, prototype中的, class自己的
    + prototype中变量, 更接近于c++中的 static变量
+ class的 static方法, 是该 class/function 的property,  不在prototype中, 实例不继承
+ class的 实例方法,   是其 prototype 的 property
+ class继承extends: 分别同时继承 class中的property  和 prototype中的property
+ 实例get/set 与prototype同名property
    + this.prototype 返回undefined
    + setter: 如果 实例中 不存在, 则创建, (无论prototype)
    + getter: 如果 实例中 存在, 返回, 否则返回 prototype中的
    + 即 无法显式地 通过 实例修改 prototype的property
+ 尽量不要写 static变量, 除非你知道自己在干嘛

## this
+ 执行环境 execution context
+ 活动对象 activation object
+ 每个函数在其调用时 自动地有this和arguments
    + callee, caller不一定有
+ bind, function的prototype中的方法, 创建一个新的function
+ 只有constructor()  中的this才必然指向对象
```js
class cls {
    constructor() {
        this.value = 100;
    }
    printValue() {
        console.log(this.value);
    }
}
let m = new cls();
let f = m.printValue;
m.printValue(); // valid
f(); // error throw
```
### react 解决this的5个方案
1. using React.createClass (deprecated, 非es6)
2. bind in render() ` onChange={ this.handleChange.bind(this) }`
3. arrow in render() ` onChange={ e => this.handleChange(e) } `
4. bind in constructor() ` this.handleChange = this.handleChange.bind(this) `
    + 使用 `react-autobind` `autobind-decorator` `reflective-bind`
5. arrow function in class ` handleChange = () => { this } `
+ 2, 3可能有性能问题, 因为render重复执行,  创建一个function相对耗时
+ 4, 5同质, 使用匿名函数的形式, 保存 实例创建时的 this, 即 实例指针
    + 然而, 这些方法不在 prototype中, 每个对象都会有副本
+ 总结起来, 只有在该方法 需要用到this时, 才使用4, 5的解决
    + 禁忌: 盲目使用 arrow-function-in-class, bind-in-constructor

## es5 module
+ CommonJS ( module exports require global ) 发源于nodejs服务端
    + Browserify
    + Tiny Browser Require
    + BUG: require 是同步的, 会require一整个js
+ AMD, 预执行
    + require.js 一个实现 `<script src="js/require.js" defer async="true" ></script>`
    + 先尽早地执行(依赖)模块
+ CMD, 懒执行
    + seajs, 一个实现
    + 严格按照模块在代码中出现(require)的顺序

## Design Pattern
+ 3 compoenet:
    + key
    + value
    + show
1. backend return data: **key -> value**
2. frontend show: **key -> show**
3. frontend underlayer: **key -> value**


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
+ underscore > lodash
    + lodash is superset of underscore