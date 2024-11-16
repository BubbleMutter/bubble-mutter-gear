require 'socket'
server = TCPServer.new 12380
while session = server.accept
  request = session.gets
  session.print [
    "HTTP/1.1 200",
    "",
    session.peeraddr.last,
  ].join("\r\n")
  session.close
end
