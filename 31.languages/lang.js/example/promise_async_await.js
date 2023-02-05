// 4 static methods of Promise class:
Promise.resolve(value) // makes a resolved promise with the given value,
Promise.reject(error)  // makes a rejected promise with the given error,
Promise.all(promises)  // waits for all promises to resolve and returns an array of their results.
// If any of the given promises rejects, then it becomes the error of Promise.all, and all other results are ignored.
Promise.race(promises) // waits for the first promise to settle, and its result/error becomes the outcome.

// 兼容性的 Promise.all
let urls = [
    'https://an.inValid.web',
    'https://api.github.com/users/iliakan',
    'https://api.github.com/users/xAxfdsa3411',
];
Promise.all(urls.map(url =>
    fetch(url).catch(err => err)
)).then(responses => { // for each response show its status
    // 3 urls => 3 array members
    console.log(responses[0]); // TypeError: failed to fetch (text may vary)
    console.log(responses[1].status); // 200
    console.log(responses[2].status); // 404
})