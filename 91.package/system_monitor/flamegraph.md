# on-cpu
```sh
custom daemon start
perf

perf script -i perf.data > perf.unfold
FlameGraph/stackcollapse-perf.pl perf.unfold > perf.folded
FlameGraph/flamegraph.pl perf.folded > perf.svg
```

# off-cpu
```sh

```