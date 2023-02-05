package main

import (
	"fmt"
	"math"
)

// define a method
type Vertex struct {
	X, Y float64
}

func (v Vertex) Abs() float64 { // readonly method
	return math.Sqrt(v.X*v.X + v.Y*v.Y)
}

func (v *Vertex) Scale(f float64) { // writable method
	v.X = v.X * f
	v.Y = v.Y * f
}

// define a custom type; and extend method
type MyFloat float64

func (f MyFloat) Abs() float64 {
	if f < 0 {
		return float64(-f)
	}
	return float64(f)
}

func main() {
	v := Vertex{3, 4}
	fmt.Println(v.Abs())
	v.Scale(10)
	fmt.Println(v.Abs())

	f := MyFloat(-math.Sqrt2)
	fmt.Println(f.Abs())
}
