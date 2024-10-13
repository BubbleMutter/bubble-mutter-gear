# tcpipv1 note
+ drop Chapter25-29 (SNMP/Telnet/FTP/SMTP/NFS)

# Pending Problem
+ what's internet checksum algorithm
+ how linux kernel choose ip identification?
+ what's the core algorithm inside the router table?
+ Difference between `ping` and `traceroute` handling memo
+ Why larger tcp-window-size requires more accurate RTT calculation?
+ What's the mechanism of tcp-option-timestamp and the problems to resolve?

# Chapter 04 ARP
Proxy ARP
Gratuitous ARP: sender-ip is equal to target-ip

# Chapter 05 RARP
RARP is used by many diskless system to obtain ip address when bootstrapped.

# Chapter 06 ICMP
ICMP error message is never generated in response to:
    1. an ICMP error message.
    2. destination ip address is multicast or broadcast
    3. destination hw address is broadcast
    4. ip fragment other than the first
    5. source ip address is not a single host

# Chapter 07 Ping
icmp: identifier and sequence number
first-icmp-rtt is larger than the rest, as for the ARP learning.

# Chapter 08 Traceroute
UDP&TTL: hop-node returns ICMP-time-exceeded, end-node returns port unreachable
Strict Source Record Routing (SSRR):
Loose  Source Record Routing (LSSR):
traceroute -g: ip source routing option

# Chapter 09 IP Routing
+ ICMP-redirects
+ ICMP-router-solicitation
+ ICMP-router-advertisement
+ ip routing table:
    1. search and find a entry matching the destination completely
    2. search and find a entry matching the network id
    3. search and find the default entry
+ CIDR: Classless Interdomain Routing

# Chapter 10 Dynamic Routing Protocols
+ IGP (Interior Gateway Protocol) = HELLO, RIP, OSPF
+ EGP (Exterior Gateway Protocol) = EGP, BGP
+ RIP (Routing Information Protocol)
    + udp:520
    + Request and Response updates, Regular updates, Trigger updates
    + ip/mask - nexthop - metric
    + distance-vector protocol
+ OSPF (OSPF)
    + ip:
    + link-state protocol
+ BGP (Border Gateway Protocol)   = TCP,
+ CIDR (Classless Interdomain Routing)
    + prevent explosion in the size of the Internet routing table
    + best match is longest match
+ IP checksum includes ip layer only, TCP/UDP checksum include payload
+ Any IP protocol needs its checksum, like OSPF
+ Load balancing increases the chances of packets being delivered out of order,
  and possibly distorts the round-trip times calculated by the transport layer.

# Chapter 11 UDP
+ udp-checksum is optional
+ tcp-checksum is mandatory
+ ip-fragmentation
    + identification
    + flags:  more-fragments-bit, don't-fragment-bit
    + offset: 8-byte units offset
+ ip-fragmentation, if one fragment lose, then all retransmitted
+ Path MTU Discovery with UDP
+ udp input queue
+ udp server restricts local ip address
+ udp server restricts remote ip address
+ udp datagrams are demultiplexed based on dip:dport-sip:sport
+ udp datagrams retransmit with different ip identification, which restricts ip reassembling

# Chapter 12 Broadcasting and Multicasting
+ broadcasting and multicasting only apply to the udp
+ 4 ip broadcasting address forms:
    + Limited Broadcast = 255.255.255.255
    + Net-direct Broadcast = netid.255.255.255, class A network
    + Subnet-directed Broadcast = class B network
    + All-subnets-directed Broadcast
+ ip multicasting provides two services:
    + delivery to multiple destination
    + solicitation of servers by clients
+ broadcasting address ranges:
    + mac address: 01:00:5e:00:00:00 - 01:00:5e:ff:ff:ff (23bits)
    + ip address: 224.0.0.0 - 239.255.255.255 (28bits)
    + lower-23bits of multicast group ID is mapped into multicast-mac-address
+ Berkeley-derived implementations do not allow broadcast datagrams to be fragmented

# Chapter 13 IGMP Protocol
+ IGMP message sending rules:
    + a host sends an IGMP-report when and only when the first process joins a multicast group
    + a host does not send ICMP-report when processes leave a group, even the last one.
    + a router sends an IGMP-query periodically to check whether the hosts are still in groups
    + a host responds to an IGMP-query by sending one IGMP-report for each group having process
+ All-Hosts Groups = 224.0.0.1
    + host automatically joins this multicast group
    + membership in this group is never IGMP-report
+ IGMP ip-ttl always is 1

# Chapter 14 DNS: Domain Name System
+ FQDN (fully qualified domain name) = a domain name that ends with a p`eriod
+ top-level domains:
    + arpa. = a special domain name used for address-to-name mappings
    + generic domains (organizational domains) = .com, .edu, .gov, .int, .mil, .net, .org
    + geographical domains = 2-character based on country code (ISO 3166)
+ primary name server loads all the information for the zone from disk files
+ secondary name server obtains all the information from the primary one (zone transfer)
+ dns-flags:
    + AA: in response, authoritative answer,
    + TC: in response, truncated (more 512bytes in UDP)
    + RD: in request, recursion desired. </br>
          in response, 1: recursive query, 0: iterative query
    + RA: in response, recursion available.
+ dns-query-type, resource-records-type (RRs type)
    + A:     IP address
    + NS:    name server
    + CNAME: canonical name
    + PTR:   pointer record (`gethostbyaddr`)
    + HINFO: host info
    + MX:    mail exchange record
+ pointer queries (`gethostbyaddr`) mechanism
    + for an ip address a.b.c.d
    + use d.c.b.a.in-addr.arpa as dns-request's domain
    + the exactly domain will be returned as dns-response
+ DNS using TCP
    + When the resolver get the dns-response with flags-TC, it requests again via TCP
    + When secondary name server perform zone-transfer from the primary name server
+ packets-flow of rlogin client and server
    + rlogin-client request root-name-server to get name-server of rlogin-server-domain (query-type-A)
    + rlogin-client request name-server to get ip address via rlogin-server-domain     (query-type-A)
    + rlogin-client establishes TCP connection with rlogin-server
    + rlogin-server request root-name-server to get name-server of rlogin-client-ip (query-type-PTR)
    + rlogin-server request name-server to get domain via rlogin-client-ip
+ dns-resolver is client always, dns-name-server is both client and server.
+ InterNICs update root-name-server 3 times per week
+ dns-resolver sorts the A records, not the name-server
    + as the dns-resolver knows the network topology of client

# Chapter 15 TFTP: Trivial File Transfer Protocol
+ TFTP is intended to be used when bootstrapping diskless systems
+ TFTP is a stop-and-wait protocol with simplicity of implementation but low throughput
    + It can not use sending-window-like mechanism in TCP
+ TFTP has no security or even username and password
+ TFTP has 512bytes per segment limitation

# Chapter 16 BOOTP: Bootstrap Protocol
+ BOOTP uses UDP and works in conjunction with TFTP
+ BOOTP can work through routers whereas RARP cannot.
+ BOOTP is based on UDP, RARP is link-layer protocol.

# Chapter 17 TCP: Transmission Control Protocol
+ TCP provides a connection-oriented, reliable, byte stream service
+ TCP provides reliability by:
    + payload is broken into approximate size segment to send
    + a timer is maintained to wait acknowledgement of the sent segments
    + the acknowledge is sent when the payload is received
    + a checksum is maintained for the header and data
    + a seq in TCP header is used to receive data out of order and discard duplicated segments
    + TCP provides flow control
+ a TCP connection is uniquely identified by:
    + source      port
    + destination port
    + source      ip address
    + destination ip address
+ ISN = initial sequence number
    + not start from 0
    + it should be changed along with time
+ TCP provides full-duplex service: data can be flowing in both direction independently
+ TCP provides flow control via each end advertising a window size
+ Why are the source and destination port numbers at the beginning of the TCP header?
    + ICMP error must return at least 8 bytes beyond the ip header
    + ICMP error on TCP needs to examine the two port numbers to determine which connection

# Chapter 18 TCP Connection Establishment and Termination
+ tcp-flags: SYN/ACK/PSH/URG/FIN/RST
+ tcp-connection-establish: three-way-handshake
    + active open: send SYN (client-ISN)
    + passive open: send SYN+ACK (server-ISN)
    + active open: send ACK
+ tcp-connection-termination:
    + active close:  send FIN
    + passive close: send ACK
    + passive close: send FIN
    + active close:  send ACK
+ tcp-connection-termination: (simultaneous close)
    + both: send FIN, enters FIN_WAIT_1
    + both: receive FIN and send ACK, enters CLOSING
    + both: receive ACK, enters TIME_WAIT
+ MSS (Maximum Segment Size), Ethernet imply tcp-mss is 1460
+ tcp-half-close:
    + `shutdown(SHUT_RD)`: no send, local-`read()` returns EOF, local-`write()` is OK
    + `shutdown(SHUT_WR)`: send FIN, local-`write` return error, remote-`read()` returns EOF
+ 2MSL Wait State (TIME_WAIT): maximum segment lifetime
    + RFC 793, 2MSL = 360 seconds
    + only active-close-end enters TIME_WAIT
    + sock can not be created if the identical 5tuple is in TIME_WAIT
    + create sock with (SO_REUSEADDR) can pass through the 2MSL restriction
+ 2msl-quiet-time:
    + 2MSL only works without machines crashing
    + After machines reboot, wait MSL before creating TCP
+ tcp-options:
    + kind: 1byte
    + len:  1byte
    + value: N byte
+ maximum-initiated-new-connection per second = `(65536 - 1024) / 2MSL`
+ in TIME_WAIT, sock re-receives FIN, then re-send ACK and reset timer of 2MSL

# Chapter 19 TCP Interactive Data Flow
+ Delayed Acknowledgments (ACK piggyback)
    + Normally tcp doesn't send ACK the instant it receives data
    + Instead it delays the ACK, hoping to have data going in the same direction as the ACK.
    + Most of the implementation is 200ms
    + It also performs ack-compression
+ Nagle Algorithm
    + TCP small segments can not be sent util the outstanding dat is acknowledged
    + TCP small segments are collected and sent in a single segment when ACK arrives.

# Chapter 20 TCP Bulk Data Flow
+ tcp-sliding-window
    + sent and acknowledged
    + sent, not ACKed
    + can send
    + can not send util window moves
+ tcp-receive-window is normally 2048 bytes
+ congestion window (cwnd)
+ slow start: cwnd starts by 1, each time an ACK is received, cwnd increases 1
+ bandwidth-delay product = bandwidth(bits/sec) x round-trip-time(sec)
+ SYN and FIN occupy 1 byte in the sequence number space

# Chapter 21 TCP Timeout and Retransmission
+ tcp-timers for each connection
    + retransmission timer
    + persist timer keeps window size information flow
    + keepalive timer detects connection idle
    + 2MSL timer for TIME_WAIT state
+ RTT
    + `Err = M - A` (A: RTT)
    + `A  += 0.125 * Err`
    + `D  += 0.25 * (abs(Err) - D)`
    + `RTO = A + 4*D`
+ Karn's Algorithm
    + when a timeout and retransmission occur, RTT can not be updated.
+ cwnd increases rate:
    + if slow start, cwnd increases 1 every ACK received
    + if congestion avoidance, cwnd increases `1/cwnd` every ACK received
+ Why fast retransmit is triggered by third duplicated ACK not first ?
    + a single duplicated ACK may be caused by a lost-segment or a reordering-segment
+ When does the ssthresh increase?
    +
+ tcp slow start and congestion avoidance algorithm
    + initialize `cwnd = 1`, `ssthresh = 65535`;
    + tcp output send less than `min(cwnd, receiver-advertised-window)`
    + after congestion occurs, then </br>
      `ssthresh = 0.5 * min(cwnd, receiver-advertised-window)` </br>
      `cwnd = 1`
    + after congestion occurs, if `cwnd <= ssthresh`, doing slow start
    + after congestion occurs, if `cwnd  > ssthresh`, doing congestion avoidance
+ fast retransmit and fast recovery algorithm
    + after 3rd duplicated ACK is received, then </br>
      `ssthresh = 0.5 * min(cwnd, receiver-advertised-window)` </br>
      `cwnd = ssthresh + 3`
    + every another(more than 3rd) duplicated ACK is received, </br>
      cwnd increases 1, and transmit a packet
    + when newer ACK is received, </br>
      `cwnd = ssthresh` </br>
    + then back to the slow start (`cwnd <= ssthresh`), as the result </br>
      `cwnd = ssthresh+1` for this newer ACK is received

# Chapter 22 TCP Persist Timer
+ tcp-persist-timer performs `window probes`
    + tcp only ACKs segments containing data, but not acknowledgments
    + a mechanism is needed to detect receiver window turning back to nonzero
    + tcp allows to send 1 byte segments beyond the end of a closed window
    + then ACK will be returned with the window size, but not ACKed this byte
    + tcp-persist-timer periodically send 1 byte for `window probes`
+ SWS (Silly Window Syndrome):
    + when the `receiver-advertised-window` getting smaller,
    + small size of segments are exchanged across the connection, wasting the network resource
+ correct avoidance of silly-window-syndrome:
    + receiver must not advertise small window
    + sender send data only when one of conditions </br>
      a. send a full-sized MSS </br>
      b. send at least one-half of advertised-window </br>
      c. send everything when have no unacknowledged segment </br>
      d. send everything when Nagle algorithm is disabled </br>

# Chapter 23 TCP Keepalive Timer
+ normally keepalive timer runs on tcp-server
+ tcp-server periodically send keepalive-probe
    + net.ipv4.tcp_keepalive_time = 7200
    + net.ipv4.tcp_keepalive_intvl = 75
    + net.ipv4.tcp_keepalive_probes = 9 (total probe 10 times before close)
+ keepalive-probe-segment has no data, causing client to return ACK if still up
+ keepalive states
    + client is up and reachable, client response and server reset timer
    + client is down or unreachable, server redo N times keepalive-probes then close the connection
    + client has rebooted, client response a RST to the keepalive-probes, server close the connection
+ keepalive advantages:
    + tcp layer's keepalive-probe takes less network bandwidth than application-layer's one.
    + no keepalive-probes are sent unless the connection is idle
+ keepalive disadvantages:
    + keepalive-probe may cause a good connection to be closed as temporary network issues
    + keepalive-probe-timer's parameters are configurable for userspace processes

# Chapter 24 TCP Futures and Performance
+ tcp-option-window-scale: increase TCP's window over 65535 bytes
+ tcp-option-timestamp:
    + perform more accurate RTT measurement
    + provide protection against wrapped sequence numbers (in high speed datalink)
+ tcp-path-mtu-discovery:
    + once an intermediate router needs to fragment a TCP, it drops the packet and send icmp-cant-fragment
    + once icmp-cant-fragment is received, tcp decreases segment-size and retransmit
    + tcp re-enlarge segment-size periodically (10min in RFC 1191)
+ long-fat pipe: large bandwidth-delay products
    + tcp window size is 16bits, limiting the window size to 65535 bytes </br>
      tcp-option-window-scale increase tcp-window-size to 32bits
    + packet loss can reduce throughput drastically </br>
      SACKs (selective ACK) to handle this problem
    + tcp-seq-number is 32bits, causing wrapped seq-number in high speed network easily </br>
      tcp-option-timestamp to solve this problem
+ tcp-option-window-scale:
    + tcp header still holds 16bits window size
    + tcp-option-window-scale applies a 1-byte shift, from 1 to 14 </br>
      `real-window-size = 16bits-window-size * 2**(tcp-option-window-scale)`
    + only appears in SYN
    + negotiate via both SYN with this option </br>
      requester will disable this option if the 2nd-SYN without this option
+ tcp-option-timestamp-option:
    + sender places 32bits timestamps value
    + receiver echo back this timestamps value in ACK
+
