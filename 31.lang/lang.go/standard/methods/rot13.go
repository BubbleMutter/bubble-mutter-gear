package main

import (
	"fmt"
	"io"
	"os"
	"strings"
)

type rot13Reader struct {
	r io.Reader
}

func (reader *rot13Reader) Read(b [] byte) (int, error) {
	n, err := reader.r.Read(b)
	if err == io.EOF {
		return 0, err
	}
	fmt.Println(n)
	fmt.Println(b[0])
	for i := 0; i < n; i++ {
		if 'a' <= b[i] && b[i] <= 'z' {
			b[i] = 'a' + ((b[i] - 'a') + 13) % 26
		} else if 'A' <= b[i] && b[i] <= 'Z' {
			b[i] = 'A' + ((b[i] - 'A') + 13) % 26
		} else {
			b[i] = b[i]
		}
	}
	fmt.Println(b[0])
	return len(b), nil
}

func main() {
	s := strings.NewReader("Lbh penpxrq gur pbqr!")
	r := rot13Reader{s}
	io.Copy(os.Stdout, &r)
}
