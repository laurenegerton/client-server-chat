# client-server-chat

** This program was developed to be run on Oregon State University's shell server (Linux) ***

1. Put the following three files in the same folder: chatclient.c, chatserve.py, chatProgram.sh

2. Change permissions for chatProgram.sh by typing: chmod +x chatProgram.sh

3. The following is a bash script that will compile the chatclient program. Run chatProgram.sh by typing: chatProgram.sh

4. The chatProgram.sh program will display the hostname of the current terminal. In THIS terminal, run the server program by typing in the following (note - I have specified a port number as the second argument, but you may use a different port number if this is already taken. For example, 30021, 30022, 30023): ./chatserve 30020

5. Open a NEW terminal window and ssh into a different OSU flipX. For example, if the server is running on flip2, ssh into flip1 or flip3.

6. Refer back to the first terminal window to note which flip server the server program is waiting on. Note which port # your server is running on. You will need this information to run your client and connect to the server. Run the client program by typing this information as follows (note - replace the X with the flip#, replace ‘port#’ with the port number used by the server: ./chatclient flipX.engr.oregonstate.edu port#

7. The client program will notify you that the client chats first. The ‘>>’ symbol means it is that user’s turn to enter a message and hit enter. The ‘:’ symbol shows a received message.

8. The current version of this program only works if the client and server take turns sending messages, with the client sending the first message.
