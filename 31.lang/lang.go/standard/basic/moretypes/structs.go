package main

import (
	"fmt"
)

func structs() {
	// struct declaration
	type Vertex struct {
		X int
		Y int
	}
	// struct initialization
	fmt.Println(Vertex{1, 2})

	// struct member access
	v := Vertex{1, 2}
	v.X = 4
	fmt.Println(v.X)

	// pointer to structs
	p := &v
	p.X = 1e9 // instead of (*p).X or p->X
	fmt.Println(v)

	fmt.Printf("%T\n", p)

	// explicit member initialization
	v2 := Vertex{X: 15, Y: 25}
	fmt.Println(v2)
}
