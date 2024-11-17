package main

import "fmt"

// chan 形同 计数器 + pthread_join + 获取线程回调函数的传出结果

func sum(s []int, c chan int) {
	sum := 0
	for _, v := range s {
		sum += v
	}
	c <- sum // send sum to c
}

func buf() {
	ch := make(chan int, 2)
	ch <- 1
	ch <- 2
	fmt.Println(<-ch)
	fmt.Println(<-ch)
}

func main() {
	s := []int{7, 2, 8, -9, 4, 0}

	c := make(chan int)
	go sum(s[:len(s)/2], c)
	go sum(s[len(s)/2:], c)

	// below code will be blocked until `c` return
	// x, y, z := <-c, <-c, <-c // will crash on runtime as reason for deadlock
	x, y := <-c, <-c // receive from c

	fmt.Println(x + y)
}
