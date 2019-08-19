MP3 Report
Michael Hagaman – report, server, RRQ, report, testing

Usage: 
To run the code you can use the make file to compile. To run the server us ./MP3 [portnumber}. I used the TFTP client on ubuntu. To get this to work I used “tftp 127.0.0.1 [portnumber]”. The server and client MUST run in different directories. 
Code:
The server connects to the TFTP client and receives the initial get/put command. The server is able to parse the message and get the message type, file name, and transfer mode. The the TFTP clients sends a get request I am able to send the first 512 bytes (first packet) of data and the client saves said data as a file. I am unable to send more then one packet per transfer. This works for both octet and netascii. After the first packet is sent the client times out because the server is unable to execute the sendto function. The server is able to read the data from the file and insert it into the struct. The struct contains 3 message types for request, data transfer and ack. 
Testing:
I am able to send a file that is less then 512 bytes as see in the pdf report. When ever I send a file that is greater then 512 bytes the first 512 bytes is transferred and saved in the client directory. 
Bugs:
The server is unable to send more then one packet. After the first packet is sent the server is unable to execute the sendto code. Sometimes the code hangs before it constructs the out going message. When this happened, you need to remake the code. Run “make clean” then “make” this should fix the hanging error. Other than that, I cannot get past the first packet.  
