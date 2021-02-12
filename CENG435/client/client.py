import os
import sys
import math
import time
import socket
import hashlib
from functools import partial

def make_checksum(seq_chunk):
    # Hash sequence number + chunk and take first 10 bytes
    hashed_checksum = hashlib.sha1()
    hashed_checksum.update(seq_chunk)
    checksum = hashed_checksum.digest()[:10]
    return checksum

def UDP_transmission(server_address, server_port, client_port):

    SERVER = (server_address, server_port)
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    client_socket.bind(("localhost", client_port))
    client_socket.settimeout(1)

    file_size = os.stat("transfer_file_UDP.txt").st_size
    num_of_packets = math.ceil(file_size / 960)
    num_of_packets_bytes = num_of_packets.to_bytes(10, byteorder='big')
    
    very_first_packet_sent = False
    while not very_first_packet_sent:
        # First send how many packets the server should expect
        client_socket.sendto(num_of_packets_bytes, SERVER)
        try:
            reply, serverAddr = client_socket.recvfrom(5) 
            # Server sent ACK-1 for very first packet
            if int.from_bytes(reply, 'big') == 0:
                very_first_packet_sent = True
        except socket.timeout:
            continue

    
    # Packet's sequence numbers start with 0
    seq_num = 0
    # Read file, prepare packets and store them in an array 
    udp_packets_array = []
    with open("transfer_file_UDP.txt", 'rb') as transfer_file:
        """
        20 bytes for timestamp
        10 bytes for checksum   
        10 bytes for sequence number
        960 bytes for real data
        -------------------------------------
        TOTAL 1000 bytes
        """
        for chunk in iter(partial(transfer_file.read, 960), b''):
            seq_num_bytes = seq_num.to_bytes(10, 'big')
            seq_chunk = seq_num_bytes + chunk
            checksum = make_checksum(seq_chunk) 
            segment = checksum + seq_chunk
            udp_packets_array.append(segment) 
            seq_num += 1
            
    seq_num = 0 
    num_of_retransmitted_packets = 0
    while seq_num < num_of_packets:
        ack_received = False
        # Wait ACK of the packet with seq_num
        while not ack_received:
            send_time = int(time.time()*1000) # get 3 precision
            send_time = send_time.to_bytes(20, byteorder='big') # 20 bytes
            packet = send_time + udp_packets_array[seq_num]
            client_socket.sendto(packet, SERVER)
            try:
                # Receive 10 bytes ACK number 
                reply, serverAddr = client_socket.recvfrom(10) 
                reply_ack_num = int.from_bytes(reply, 'big')
            except socket.timeout:
                # ACK or Packet is not received, retransmit the packet
                num_of_retransmitted_packets += 1
                break

            else:
                #Ignore duplicate ACK
                if reply_ack_num < seq_num:
                    break
                # ACK received for seq_num
                elif reply_ack_num == seq_num:
                    ack_received = True
                    seq_num += 1
                # ACK may be corrupted, retransmit the packet
                else:
                    num_of_retransmitted_packets += 1
                
    print("UDP Transmission Re-transferred Packets: {}".format(num_of_retransmitted_packets))
    client_socket.close()


def TCP_transmission(server_address, server_port):
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # Client wants to send data to the specified server,
    # thus it connects to it
    client_socket.connect((server_address, server_port))

    # Server needs to know how many packets it will receive 
    # for not listening forever 
    file_size = os.stat("transfer_file_TCP.txt").st_size
    num_of_packets = math.ceil(file_size / 980)
    num_of_packets = num_of_packets.to_bytes(10, byteorder='big')
    # Measure the starting of TCP communication with 3 precision
    tcp_starting_time = int(time.time()*1000)
    tcp_starting_time = tcp_starting_time.to_bytes(20, byteorder='big')
    
    first_packet = num_of_packets + tcp_starting_time
    # Send these two information only once
    client_socket.send(first_packet)

    # Open the file to be sent in binary mode
    with open("transfer_file_TCP.txt", 'rb') as transfer_file:
        """
        20 bytes for time
        980 bytes for data
        --------------------
        TOTAL 1000 bytes
        """
        # Read the file with 980 byte chunks at a time and add
        # sending time info besides it
        for chunk in iter(partial(transfer_file.read, 980), b''):
            sending_time = int(time.time()*1000) # get 3 precision
            sending_time = sending_time.to_bytes(20, byteorder='big')
            packet = chunk + sending_time
            # Don't need to specify where to send the packet, it will 
            # send to the server it connectes to.
            client_socket.sendall(packet)

    # All data has been sent, close the socket
    client_socket.close() 
    return True      
 
def main():
    num_of_args = len(sys.argv)
    if num_of_args == 7:
        server_address = sys.argv[1]
        udp_server_listener_port = sys.argv[2]
        tcp_server_listener_port = sys.argv[3] 
        udp_client_sender_port = sys.argv[4]
        tcp_client_sender_port = sys.argv[5]
        protocol = int(sys.argv[6]);
    else:
        print("Incorrect number of arguments!")
        return
        
    if protocol == 0:
        TCP_transmission(server_address, int(tcp_server_listener_port))
    else:
        UDP_transmission(server_address, int(udp_server_listener_port), int(udp_client_sender_port))
                     

if __name__=="__main__": 
    main() 
