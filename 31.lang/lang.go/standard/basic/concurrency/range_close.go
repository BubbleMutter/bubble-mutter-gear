/**
 * range + close 必须配对
 * close 用于结束 chan 的 range 遍历
 * 往 closed channel 发送数据; 将导致 crash
 */

package main

import (
	"fmt"
)

func fibonacci(n int, c chan int) {
	x, y := 0, 1
	for i := 0; i < n; i++ {
		c <- x
		x, y = y, x+y
	}
	close(c) // only need on `for range block`
}

func main() {
	c := make(chan int, 10)
	go fibonacci(cap(c), c)
	for i := range c {
		fmt.Println(i)
	}
}
