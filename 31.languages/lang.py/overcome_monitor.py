# -*- encoding utf-8 -*-
from fabric import Connection, Config
from pprint import pprint
import time
import threading

directories = [
    "/proc/sys/net",
    "/proc/sys/net/bridge",
    "/proc/sys/net/core",
    "/proc/sys/net/ipv4",
    "/proc/sys/net/ipv4/conf",
    "/proc/sys/net/ipv4/neigh",
    "/proc/sys/net/ipv4/conf/all",
    "/proc/sys/net/netfilter",
    "/proc/sys/net/ipv6",
    "/proc/sys/net/ipv6/conf",
];

makeentries = [
    "process/abcd/efgh"
]

accounts = [
    {'host':'192.168.1.102', 'port':'22',
     'user':'root', 'password':'123456'},
];

def doTask(conn):
    while True:
        for e in makeentries:
            try:
                conn.run("make -C {}".format(e))
            except:
                pass
            time.sleep(4)
        for d in directories:
            try:
                conn.run("ls -1 {} | head -1".format(d))
            except:
                pass
            time.sleep(4)


def SshRandomCommand(account):
    conn=Connection(host=account['host'],
            port=account['port'],
            user=account['user'],
            connect_kwargs={ 'password': account['password'] })
    doTask(conn)

for account in accounts:
    thr = threading.Thread(target=SshRandomCommand,
                 args=(account,),
                 daemon=True)
    thr.start();