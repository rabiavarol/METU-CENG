Two scripts (server.py and client.py) were written for transferring large files by chunks with MSS from client side to server side.

To run server.py:
    python3 server.py <udp_server_listener_port> <tcp_server_listener_port> <protocol_no>

To run client.py:
    python3 client.py <server_address> <udp_server_listener_port> <tcp_server_listener_port> <udp_client_sender_port> <tcp_client_sender_port> <protocol_no>

for TCP, protocol_no = 0

for UDP, protocol_no = 1

----------------------- TCP -----------------------

As TCP is a connection oriented protocol (server must create socket for incoming request from client), you should first start server side, then start client side.

While implementing TCP application I followed the following steps:

- On the client side:
    1. Create a TCP socket, connect to the server and send starting time of TCP communication and the number of packets to be sent.
    2. Read 980 bytes chunks from the file to be transferred and measure the sending time with each chuck.
    3. Send packets to the server until all data is sent.

- On the server side:
    1. Create a TCP socket, bind it to server listener port and start to listen.
    2. Wait the client and when it comes, create new connection socket.
    3. Start timer of TCP transmission
    3. Get 1000 bytes packets. Since TCP is reliable and provides in-order
    byte-stream, there is no need to handle packet corruption, delaying/reordering.
    4. Extract sending time information from it and measure the receiving time.
    5. Calculate transmission time and record it in an array to be able to calculate average transmission time of all transmitted packets.
    6. Calculate total transmission time.

Since the server often does not receive all 1000 bytes at a time, I tackled this problem by cumulating received small packets until the total packet size becomes 1000 bytes.  
    
----------------------- UDP -----------------------

Since UDP is unreliable, we had to deal with possible packet/ACK delays, corruptions. That is, on top of the UDP I tried to implement RDT 3.0.

- On the client side:
    1. Create a UDP socket, bind to client port for incoming messages from the server.
    2. Set timeout to detect packet/ACK losts
    3. Send packet with sendto() method by every time specifying server address and listening port. This is because UDP is connectionless.
    3. Read 960 bytes chunks from the file to be transferred, measure the sending time, sequence number and checksum with each chuck.
    4. Keep sending each packet until its ACK received (retransmit if timeout occurs) 
    5. Ignore duplicate ACKs.
    6. Retransmit packet if its ACK is corrupted

- On the server side:
    1. Create a UDP socket, bind it to server listener port
    2. Receive packet from the client, check if it is the one the server awaits and if received checksum and currently calculated checksum match. If it is correct packet transmitted w/o corruption, send its ACK to the client.
    3. If duplicate packet is detected, send its ACK



