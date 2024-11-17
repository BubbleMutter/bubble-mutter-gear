### utils libs
+ `urllib2` instead of `urllib`, use to download file
+ `urlparse`
+ cgi, cgitb,
+ Cookie on Server
+ cookielib in client
+ `socket`

### task element
+ `ThreadingMixIn` and `ForkingMixIn` for asynchronous handler

### server module
+ SocketServer
+ TCPServer, UDPServer
+ HttpServer
+ Twisted

### concurrent
+ `threading`, `multiprocessing`
+ `asynchat` is better than `asyncore`
+ `from select import select, poll, kqueue, kevent`  IO-Module
+ libevent, a cross-platform modern wapper
	+ `/dev/poll, event ports`, in solaris only
	+ `kqueue`, in freebsd
	+ `select`, in Posix
	+ `select()` in Windows
	+ `poll()` and `epol()` in Linux

### inside multiprocessing module
+ `Pool, Process`, worker module
+ `Lock, RLock`, as mutex in posix
+ `Queue`, as message-queue in posix, synchronized-blocked
+ `Value, Array`, as shared-memory in posix
+ `Semaphore, BoundedSemaphore`
+ `Condition`
+ `Event`
+ `Barrier`

### amazing IO module
+ `Sanic` & `uvloop` => high performance