# IP/TCP low level data structure

## Packet
In networking, a packet is a small segment of a larger message. Data sent over computer networks*, such as the Internet, is divided into packets. These packets are then recombined by the computer or device that receives them. 

## Segment
The original TCP RFC is kind of fuzzy with how it uses the term ***segment***.

In some cases, the term "segment" refers to just the current piece of the application data stream that's being transmitted, which excludes the TCP headers. For example, the TCP "Maximum Segment Size" (MSS) is the maximum size of the application data chunk in this message, not counting the TCP headers.

But in other cases the term "segment" includes the whole TCP message, including the TCP headers. In fact in at least one case the spec mentions TCP segments with no application data (such as plain Acks).

A single whole IP message is a ***datagram***.

The original IP RFC refers to link-layer messages as "packets". IP datagrams can be broken into "fragments" to fit into the packet size limits on small-packet networks.

The IEEE 802.3 / Ethernet link layer refers to a single contiguous physical-layer transmission as a "packet". The MAC data-link portion of the packet is called the "frame". The frame begins with the destination MAC address and ends with the Frame Check Sequence. The part of the frame that may contain an IP datagram (or fragment thereof) is called the "MAC Client Data field".

So, technically, there is no such thing as a "TCP packet" or an "IP packet". Packets are terms from the layers below IP. TCP has "segments", and IP has "datagrams"

##MSS

More specifically, MSS is the largest [TCP (Transport Control Protocol)](https://www.cloudflare.com/learning/ddos/glossary/tcp-ip/) segment size that a network-connected device can receive. MSS defines "segment" as only the length of the payload, not any attached headers. MSS is measured in bytes.

![Data packet headers and payload - TCP segment and MSS](https://www.cloudflare.com/resources/images/slt3lc6tev37/29tC841gKxTb6c2fUFJro6/9c49654618fe84f3c00700629f30a6e4/MSS_TCP_segment_packet_diagram.png)

MSS is determined by another metric that has to do with packet size: [MTU](https://www.cloudflare.com/learning/network-layer/what-is-mtu/), or the maximum transmission unit, which does include the TCP and [IP (Internet Protocol)](https://www.cloudflare.com/learning/ddos/glossary/internet-protocol/) headers. To continue the analogy, MTU measures the total weight of the truck and its trailer and cargo, instead of just the trailer and cargo.

Essentially, the MSS is equal to MTU minus the size of a TCP header and an IP header:

*MTU - (TCP header + IP header) = MSS*

One of the key differences between MTU and MSS is that if a packet exceeds a device's MTU, it is broken up into smaller pieces, or "fragmented." In contrast, if a packet exceeds the MSS, it is dropped and not delivered

## Ethernet packet


## IP datagram
Thus, the maximum size of an IP datagram is 65,535 byte. They can be devided by the sender / router if it exceedes MTU size. An IP packet consists of a header section and a data section. An IP packet has no data checksum or any other footer after the data section. Typically the link layer encapsulates IP packets in frames with a CRC footer that detects most errors, many transport-layer protocols carried by IP also have their own error checkin  


![](images/socket/20230819125128.png)

### Intresting fields inside IP datagram:

**Identification** 
if three packets are sent from host A to host B and each must be fragmented into four fragments:
* the four fragments of the first packet will share the same Identification field value  
* the four fragments of the second packet will share the same Identification field value, which will be different to the value used with the fragments created from the first packet
* the four fragments of the third packet will share the same Identification field value, which will be different to the value used with the fragments created from the first and second packets

![](images/socket/20230819131130.png)

**Time to live (TTL)**
the number of network hops a packet can make before it is discarded
 
**Fragment offset**
This field specifies the offset of a particular fragment relative to the beginning of the original unfragmented IP datagram. The fragmentation offset value for the first fragment is always 0. The field is 13 bits wide, so that the offset can be from 0 to 8191 (from (20  –1) to (213 – 1)). Fragments are specified in units of 8 bytes, which is why fragment length must be a multiple of 8.[37] Therefore, the 13-bit field allows a maximum offset of (213 – 1) × 8 = 65,528 bytes, with the header length included (65,528 + 20 = 65,548 bytes), supporting fragmentation of packets exceeding the maximum IP length of 65,535 byte

[IP fragmentation detailed](https://packetpushers.net/ip-fragmentation-in-detail/)

## TCP message

# Transfer rules
when you send data that is bigger than ip mtu, it is put into one ip packet, but the ip layer then find the ip packet is too long to transmit，so it will split big packet into several pieces, each of them have the same Identifier but with different offset and data length. the receive side is responsible for collecting all the pieces, after received all the pieces it will reassemble all the pieces into one whole ip packet and push it to upper protocol layer.

but the tcp layer have different behavior. when you send a data that is big enough, the tcp layer won't put the data into one tcp packet then split them into pieces(but ip does), it retrieve a part of the raw data into one tcp packet and then push the tcp packet to ip layer, the length of the tcp packet is determined by mss, later it will retrieve another part of the rest data into another tcp packet, and repeat the process till all the data are transmitted.

if tcp don't use mss，it is horrible. assume you send a data that is bigger than mss, it will be put into only one tcp packet（data is not split into small pieces due to mss not used）, tcp packet is bigger than ip mtu, so the ip will split the tcp packet into pieces. the tcp packet will re-transmit if any one of the pieces is lost, waste time and bandwith

if there is new data to send
  if the window size >= MSS and available data is >= MSS
    send complete MSS segment now
  else
    if there is unconfirmed data still in the pipe
      enqueue data in the buffer until an acknowledge is received
    else
      send data immediately
    end if
  end if
end if

------------------
Transmission Control Protocol accepts data from a data stream, divides it into chunks, and adds a TCP header creating a TCP segment. The TCP segment is then encapsulated into an Internet Protocol (IP) datagram, and exchanged with peers.[4]

Window size (16 bits)
The size of the receive window, which specifies the number of window size units (by default, bytes) (beyond the segment identified by the sequence number in the acknowledgment field) that the sender of this segment is currently willing to receive
1


Acknowledgment number (32 bits)
If the ACK flag is set then the value of this field is the next sequence number that the sender of the ACK is expecting. This acknowledges receipt of all prior bytes (if any). The first ACK sent by each end acknowledges the other end's initial sequence number itself, but no data.

There are a few key features that set TCP apart from User Datagram Protocol:

-------------------

Ordered data transfer: the destination host rearranges according to sequence number[3]
Retransmission of lost packets: any cumulative stream not acknowledged is retransmitted[3]
Error-free data transfer[15]
Flow control: limits the rate a sender transfers data to guarantee reliable delivery. The receiver continually hints the sender on how much data can be received (controlled by the sliding window). When the receiving host's buffer fills, the next acknowledgment contains a 0 in the window size, to stop transfer and allow the data in the buffer to be processed.[3]
Congestion control
( slow-start, congestion avoidance, fast retransmit, and fast recovery)
--------------------------
The throughput of a communication is limited by two windows: the congestion window and the receive window. The congestion window tries not to exceed the capacity of the network (congestion control); the receive window tries not to exceed the capacity of the receiver to process data (flow control)
---------
 window scale - MAX value establishes during SYN packet(connection)! reciever can advertises futher recive window size in ACKs. The maximum size is 65K, but it can be scaled up to 1GB! The settings is controled by /proc/sys/net/ipv4/tcp_window_scaling=0/1 in Linux to enable or disable it

-----------------

------------
n TCP, the congestion window is one of the factors that determines the number of bytes that can be outstanding at any time. The congestion window is maintained by the sender. Note that this is not to be confused with the sliding window size which is maintained by the receiver. The congestion window is a means of stopping a link between the sender and the receiver from becoming overloaded with too much traffic. It is calculated by estimating how much congestion there is on the link.

 The congestion (sender) window keeps growing dynamically until a timeout occurs. On timeout:

Congestion window is reset to 1 MSS.
ssthresh is set to half the congestion window size before the timeout.
slow start is initiated.

---------------
Fast retransmit (typical algos Reno, Vega, TCPH)
----------------
Duplicate acknowledgement is the basis for the fast retransmit mechanism which works as follows: after receiving a packet (e.g. with sequence number 1), the receiver sends an acknowledgement by adding 1 to the sequence number (i.e. acknowledgement number 2) which means that the receiver received the packet number 1 and it expects packet number 2 from the sender. Suppose that three subsequent packets have been lost. In the meantime the receiver receives packet numbers 5 and 6. After receiving packet number 5, the receiver sends an acknowledgement, but still only for sequence number 2. When the receiver receives packet number 6, it sends yet another acknowledgement value of 2. Because the sender receives more than one acknowledgement with the same sequence number (2 in this example) this is called duplicate acknowledgement.

The fast retransmit enhancement works as follows: if a TCP sender receives a specified number of acknowledgements which is usually set to three duplicate acknowledgements with the same acknowledge number (that is, a total of four acknowledgements with the same acknowledgement number), the sender can be reasonably confident that the segment with the next higher sequence number was dropped, and will not arrive out of order. The sender will then retransmit the packet that was presumed dropped before waiting for its timeout

---------------
The term "window" on the transmitter side represents the logical boundary of the total number of packets yet to be acknowledged by the receiver. 


---------
SACKs work by appending to a duplicate acknowledgment packet a TCP option containing a range of noncontiguous data received. In other words, it allows the client to say "I only have up to packet #1 in order, but I also have received packets #3 and #4". This allows the server to retransmit only the packet(s) that were not received by the client.

Support for SACK is negotiated at the beginning of a TCP connection; if both hosts support it, it may be used

----------------

netdev_max_backlog - 2000 default per CPU queue to sore data from NIC and push it to sockets