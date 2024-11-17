[TOC]
# smp affinity
1. `taskset`: command tool to manage affinity of "process"
   example: `taskset --cpu-list 0-2,6 nginx`
2. `irqbalance`: systemd service to make cpu balance
   ```sh
   systemctl start irqbalance
   systemctl stop irqbalance
   ```
3. `/proc/irq/$irqid/smp_affinity_list`
    ```sh
    # example: bind a single core to each interrupt-id of a nic.
    #          we already know the number of interrupts is less than cpu
    cpu=0
    for irqid in $(cat /proc/interrupts | grep eth0 | awk '{print $1}' | tr -d :); do
        echo $cpu > /proc/irq/$irqid/smp_affinity_list
        cpu=$[ $cpu + 1 ]
    done
    ```