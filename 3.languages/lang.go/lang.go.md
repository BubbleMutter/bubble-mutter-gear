# binary 的 Read 要求 public 结构体成员 (首字母大写)
```go
type date_t struct {
	Name string
	Id   int
}

data := date_t{}
binary.Read(buf, binary.BigEndian, &data)
```
