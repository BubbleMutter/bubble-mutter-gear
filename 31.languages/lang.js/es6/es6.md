+ utf16
    + utf16: `aString.charAt()` to `aString.codePointAt();`
    + utf16: convert from code `fromCodePoint(aUtf16Code)`
    + utf16: regexp flag, `/^.$/u.test('嘉')` return  true
    + utf16 string length:  `aString.match(/[\s\S]/gu).length;`
+ rest parameter `function func(param1, ...keys);`
+ The Spread Operator
    + Deprecated: `Math.max.apply([1, 2, 3, 4]);`
    + New in ES6: `Math.max(...[1, 2, 3, 4])`
+ Normal Function
    + function is an object
    + `[[Call]]` is an internal object
    + `[[Construct]]` is an internal object
+ Arrow Function
    + no `[[Construct]]`
    + no `this`, `super`, `arguments`, `new.target`

### Property Initializer Shorthand
```
function createPerson(name, age) {
    return {
        name,
        age
    };
}
let retval = ((name, log) => {return { name, log}; })("Darkness", "Never Die");
console.log(retval);
```

### Deconstruct
+ Object Deconstruct
```
let node = {
        type: "Identifier",
        name: "foo"
    };

let { type, name } = node;
```

+ Array Deconstruct
```
let colors = [ "red", "green", "blue" ];

let [ firstColor, secondColor ] = colors;
let [ , , thirdColor ] = colors;
```

### Symbol allow anonymous key of an object
+ `Symbol` with `for`
```
let firstName = Symbol("first name");
let person = {};

person[firstName] = "Nicholas";

console.log("first name" in person);        // false
console.log(person[firstName]);             // "Nicholas"
console.log(firstName);                     // "Symbol(first name)"
```
+ `Symbol` with `for` can be retrieved via `keyFor`
```
let uid = Symbol.for("uid");
console.log(Symbol.keyFor(uid));    // "uid"

let uid2 = Symbol.for("uid");
console.log(Symbol.keyFor(uid2));   // "uid"

let uid3 = Symbol("uid");
console.log(Symbol.keyFor(uid3));   // undefined
```

### Sets
+ `has`, `delete`, `add`
```
let set = new Set([1, 2, 3, "1", "3", "2"]);
set.add(["100", 50]);
console.log(set.size);    // 5
console.log(set.has(1));  // true
set.delete(1);

let array = [...set];
```

### Weak Sets
+ `has`, `delete`, `add`
+ key is stored as reference
+ when the key has been set to `null`, it will be deleted from WeakkSet
```
let wset = new WeakSet();
let key = { 'o':'p' };
wset.add(key);
console.log(wset.has(key));
```

### Map
+ a key-value pair
```
let map = new Map([ ["name", "Nicholas"], ["age", 25]]);

map.forEach(function(value, key, ownerMap) {
    console.log(key + " " + value);
});
/*
 name Nicholas
 age 25
 */
```

### Weak Map
+ key is stored as reference
```
let key1 = {},
    key2 = {},
    map = new WeakMap([[key1, "Hello"], [key2, 42]]);
```
+ using for real private member
```
let Person = (function () {

    let privateData = new WeakMap();

    function Person(name) {
    privateData.set(this, { name: name });
    }

    Person.prototype.getName = function () {
    return privateData.get(this).name;
    };

    return Person;
}());
```

### Iterator
```
let createIterator = function *(items) {
    for (let i = 0; i < items.length; i++) {
        yield items[i];
    }
};

let iterator = createIterator([1, 2, 3]);

console.log(iterator.next());           // "{ value: 1, done: false }"
console.log(iterator.next());           // "{ value: 2, done: false }"
```
+ passing parameter
```
function *createIterator() {
    let first = yield 1;
    let second = yield first + 2;       // 4 + 2
    yield second + 3;                   // 5 + 3
}

let iterator = createIterator();

console.log(iterator.next());           // "{ value: 1, done: false }"
console.log(iterator.next(4));          // "{ value: 6, done: false }"
console.log(iterator.next(5));          // "{ value: 8, done: false }"
```