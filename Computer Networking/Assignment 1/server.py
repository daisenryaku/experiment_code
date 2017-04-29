# -*- coding: utf-8 -*-
#!/usr/bin/env python3
import sys
import socket
from socket import socket as Socket

#
from http import http_response
from http import http_request
from urls import http_urls

def main():
    serverPort = 8080

    with Socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:

        # 建立socket连接
        server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        server_socket.bind(('', serverPort))
        server_socket.listen(1)
        print("server ready")

        while True:

            with server_socket.accept()[0] as connection_socket:
                request = connection_socket.recv(1024).decode('ascii')
                #print(request)
                reply = http_handle(request)
                connection_socket.send(reply)
                #print(reply)

    return 0


def http_handle(request_string):
    request  = http_request(request_string)
    filepath, arg = http_urls(request)
    response = http_response(filepath, arg)
    return response

if __name__ == "__main__":
    sys.exit(main())
