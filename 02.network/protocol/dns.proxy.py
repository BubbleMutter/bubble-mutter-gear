#!/usr/bin/env python3

import dns.resolver
import dns.message
import dns.rdatatype
import dns.rdtypes
import socketserver
import struct
import re

class DNSHandler(socketserver.BaseRequestHandler):
    def handle(self):
        data = self.request[0]
        sock = self.request[1]
        request = dns.message.from_wire(data)

        # Check if requested domain is interesting
        domain = request.question[0].name.to_text().strip('.')
        print("domain %s\n"%(domain))
        # Check if the domain is interesting and matches the specified pattern
        if domain.startswith('n') and re.match(r'n(\d{3})-(\d{3})-(\d{3})', domain):
            # Parse the three groups of digits in the domain name and construct the corresponding IP address
            match = re.match(r'n(\d{3})-(\d{3})-(\d{3})', domain)
            addr = f'10.{match.group(1)}.{match.group(2)}.{match.group(3)}'

            # Create the response message with the custom IP address
            response = dns.message.make_response(request)
            response.answer = [dns.rrset.from_text(str(request.question[0].name), 60, dns.rdataclass.IN, dns.rdatatype.A, addr)]
            response.set_rcode(dns.rcode.NOERROR)
            sock.sendto(response.to_wire(), self.client_address)
            return

        print("not interesting")
        # If domain is not interesting, return an error message
        response = dns.message.make_response(request)
        response.set_rcode(dns.rcode.NXDOMAIN)
        sock.sendto(response.to_wire(), self.client_address)
        return

if __name__ == '__main__':
    HOST, PORT = "0.0.0.0", 53
    # Create UDP server and handle incoming requests
    with socketserver.UDPServer((HOST, PORT), DNSHandler) as server:
        print("start server %s:%d\n"%(HOST, PORT))
        server.serve_forever()