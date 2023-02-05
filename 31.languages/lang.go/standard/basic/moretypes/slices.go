package main

import (
	"fmt"
)

func slices() {
	// explicit declaration
	var a [2]string
	a[0] = "Hello"
	a[1] = "World"
	fmt.Println(a[0], a[1])

	// implicit declaration
	primes := [6]int{2, 3, 5, 7, 11}
	fmt.Println(primes)

	// slice
	s := primes[1:4]
	fmt.Println(s)
	// slice reference
	s[0] *= 10
	fmt.Println(primes[1])
}

func slices_len_cap() {
	s := []int{2, 3, 5, 7, 11, 13}
	fmt.Printf("len=%d cap=%d %v\n", len(s), cap(s), s)

	// Slice the slice to give it zero length.
	s = s[:0]
	fmt.Printf("len=%d cap=%d %v\n", len(s), cap(s), s)
	// Extend its length.
	s = s[:4]
	fmt.Printf("len=%d cap=%d %v\n", len(s), cap(s), s)

	// Drop its first two values.
	s = s[2:]
	fmt.Printf("len=%d cap=%d %v\n", len(s), cap(s), s)

	// nil slice has a length and capacity of 0
	var nil_slice []int
	fmt.Println(nil_slice, len(nil_slice), cap(nil_slice))
	if nil_slice == nil {
		fmt.Println("nil!")
	}
}

func matrixs() {
	// Create a tic-tac-toe board.
	board := [][]string{
		{"_", "_", "_"},
		{"_", "_", "_"},
		{"_", "_", "_"},
	}

	// The players take turns.
	board[0][0] = "X"
	board[2][2] = "O"
	board[1][2] = "X"
	board[1][0] = "O"
	board[0][2] = "X"

	// for i := 0; i < len(board); i++ {
	// 	fmt.Printf("%s\n", strings.Join(board[i], " "))
	// }
	fmt.Println(board)
}

func slices_operation(size int) {
	// var s [size]int // compile error; dynamic array is not allowed

	// allocate dynamic slice
	var s []int = make([]int, size /* size */, size*2 /* capacity */)
	fmt.Println(s, len(s), cap(s))

	// append to slice
	s = append(s, 1, 2, 3)
	fmt.Println(s, len(s), cap(s))

	// loop slice
	pow := []int{1, 2, 4, 8, 16, 32, 64, 128}
	for i, v := range pow { // index and value
		fmt.Printf("2**%d = %d\n", i, v)
	}
	for i := range pow { // index only; ignore index
		pow[i] = 1 << uint(i) // == 2**i
	}
	for _, value := range pow { // value only; ignore index
		fmt.Printf("%d\n", value)
	}
}

func main() {
	slices()
	slices_len_cap()
	matrixs()
	slices_operation(2)
}
