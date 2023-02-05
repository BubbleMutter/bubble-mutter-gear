# binary 的 Read 要求 public 结构体成员 (首字母大写)
```go
type date_t struct {
	Name string
	Id   int
}

data := date_t{}
binary.Read(buf, binary.BigEndian, &data)
```

# debian 10 安装 go1.8
```sh
# /etc/apt/source.list
apt update
apt install golang-1.18

sudo ln -srf /usr/lib/go-1.18/bin/go /usr/bin
sudo ln -srf /usr/lib/go-1.18 /usr/lib/go
```
