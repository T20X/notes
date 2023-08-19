----
commands
---
sudo tcpdump -ni lo port 5000 -ttttt -S

-------------
network  - big endian (most significant byte is at lower address)
CPU - little endian (most significant byte is at higher address)


while it is said that TCP,UDP is big endiean it only matters for TCP,UDP headers not actual payload itself


----------------
<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [-n Does not resolve the service name](#-n-does-not-resolve-the-service-name)
- [-t only show tcp sockets](#-t-only-show-tcp-sockets)
- [-l Displays LISTEN-state sockets](#-l-displays-listen-state-sockets)

<!-- /code_chunk_output -->
-

NIC errros
-------
overruns - not enogh space in ring buffers
dropped - not enough space in buffers TCP/UDP.
errors - ill-formed packets

netstrat -s //networking stats

OOB data - priority delivery

RoCE stands for RDMA over Converged Ethernet. It is a standard that enables
 passing RDMA traffic over an Ethernet network.

FCoE - Fibre Channel over Ethernet (FCoE) is a computer network technology that encapsulates Fibre Channel frames 
over Ethernet network

tcp_window_scaling (Boolean; default: enabled; since Linux 2.2)
              Enable  RFC 1323  TCP  window  scaling.   This feature allows the use of a large window (> 64 kB) on a TCP connection, should the other end support it

tcp_fin_timeout (integer; default: 60; since Linux 2.2)
              This specifies how many seconds to wait for a final FIN packet before the socket is forcibly closed.  This is strictly a violation of  the  TCP  specification,  but  required  to  prevent
              denial-of-service attacks.  In Linux 2.2, the default value was 180


tcp_mem [low,pressure,limit] - in PAGES!
 low - no regulation for memory allocation!
 pressure - starts to regulate buffer memory allocation!
 limit - no more allocation!This value overrides any other limits imposed by the kernel

        tcp_retries1 (integer; default: 3; since Linux 2.2)
              The number of times TCP will attempt to retransmit a packet on an established connection normally, without the extra effort of getting the network layers involved.  Once  we  exceed  this
              number of retransmits, we first have the network layer update the route if possible before each new retransmit.  The default is the RFC specified minimum of 3.

       tcp_retries2 (integer; default: 15; since Linux 2.2)
              The  maximum  number of times a TCP packet is retransmitted in established state before giving up.  The default value is 15, which corresponds to a duration of approximately between 13 to
              30 minutes, depending on the retransmission timeout.  The RFC 1122 specified minimum limit of 100 seconds is typically deemed too short.

TCP Segmentation offload - These days many NICs can create headers/checksum for a given user data autmatically given a template from kernel! this can save massive amount of CPU time.

TCP_CORK aggressively accumulates data. If TCP_CORK is enabled in a socket, it will not send data until the buffer fills to a fixed limit (MSS size). But still up until 200ms  - then it will be sent anyway!

HTTP/2 connections are persistent. For best performance, it is expected that clients will not close connections until it is determined that no further communication with a server is necessary (for example, when a user navigates away from a particular web page) or until the server closes the connection.

We don't use websocket. But, why is application-level keep alive better? It adds a complexity level to the application to deal with infrastructure setup. – Sumrak May 24 '16 at 21:51
There are a variety of reasons. The primary reason is that it's better to add completely portable complexity to the application than to demand an unusual system-level configuration that affects everything. A key secondary reason is that using TCP keepalives this way is not guaranteed to work (see, for example, RFC 1122 section 4.2.3.6). TCP keepalives exist as a kludge to work around applications and protocols that weren't designed to work with TCP

TCP_NODELAY ON means send the data (partial frames) the moment you get, regardless if you have enough frames for a full network packet.
TCP_NODELAY OFF means Nagles Algoritm which means send the data when it is bigger than the MSS or waiting for the receiving acknowledgement before sending data which is smaller.
TCP_CORK ON means don't send any data (partial frames) smaller than the MSS until the application says so or until 200ms later.
TCP_CORK OFF means send all the data (partial frames) now


 Both this post and the link omit an important conceptual reason to want to use shutdown: to signal EOF to the peer and still be able to receive pending data the peer sent.

SYN: a synchronization message typically used to request a connection between a client and a server
ACK: an acknowledgment message employed to declare the receipt of a particular message
FIN: a message that triggers a graceful connection termination between a client and a server
RST: a message that aborts the connection (forceful termination) between a client and a server

Linger off means that close() doesn't block. Linger on with a positive timeout means that close() blocks for up to the timeout. Linger on with a zero timeout causes RST, and this is what the question is about
When you close a socket with the SO_LINGER time set to zero:
TCP discards any data in the send buffer
TCP sends a RST packet to the peer
The socket resource are deallocated.
The socket does not enter TIME-WAIT


-----
RST
-----

Linux only tear down a TCP connection if the RST sequence number is the next expected sequence number

A RST isn't ordinarily sent between peers in a normal connection termination. A FIN is. When you kill the client, a FIN is sent on the connection to indicate to the server that the client won't be sending any more data.  But the server is apparently not paying attention to the FIN it receives when the client is killed (i.e. it would need to attempt a recv on the socket and react appropriately to the end-of-file indication it will get -- usually that means close its own socket). Subsequently, the server is attempting to send data to the client but the connection is closed. That is what results in a RST packet being sent.
RST means (roughly) "there is no active connection available to receive the data you're sending; it's pointless to send more." And so the timing of that RST is likely based on when the server next attempts to send to the client, not on any kernel / OS configuration setting. If the server doesn't attempt to send and it doesn't close, the connection should just sit there idle forever, and no RST will be sent


Setting SO_LINGER with a positive timeout does exactly one thing. It enables close() to block for up to that timeout while there is any outbound pending data still in flight. If you don't modify it, the default is for the close() to be asynchronous, which means the application can't tell whether any data still in flight got sent.


When a TCP connection is closed cleanly, the end that initiated the close ("active close") ends up with the connection sitting in TIME_WAIT for 60 seconds. So if your protocol is one where the server initiates the connection close, and involves very large numbers of short-lived connections, then it might be susceptible to this problem. An application level protocol sitting on top of TCP should be designed in such a way that the client always initiates the connection close. That way, the TIME_WAIT will sit at the client doing no harm. Remember as it says in "UNIX Network Programming" third edition (Stevens et al) page 203: "The TIME_WAIT state is your friend and is there to help us. Instead of trying to avoid the state, we should understand it

Note that it is very unlikely that delayed segments will cause problems like this. Firstly the address and port of each end point needs to be the same; which is normally unlikely as the client's port is usually selected for you by the operating system from the ephemeral port range and thus changes between connections. Secondly, the sequence numbers for the delayed segments need to be valid in the new connection which is also unlikely. However, should both of these things occur then TIME_WAIT will prevent the new connection's data from being corrupted.


The server will silently drop the packet since it already has a connection in the ESTABLISHED state, one of the four values from (client-ip, src-port, server-ip, dest-port) must be different for the new SYN to be accepted

-------
TCP session
------
The only way to identify a TCP connection is by a unique 4-tuple (client-ip, src-port, server-ip, dest-port)


--------
TCP connection establish
---------
The ACK is accompanied by sequence numbers. The RFC is pretty clear on the handshake:

1) A --> B SYN my sequence number is X
2) A <-- B ACK your sequence number is X
3) A <-- B SYN my sequence number is Y (the moment A gets it, it is considered complete)
4) A --> B ACK your sequence number is Y (the moment B gets it it is complete as well)
Steps 2 and 3 are combined in a single packet.

Reasons for sequence numbers is SYN to be randomized:
       - Security: anything too predictable is likely to be used for spoofing purposes
       - Multi-session interference. If all sessions started their sequence numbers at 1, then it would be much easier to end up in situations where you mix up packets from various sessions between two hosts (though there are other measures in place to avoid this, like randomizing the source port).

---> another view:
During the TCP three-way handshake process, the Linux kernel maintains two queues, namely:

SYN Queue
Accept Queue
Normal TCP three-way handshake process:

The Client side sends SYN to the Server side to initiate a connection. The Client side enters the SYN_SENT state.
After the Server side receives the SYN request from the Client side, the Server side enters the SYN_RECV state. At this time, the kernel will store the connection in the SYN queue and reply to the Client side with SYN+ACK.
After the Client side receives the SYN + ACK from the server side, the Client replies and enters the ESTABLISHED state.
After the Server side receives the ACK from the Client side, the kernel removes the connection from the SYN queue and adds it to the accept queue. The Server side enters the ESTABLISHED state.
When the Server side application calls the accept function, the connection is taken out of the accept queue.
Both the SYN queue and the Accept queue have length and size limits. If the limit is exceeded, the kernel discards the connection Drop or returns the RST packet.
s


ss
You can view the information of a fully connected queue by ss.

# -n Does not resolve the service name
# -t only show tcp sockets
# -l Displays LISTEN-state sockets

$ ss -lnt
State      Recv-Q Send-Q    Local Address:Port         Peer Address:Port
LISTEN     0      128       [::]:2380                  [::]:*
LISTEN     0      128       [::]:80                    [::]:*
LISTEN     0      128       [::]:8080                  [::]:*
LISTEN     0      128       [::]:8090                  [::]:*

$ ss -nt
State      Recv-Q Send-Q    Local Address:Port         Peer Address:Port
ESTAB      0      0         [::ffff:33.9.95.134]:80                   [::ffff:33.51.103.59]:47452
ESTAB      0      536       [::ffff:33.9.95.134]:80                  [::ffff:33.43.108.144]:37656
ESTAB      0      0         [::ffff:33.9.95.134]:80                   [::ffff:33.51.103.59]:38130
ESTAB      0      536       [::ffff:33.9.95.134]:80                   [::ffff:33.51.103.59]:38280
ESTAB      0      0         [::ffff:33.9.95.134]:80                   [::ffff:33.51.103.59]:38204
For sockets in LISTEN states

Recv-Q: The size of the current accept queue, which means the three connections have been completed and are waiting for the application accept() TCP connections.
Send-Q: the maximum length of the accept queue, which is the size of the accept queue.
For sockets in non-LISTEN state

Recv-Q: the number of bytes received but not read by the application.
Send-Q: the number of bytes sent but not acknowledged.
---------------------
The normal TCP termination sequence looks like this (simplified):
-------------

We have two peers: A and B

A calls close()
A sends FIN to B
A goes into FIN_WAIT_1 state
B receives FIN
B sends ACK to A
B goes into CLOSE_WAIT state
A receives ACK
A goes into FIN_WAIT_2 state
B calls close()
B sends FIN to A
B goes into LAST_ACK state
A receives FIN
A sends ACK to B
A goes into TIME_WAIT state
B receives ACK
B goes to CLOSED state – i.e. is removed from the socket tables

FIN_WAIT timeouts are normally 60 seconds

CLOSE_WAIT - Indicates that the server has received the first FIN signal from the client and the connection is in the process of being closed. This means the socket is waiting for the application to execute close() . A socket can be in CLOSE_WAIT state indefinitely until the application closes

This is where the problem starts. The (127.0.0.1:5000, 127.0.0.1:some-port) socket is still in CLOSE_WAIT state, while (127.0.0.1:some-port, 127.0.0.1:5000) has been cleaned up and is ready to be reused. When this happens the result is a total mess. One part of the socket won't be able to advance from the SYN_SENT state, while the other part is stuck in CLOSE_WAIT. The SYN_SENT socket will eventually give up failing with ETIMEDOUT.
resource: https://blog.cloudflare.com/this-is-strictly-a-violation-of-the-tcp-specification/

--------------
"Address alreayd used error"
Setting SO_REUSEADDR on the client side doesn't help the server side unless it also sets SO_REUSEADDR. This  will NOT fix the issue of CLOSE_WAIT where if the the other side gets FIN and does not call close().

------------

--------
in client-server architecture it is always recommended that server closes the connection and not client
(here is some recommendation from websocket RFC)

---

   To _Close the WebSocket Connection_, an endpoint closes the
   underlying TCP connection.  An endpoint SHOULD use a method that
   cleanly closes the TCP connection, as well as the TLS session, if
   applicable, discarding any trailing bytes that may have been
   received.  An endpoint MAY close the connection via any means
   available when necessary, such as when under attack.

   The underlying TCP connection, in most normal cases, SHOULD be closed
   first by the server, so that it holds the TIME_WAIT state and not the
   client (as this would prevent it from re-opening the connection for 2
   maximum segment lifetimes (2MSL), while there is no corresponding
   server impact as a TIME_WAIT connection is immediately reopened upon
   a new SYN with a higher seq number) <----- THIS IS WRONG ---------->
  (https://stackoverflow.com/questions/76932971/syn-flag-and-sequence-number-websocket-rfc-assumes-it-is-increasing) 
  as sequence numbers in SYN are randomized
   .  In abnormal cases (such as not
   having received a TCP Close from the server after a reasonable amount
   of time) a client MAY initiate the TCP Close.  As such, when a server
   is instructed to _Close the WebSocket Connection_ it SHOULD initiate
   a TCP Close immediately, and when a client is instructed to do the
   same, it SHOULD wait for a TCP Close from the server.
   

   As an example of how to obtain a clean closure in C using Berkeley
   sockets, one would call shutdown() with SHUT_WR on the socket, call
   recv() until obtaining a return value of 0 indicating that the peer
   has also performed an orderly shutdown, and finally call close() on
   the socket


------------
how to properly orderly shutdown on client and server!
-------
  When a stream socket peer has performed an orderly shutdown, the
   return value will be 0 (the traditional "end-of-file" return).
What the manpage means here is, orderly shutdown is done by one end (A) choosing to call shutdown(SHUT_WR), which causes a FIN packet to be sent to the peer (B), and this packet takes the form of a 0 return code from recv inside B. (Note: the FIN packet, being an implementation aspect, is not mentioned by the manpage). The "EOF" as the manpage calls it, means there will be no more transmission from A to B, but application B can, and should continue to send what it was in the process of sending, and even send some more, potentially (A is still receiving). When that sending is done (shortly), B should itself call shutdown(SHUT_WR) to close the other half of the duplex. Now app A receives EOF and all transmission has ceased. The two apps are OK to call shutdown(SHUT_RD) to close their sockets for reading and then close() to free system resources associated with the socket (TODO I haven't found clear documentation taht says the 2 calls to shutdown(SHUT_RD) are sending the ACKs in the termination sequence FIN --> ACK, FIN --> ACK, but this seems logical
----



SO_SNDTIMEOUT and SO_REDVTIMEOUT could be used for blocking sockeets to control timeouts on send and recv! if you doing blocking UDP then EGAIN error will indicate timeout!

in linux closing socket descriptor in another thread has not effect on select

it is not good to receive on the same socket from multiple threads as this will cause lock contention over a socket buffe, but recent kernel feature called SO_REUSEPORT can allow multiple processes /threads to share the load
by having dedicated UDP buffers for each!



----
accept
---
--------
RENTR
----
The code which calls write (or other blocking operations) has to be aware of EINTR. If a signal occurs during a blocking operation, then the operation will either (a) return partial completion, or (b) return failure, do nothing, and set errno to EINTR.
int total = 0;
while(size > 0) {
    int written = write(filedes, buf, size);
    if (written == -1) {
        if (errno == EINTR) continue;
        return (total == 0) ? -1 : total;
    }
    buf += written;
    total += written;
    size -= written;
}
return total; // bytes written

---
close
---
It is probably unwise to close file descriptors while they may be
       in use by system calls in other threads in the same process.
       Since a file descriptor may be reused, there are some obscure
       race conditions that may cause unintended side effects.

On Linux (and possibly some other systems), the behavior is
       different.  the blocking I/O system call holds a reference to the
       underlying open file description, and this reference keeps the
       description open until the I/O system call completes.  (See
       open(2) for a discussion of open file descriptions.)  Thus, the
       blocking system call in the first thread may successfully
       complete after the close() in the second threa

       Retrying the close() after a failure return is the wrong thing to
       do, since this may cause a reused file descriptor from another
       thread to be closed

              A careful programmer will check the return value of close(),
       since it is quite possible that errors on a previous write(2)
       operation are reported only on the final close() that releases
       the open file description. 

              A careful programmer who wants to know about I/O errors may
       precede close() with a call to fsync(2).

       The EINTR error is a somewhat special case.  Regarding the EINTR
       error, POSIX.1-2008 says:

              If close() is interrupted by a signal that is to be
              caught, it shall return -1 with errno set to EINTR and the
              state of fildes is unspecified.

              As Allbery said, the POSIX EINTR semantics are not really possible on Linux. The file descriptor passed to close() is de-allocated early in the processing of the system call and the same descriptor could already have been handed out to another thread by the time close() returns. The Linux behavior could be changed if there were a sufficiently good reason to do so, but, so far, that reason has been elusive.

So the POSIX-suggested handling of an EINTR, which is to retry the close(), could actually be quite dangerous on Linux. For that reason, Mark Mentovai suggested a change to the glibc manual to avoid recommending retrying close() on Linux.

-------------
Kerbores
---------------
User Client-based Login
A user enters a username and password on the client machine(s). Other credential mechanisms like pkinit (RFC 4556) allow for the use of public keys in place of a password.
The client transforms the password into the key of a symmetric cipher. This either uses the built-in key scheduling, or a one-way hash, depending on the cipher-suite used.
Client Authentication
The client sends a cleartext message of the user ID to the AS (Authentication Server) requesting services on behalf of the user. (Note: Neither the secret key nor the password is sent to the AS.)
The AS checks to see if the client is in its database. If it is, the AS generates the secret key by hashing the password of the user found at the database (e.g., Active Directory in Windows Server) and sends back the following two messages to the client:
Message A: Client/TGS Session Key encrypted using the secret key of the client/user.
Message B: Ticket-Granting-Ticket (TGT, which includes the client ID, client network address, ticket validity period, and the client/TGS session key) encrypted using the secret key of the TGS.
Once the client receives messages A and B, it attempts to decrypt message A with the secret key generated from the password entered by the user. If the user entered password does not match the password in the AS database, the client's secret key will be different and thus unable to decrypt message A. With a valid password and secret key the client decrypts message A to obtain the Client/TGS Session Key. This session key is used for further communications with the TGS. (Note: The client cannot decrypt Message B, as it is encrypted using TGS's secret key.) At this point, the client has enough information to authenticate itself to the TGS.
Client Service Authorization
When requesting services, the client sends the following messages to the TGS:
Message C: Composed of the TGT from message B and the ID of the requested service.
Message D: Authenticator (which is composed of the client ID and the timestamp), encrypted using the Client/TGS Session Key.
Upon receiving messages C and D, the TGS retrieves message B out of message C. It decrypts message B using the TGS secret key. This gives it the "client/TGS session key". Using this key, the TGS decrypts message D (Authenticator) and compare client ID from message C and D, if they match server sends the following two messages to the client:
Message E: Client-to-server ticket (which includes the client ID, client network address, validity period and Client/Server Session Key) encrypted using the service's secret key.
Message F: Client/Server Session Key encrypted with the Client/TGS Session Key.
Client Service Request
Upon receiving messages E and F from TGS, the client has enough information to authenticate itself to the Service Server (SS). The client connects to the SS and sends the following two messages:
Message E: from the previous step (the client-to-server ticket, encrypted using service's secret key).
Message G: a new Authenticator, which includes the client ID, timestamp and is encrypted using Client/Server Session Key.
The SS decrypts the ticket (message E) using its own secret key to retrieve the Client/Server Session Key. Using the sessions key, SS decrypts the Authenticator and compares client ID from messages E and G, if they match server sends the following message to the client to confirm its true identity and willingness to serve the client:
Message H: the timestamp found in client's Authenticator (plus 1 in version 4, but not necessary in version 5[6][7]), encrypted using the Client/Server Session Key.
The client decrypts the confirmation (message H) using the Client/Server Session Key and checks whether the timestamp is correct. If so, then the client can trust the server and can start issuing service requests to the server.
The server provides the requested services to the client.

-