# import socket module
from socket import *
import sys

# Create a (TCP) server socket
serverSocket = socket(AF_INET, SOCK_STREAM)

# Prepare a server socket  
PORT = 9841  
serverSocket.bind(("", PORT))
serverSocket.listen(1)

while True:
    # Establish the connection
    print('Ready to serve...')

    connectionSocket, addr = serverSocket.accept()

    try:
        # Receive the HTTP request message from the client
        message = connectionSocket.recv(1024).decode()

        filename = message.split()[1]  # Get the requested file

        # Open the requested file
        f = open(filename[1:])

        # Read the contents of the file
        outputdata = f.readlines()

        # Send the HTTP response header to the client
        connectionSocket.send("HTTP/1.1 200 OK\r\n".encode())
 	connectionSocket.send("Content-Type: text/html\r\n".encode())
	connectionSocket.send("\r\n".encode())

        # Send the content of the requested file to the client
        for i in range(0, len(outputdata)):
            connectionSocket.send(outputdata[i].encode())

        connectionSocket.send("\r\n".encode())

        # Close the connection
        connectionSocket.close()

    except IOError:
        # Send a 404 Not Found response message to the client
        connectionSocket.send('HTTP/1.1 404 Not Found\r\n'.encode())
	connectionSocket.send("Content-Type: text/html\r\n")
        connectionSocket.send("\r\n")
        connectionSocket.send("<html><head></head><body><h1>404 Not Found</h1></body></html>\r\n")

        # Close the connection
        connectionSocket.close()

# Close the server socket
serverSocket.close()
sys.exit()  # Terminate the program after sending the corresponding data
