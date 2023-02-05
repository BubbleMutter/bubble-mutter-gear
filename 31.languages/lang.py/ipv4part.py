from itertools import *
import sys

ipv4_stoi=lambda x: sum([int(v)<<(32-8*(i+1)) for i,v in [ s for s in [s for s in enumerate(x.split('.'))]]])
ipv4_mtoi=lambda x: sum([ 1<<(32-(i+1)) for i in range(int(x))])
ipv4_itoa=lambda x: '.'.join(reversed([str((x&(0xff<<8*i)) >> (8*i)) for i in range(4)]))
addr_mask=lambda x: x.split('/')

addr, mask= addr_mask(sys.argv[1])
addr_i = ipv4_stoi(addr)
mask_i = ipv4_mtoi(mask)
netmin = addr_i & mask_i
netmax = netmin | (~mask_i & 0xffffffff)
hosts  = [ x for x in range(netmin, netmax+1) ]

# --hosts
for x in hosts: print(ipv4_itoa(x))
# print(addr_itoa(netmin))
# print(addr_itoa(netmax))
