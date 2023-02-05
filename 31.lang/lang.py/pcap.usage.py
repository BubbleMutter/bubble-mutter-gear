import pcapkit

# 检查发包端发的包; 收包端是否能全部收到
mpath = [ 'send.pcap', 'recv.pcap' ]

# 提取报文
mpcap = [pcapkit.extract(fin=path, nofile=True) for path in mpath]

# 提取 ip 帧
frame = [ list(filter(lambda frame: frame.info.ethernet.type == pcapkit.const.reg.ethertype.EtherType.Internet_Protocol_version_4, pcap.frame)) for pcap in mpcap ]

# 构造 ipid 的序列
ipidv = [ [ f[pcapkit.IP].info.id for f in frm  ] for frm in frame ]

def analyse(frame, fname):
    f = open(fname, 'w')
    f.write("There are %0d packets\n"%(len(frame)))
    for i in range(len(frame)-1):
        c = frame[i][pcapkit.IP].info.id   # 取 ip id
        n = frame[i+1][pcapkit.IP].info.id # 取 ip id
        delta = frame[i+1].info.time - frame[i].info.time
        if delta.microseconds > 11000: # 包的间隔超过 11 毫秒
            f.write("%06d ~ %06d delta[%s]\n"%(c, n, str(delta)))

    for i in range(len(frame)-1):
        c = frame[i][pcapkit.IP].info.id   # 取 ip id
        n = frame[i+1][pcapkit.IP].info.id # 取 ip id
        if c+1 != n:
            f.write("%05d ~ %06d is not linear\n"%(c, n))

analyse(frame[0], 'send.delta')
analyse(frame[1], 'recv.delta')

with open('send.ipid', 'w') as f:
    f.write('\n'.join(str(ipid) for ipid in ipidv[0]))
    f.write('\n')

with open('recv.ipid', 'w') as f:
    f.write('\n'.join(str(ipid) for ipid in ipidv[1]))
    f.write('\n')
