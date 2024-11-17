 PCa(1.2) - [ (1.1) mac-s (2.1) ] - PCb(2.2)
+ 括号为一个ip 中括号为一个 switch
1. PCa send arp-request arp广播
    d-mac ff-ff-ff-ff-ff-ff & d-ip 1.1
    router-table 已知 (1.1) 
    arp-table    未知 mac-s
2. switch recv
    update arp-table,  ip-a <=> mac-a
    update vlan-table  (mac-a ip-a vlan-a)
3. switch send arp-response to PCa
4. PCa recv
    update arp-table, 1.1 <=> mac-s
5. PCa send tcp-request
    d-mac: mac-s & d-ip: ip-b
    已知 ip-b 未知 mac-b
6. switch recv
    update arp-table ip-a <=> mac-a
    update vlan-table (mac-a ip-a vlan-a)
    FDB try match ip-b (找到则转发给ip-b)
    FDB not match then router-table try match
    if find ip-b and match arp-table then send
    else find ip-b's subnet
7. switch send arp-request to ip-b's subnet
    d-mac: ff-... & d-ip: ip-b
    s-mac: mac-s  & s-ip: 192.168.2.1
8. PCb recv
    update arp-table (2.1 <=> mac-s)
9. PCb send arp-response
    d-mac: mac-s & d-ip: 2.1
10. switch recv
    update arp-tables (mac-b ip-b)
    update vlan-table (mac-b ip-b vlan-b)
11. switch send tcp-request (get from PCa) to PCb
    s-mac: mac-s s-ip: ip-a
12. finally all PCa <=> PCb via vlan-table