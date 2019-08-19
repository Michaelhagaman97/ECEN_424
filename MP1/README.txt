Issues:
We ran into some issues we were unable to iron out due to our lack of knowledge of C++. The client will connect to the server, but upon receiving the connection the server terminates. The server does accept the client socket. When writenIN is called on the server side we get a “Broken pipe error” according to the GDB and ends the server. The client then breaks out of the echo loop since the server is down. The client code is functional but with the issue coming from the writeIN function we were unable to perform the test cases with our code.

This what the our code should have done.
Test Cases
1. Line of text terminated by a newline
- We entered the following line of text on the client before pressing the enter key: “Hello”
- The server will send the message “Hello” back to the client


2. Line of text the maximum line length without a newline
- When the character input is filled to the maximum length, the input will automatically be terminated and the input sent to the server
- The server will send the message back to the client 


3. Line with no characters and EOF
- When the client reads a line with no characters and EOF, it will terminate the connection with the server and close the socket
- When the server detects the closed socket, it will terminate the child process


4. Client terminated after entering text: “Hello”
- Nothing will be transmitted from the client to the server unless the size of the line reaches the size of the buffer or the enter key is invoked
- Terminating the client will cause the socket to close on both the client and the server
- Closing the socket will cause the child process to terminate


5. Three clients connected to the server.
- The server will use the fork function to create a child process for each of the connected clients
- When a client sends data to the server, the corresponding child process for that client will handle the data and echo the data back to the client


Architecture
Server Side
When the server is started, it is given a port number and opens a socket on the given port at the server’s current IP Address through the bind and listen commands. The server will then wait for a client side socket to request a connection and accept the connection if the IP and port number match. Now the server will read information being written by the client and then write that information back to the client. This cycle will continue until the server receives an EOF from the client. At this point, the server will return to the listen state and wait for a new connection to be made.


Client side
When the client is started, it is given an IP address and port number. The client will then open a socket at this port and request a connection with the server. If/when the connection is accepted by the server, the client will then be able to write and information to the server which will then send that same information back to the client, who will read the incoming data and display it to the user. This cycle will continue until the client terminates the connection. For the client to terminate the connection, it needs to send an EOF to the server. At this point, both the client and the server will terminate the connection.


Individual Roles

Michael Hagaman
Wrote the code for the client and wrote writeIN and readline functions. Assisted with server side. Worked on debugging and finding solutions for issues. Uploaded final code.

Layne Jackson
Wrote the code for the server side and the Makefile. Worked on debugging and finding solutions for issues. Created the github and uploaded the code.
