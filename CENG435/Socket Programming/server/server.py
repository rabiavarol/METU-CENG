import hashlib
import os
import sys
import socket
import time
from hashlib import sha1


def calculate_checksum(packet):

    curr_checksum = hashlib.sha1()
    curr_checksum.update(packet)
    curr_checksum = curr_checksum.digest()[:10]
    return curr_checksum

def UDP_transmission(server_port):
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    server_socket.bind(('', server_port))
    
    num_of_packets = 0
    very_first_packet_received = False
    while not very_first_packet_received:
        # First learn how many packet will come
        very_first_packet, clientAddr = server_socket.recvfrom(10)
        if very_first_packet:
            num_of_packets = int.from_bytes(very_first_packet, 'big')
            print("Very first packet received (Num of packets: {})".format(num_of_packets))
            # Send ACK0 for very first packet
            reply = (0).to_bytes(5, 'big')
            # Send reply to whom you got the very first message
            server_socket.sendto(reply, clientAddr)
            very_first_packet_received = True

    # Create and open output file with append mode in order to
    # append chunks one after the other
    output_file = open("transfer_file_UDP.txt", "ab")
    clientAddr = 0   
    expected_seq_num = 0
    num_of_received_packets = 0
    # Store all packet transmission times in an array
    transmission_times = []

    while True:
        # Since recvfrom(x) does not guarantee for receiving all x bytes at a time
        # we should get 1000 bytes packet cummulatively
        received_until_now = 0
        packet = b""
        while received_until_now < 1000:
            curr_packet, clientAddr = server_socket.recvfrom(1000-received_until_now)
            packet += curr_packet
            received_until_now += len(curr_packet)
            # Last packet may be less than 1000 bytes
            if num_of_received_packets == num_of_packets-1:
                break
        # Measure time with 3 precision
        receiving_time = int(time.time()*1000)
        receiving_time /= 1000 
        sending_time = int.from_bytes(packet[:20], 'big')
        sending_time /= 1000
        received_checksum = packet[20:30]
        received_seq_num = int.from_bytes(packet[30:40], 'big')
        chunk = packet[40:]
        to_be_hashed_part = packet[30:]
        curr_checksum = calculate_checksum(to_be_hashed_part)
        
        if expected_seq_num == received_seq_num:
            if curr_checksum == received_checksum:
                # Packet with expected_seq_num got w/o any corruption
                reply = received_seq_num.to_bytes(10, 'big')
                # Send reply to whom you got the packet
                server_socket.sendto(reply, clientAddr)
                num_of_received_packets += 1
                expected_seq_num += 1
                transmission_times.append(receiving_time-sending_time)
                output_file.write(chunk)
            else:
                # Packet is corrupted
                continue
        elif expected_seq_num > received_seq_num:
            # Already received packet is received
            reply = received_seq_num.to_bytes(10, 'big')
            server_socket.sendto(reply, clientAddr)
        
        if num_of_received_packets == num_of_packets:

            avg_time = (sum(transmission_times) / num_of_packets) * 1000.0 # convert to ms
            total_time = sum(transmission_times) * 1000.0 # convert to ms
            print("UDP Packets Average Transmission Time: {} ms".format(avg_time))
            print("UDP Packets Total Transmission Time: {} ms".format(total_time))
            server_socket.close()
            break

    output_file.close()
    


def TCP_transmission(tcp_server_listener_port):
    try:
        server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    except socket.error as err:
        print("Socket creation failed with error {}".format(err))
        return

    # Empty string for ip field means that the server can listen to 
    # incoming request from other computers as well,
    # i.e. for example if we had given localhost, 
    # it would listen to local processes only.
    server_socket.bind(("", tcp_server_listener_port))
    server_socket.listen(5)
    # Create and open output file with append mode in order to
    # append chunks one after the other
    output_file = open("transfer_file_TCP.txt", "ab")
    # Store all packet transmission times in an array
    transmission_times = []
    num_of_packets = 0
    num_of_received_packet = -1
    # The starting time measures by the client
    tcp_starting_time = 0
    # Receiving time of packets
    receiving_time = 0 
    # When the client wants to connect, create connection socket
    connection_socket, client_address = server_socket.accept()
    # Connection set up with client_adress, however we are not going to
    # se client_address since tcp is a connection oriented and this 
    # connection_socket is reserved for this client only.
    
    i = 0
    while True:
        # Just for the first time, client sends the starting time of tcp 
        # transmission and total number of packets it will send
        if i == 0:
            # 20 bytes for the starting time with 3 precision
            # 10 bytes for the number of packets 
            very_first_packet = connection_socket.recv(30)
            tcp_starting_time = int.from_bytes(very_first_packet[-20:], 'big')
            tcp_starting_time /= 1000 
            num_of_packets = int.from_bytes(very_first_packet[:-20], 'big')
            num_of_received_packet = 0
            i += 1
        else:
            # Since recv(x) does not guarantee for receiving all x bytes at a time
            # we should get 1000 bytes packet cummulatively
            received_until_now = 0
            partial_packets = b""
            while received_until_now < 1000:
                curr_packet = connection_socket.recv(1000-received_until_now)
                partial_packets += curr_packet
                received_until_now += len(curr_packet)
                # Last packet may be less than 1000 bytes
                if num_of_received_packet == num_of_packets-1:
                    break
            # After getting 1000 bytes (1 packet with MSS we want), measure the receiving time
            receiving_time = int(time.time()*1000)
            receiving_time /= 1000
            # 20 bytes for the sending time with 3 precision
            sending_time = int.from_bytes(partial_packets[-20:], 'big')
            sending_time /= 1000
            # Remaining is the actual data
            packet = partial_packets[:-20]
            if not packet:
                break 
            num_of_received_packet += 1            
            transmission_times.append(receiving_time - sending_time)
            # Write the actual data to transfer_file_TCP.txt
            output_file.write(packet)

        # If the server has got all the packets, the transmission is done,
        # close the socket
        if num_of_received_packet == num_of_packets:
            avg_time = (sum(transmission_times) / num_of_packets) * 1000.0 # convert to ms
            total_time = sum(transmission_times) * 1000.0 # convert to ms
            print("TCP Packets Average Transmission Time: {} ms".format(avg_time))
            print("TCP Packets Total Transmission Time: {} ms".format(total_time))
            connection_socket.close()
            break

    output_file.close()
    return True

def main():
    num_of_args = len(sys.argv)
    if num_of_args == 4:
        udp_server_listener_port = sys.argv[1]
        tcp_server_listener_port = sys.argv[2] 
        protocol = int(sys.argv[3])
    else:
        print("Incorrect number of arguments!")
        return
    
    if protocol == 0:
        TCP_transmission(int(tcp_server_listener_port))
    else:
        UDP_transmission(int(udp_server_listener_port))
    

if __name__=="__main__": 
    main() 
