#!/usr/bin/python

################################################################
# Lauren Egerton
# CS 372 / Spring 2017
# Project 1
# Filename : chatserve.py
# Description : Host A / Server
# Runs with command line: ./chatserve <port#>
# Cite1 - general resource: https://pymotw.com/2/socket/tcp.html
# Cite2: http://stackoverflow.com/questions/27360218/how-to-close-socket-connection-on-ctrl-c-in-a-python-programme
# Cite3 - general resource: https://docs.python.org/2/library/socket.html
# Cite4: http://stackoverflow.com/questions/35276924/simple-python-chat-app-with-sockets
# Cite5 - general resource: https://docs.python.org/2.6/library/sys.html
################################################################

import sys
import socket
import signal

# function to define exit terms of program (Cite2)
# param: signal number, current stack frame
# pre: must be used with 'signal.signal(signalnum, handler)' in main
# post: signal operation is caught, socket is closed, program exits
# ret: none
def sig_handler(signal, frame):
	print "     Server closed - Goodbye"
	serverSocket.close()
	sys.exit(0)

# function to set up server socket 
# param: string hostname, int port number
# pre: hostname and port number declared in main
# post: TCP connection started on socket s
# ret: socket s
def startUp(HOST, PORT):								
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)		# TCP connection
	s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)		# (Cite2) - reuse recent port #
	s.bind((HOST,PORT))
	return s

# function to send message to client
# param: none
# pre: must be an connected TCP socket to send to
# post: message of max 500 char sent to connectionSocket
# ret: returns 1 if user enters '\quit'
def sendMsg():	
	while True:
		input = raw_input(name+'>> ') 			# get user input
		if( len(name + ': ' + input) > 500):
			print " You can only send messages of %d characters" % (500-len(name+':'))
		if( len(name + ': ' + input) <= 500):
			break
	if(input == '\quit'):
		print " Connection terminated by server.\n"
		return 1
	reply = name + ': ' + input   			# add user handle
	connectionSocket.send(reply.encode())	# send reply

# function to receive message from client
# param: none
# pre: must be an connected TCP socket to send to
# post: message of max 500 char saved to 'msg'
# ret: msg
def recvMsg():
	msg = connectionSocket.recv(500).decode() 	
	return msg


################ MAIN FUNCTION ################
# param: takes two argv, name of program and port number
# pre: program must be run with a valid port number
# post: waits on a TCP connection for a client request.
# sends and receives max 500 char from client-TCP connection.
# ret: none
###############################################
signal.signal(signal.SIGINT, sig_handler)	# catch SIGINT / ctrl-c
signal.signal(signal.SIGPIPE, sig_handler)	# catch broken pipe

if(len(sys.argv) != 2):						# check usage and args
	print "Incorrect usage."
	print "Please use: ./chatserve <port#>"
	sys.exit(0)

name = 'Rosetta'							# set user A's handle - can be hardcoded

portNum = int(sys.argv[1])					# get port no. from command line

serverSocket = startUp('', portNum)			# start server 
serverSocket.listen(1)						# only allow one connection at a time
connectionSocket = None						# no client yet (connection flag)

# keep this connection open until A or B types 'quit' (Cite4)
while True:
	if connectionSocket is None:								
		print('     The server is waiting...')					# wait for a request			
		connectionSocket, client_addr = serverSocket.accept()	# client 'knocks', server starts new connection
		print >>sys.stderr, '     ** Connection started. **'				
	else:
		while True:
			gotMessage=recvMsg()								# receive message from Client-user
			if (gotMessage == 0):
				connectionSocket = None							# reset connection flag
				break
			if (gotMessage == '\quit'):
				connectionSocket = None							# reset connection flag
				break
			print gotMessage
			status=sendMsg()									# send message to Client-user
			if status==1:
				connectionSocket = None							# reset connection flag
				break

connectionSocket.close()


