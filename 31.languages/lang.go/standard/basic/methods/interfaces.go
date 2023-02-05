package main

import (
	"fmt"
	"io"
	"math"
	"os"
	"strconv"
	"strings"
	"time"
)

// basic interface
type Abser interface {
	Abs() float64
}

type MyFloat float64

func (f MyFloat) Abs() float64 {
	if f < 0 {
		return float64(-f)
	}
	return float64(f)
}

type Vertex struct {
	X, Y float64
}

func (v *Vertex) Abs() float64 {
	if v == nil { // pointer receiver can be nil
		fmt.Println("<nil>")
		return 0.0
	}
	return math.Sqrt(v.X*v.X + v.Y*v.Y)
}

// ----------

func describe(i interface{}) {
	fmt.Printf("(%v, %T)\n", i, i)
}

func switch_types(i interface{}) {
	// interface switch; special language syntax
	switch v := i.(type) {
	case int:
		fmt.Printf("Twice %v is %v\n", v, v*2)
	case string:
		fmt.Printf("%q is %v bytes long\n", v, len(v))
	default:
		fmt.Printf("I don't know about type %T!\n", v)
	}
}

// standard interfaces; error
/* error is a built-in interface
 * with a `Error() string` method
 */

// standard-interface `Error` demo 1
type MyError struct {
	When time.Time
	What string
}

func (e *MyError) Error() string {
	return fmt.Sprintf("at %v, %s",
		e.When, e.What)
}

func run() error {
	return &MyError{
		time.Now(),
		"it didn't work",
	}
}

// standard-interface `Error` demo 2
type ErrNegativeSqrt float64

func (e *ErrNegativeSqrt) Error() string {
	if *e < 0 {
		return "cannot Sqrt negative number: " + strconv.FormatFloat(float64(*e), 'E', -1, 64)
	}
	return ""
}

func Sqrt(x float64) (float64, error) {
	e := ErrNegativeSqrt(x)
	if x < 0 {
		return x, &e
	}
	return math.Sqrt(x), nil
}

// standard-interface `String` demo 1
type Person struct {
	Name string
	Age  int
}

func (p Person) String() string {
	return fmt.Sprintf("%v (%v years)", p.Name, p.Age)
}

// standard-interface `String` demo 2
type rot13Reader struct {
	r io.Reader
}

func (reader *rot13Reader) Read(b []byte) (int, error) {
	n, err := reader.r.Read(b)
	if err == io.EOF {
		return 0, err
	}
	fmt.Println(n)
	fmt.Println(b[0])
	for i := 0; i < n; i++ {
		if 'a' <= b[i] && b[i] <= 'z' {
			b[i] = 'a' + ((b[i]-'a')+13)%26
		} else if 'A' <= b[i] && b[i] <= 'Z' {
			b[i] = 'A' + ((b[i]-'A')+13)%26
		} else {
			b[i] = b[i]
		}
	}
	fmt.Println(b[0])
	return len(b), nil
}

// ----------

func main() {
	{
		// basic interface
		var a Abser
		f := MyFloat(-math.Sqrt2)
		v := Vertex{3, 4}

		a = f  // a MyFloat implements Abser
		a = &v // a *Vertex implements Abser

		// In the following line, v is a Vertex (not *Vertex)
		// and does NOT implement Abser.
		// a = v // compile error

		fmt.Println(a.Abs())
	}

	{
		// empty interface; 任意类型接口
		var i interface{}
		describe(i)

		i = 42
		describe(i)

		i = "hello"
		describe(i)
	}
	{
		// interface assertison
		var i interface{} = "hello"
		{
			// obtain assertion result
			s, ok := i.(string)
			fmt.Println(s, ok)

			f, ok := i.(float64)
			fmt.Println(f, ok)
		}
		{
			// ignore assertion result; will trigger a panic
			s := i.(string)
			fmt.Println(s)
			// f := i.(float64) // panic
			// fmt.Println(f)
		}
	}
	{
		// interface switch
		switch_types(21)
		switch_types("hello")
		switch_types(true)
	}
	{
		// standard-interface `Read`
		r := strings.NewReader("Hello, Reader!")

		b := make([]byte, 8)
		for {
			n, err := r.Read(b)
			fmt.Printf("n = %v err = %v b = %v\n", n, err, b)
			fmt.Printf("b[:n] = %q\n", b[:n])
			if err == io.EOF {
				break
			}
		}
	}
	{
		// standard-interface `Read`
		s := strings.NewReader("Lbh penpxrq gur pbqr!")
		r := rot13Reader{s}
		io.Copy(os.Stdout, &r)
	}
}
