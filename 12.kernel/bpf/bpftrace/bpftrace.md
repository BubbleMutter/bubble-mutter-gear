# high level language wrapper
`apt install bpftrace`

# probe point search
```sh
bpftrace -l "kprobe:*"                 # probe kernel function
bpftrace -l "kretprobe:*"              # probe kernel function return
bpftrace -l "uprobe:binary_path:*"     # probe user function
bpftrace -l "uretprobe:binary_path:*"  # probe user function return
bpftrace -l "tracepoint:*"             # probe raw tracepoint (most used)
bpftrace -l "usdt:elf_path:*"          # probe ??
```