// https://www.cnblogs.com/wangfupeng1988/p/6532713.html
// evolution chain:
//     type Symbol > 
//     property [Symbol.iterator] > 
//     type Iterator >
//     type Generator >
//     thunkify function >   npm install trunkify
//     chain runner >        npm install co
//     ajax Generator

// most basic usage
function* Hello() {
    yield 1000
    yield (function () { return 200 })()
    return 300
}

var h = Hello()
console.log(typeof h) // object

console.log(h.next()) // { value: 100, done: false }
console.log(h.next()) // { value: 200, done: false }
console.log(h.next()) // { value: 300, done: true }
console.log(h.next()) // { value: undefined, done: true }

// nested usage
function* G1() {
    yield 'a'
    yield 'b'
}
function* G2() {
    yield 'x'
    yield* G1() // 使用 yield* 执行 G2()
    yield 'y'
}
for (let item of G2())
    console.log(item)
// x a b y


// trunkify example
trunkify_timeout = (timeout, message) =>
    // 返回一个只接受 callback 参数的函数
    (callback) => setTimeout(
        () => {
            callback(message)
        }, timeout)

const gen = function* () {
    const r1 = yield trunkify_timeout(1000, 'First')
    console.log('in gen' + r1)
    const r2 = yield trunkify_timeout(3000, 'Second')
    console.log('in gen' + r2)
}

// 链式执行 generator
const g = gen()
g.next().value((message1) => {
    console.log(message1)
    g.next(message1).value(
        (message2) => {
            console.log(message2)
            g.next(message2)
        })
})

// 优化后 链式执行 generator
const run = (gen) => {
    const g = gen()
    let next = (data) => {
        // each callback's regular task
        data && console.log(data)
        // do next()
        const result = g.next(data);
        if(result.done)
            return;
        result.value(next)
    }
    next() // 手动执行第一次
}
run(gen)
///////////////////////


// console.log([1, 2, 3][Symbol.iterator])
