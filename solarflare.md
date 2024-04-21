1. The Solarflare SFN7000, SFN8000 and X2 series adapters support multicast
replication where received packets are replicated in hardware and delivered to
multiple receive queues


2. If a multicaststream is being accelerated by Onload, and another application that is
not using Onload subscribes to the same stream, then the second application will
not receive the associated datagrams. Therefore if multiple applications subscribe
to a particular multicast stream, either all or none should be run with Onload.


3. When multiple sockets join the same multicast group, received packets are
delivered to these sockets in the order that they joined the group.
When multiple sockets are created by different threads and all threads are spinning
on recv(), the thread which is able to receive first will also deliver the packets to
the other sockets.
If a thread ‘A’ is spinning on poll(), and another thread ‘B’, listening to the same
group, calls recv() but does not spin, ‘A’ will notice a received packet first and
deliver the packet to ‘B’ without an interrupt occurring


4. Solarflare cards have hardware total 196K buffers. The card runs at its own frequence of 1Ghz. The filterting unit than DMA's data into memory RAM as given by RX ring queue descriptors. Descirptios are just pointers for packets, and there is up to 4096 entires per queue. There are about 1023 RX ring queues. EV_FI implementation will need to populate those RX ring queues, then card itself will fetch RX ring queues to read descriptors.

## Onload

note that onload bypasses kernetl entirely and hence RSS and RFS are totally not necessary

## RSS

Solarflare adapters support Receive Side Scaling (RSS). RSS enables packet receive‐
processing to scale with the number of available CPU cores. RSS requires a platform
that supports MSI‐X interrupts. RSS is enabled by default.
When RSS is enabled the controller usesmultiple receive queuesto deliverincoming
packets. The receive queue selected for an incoming packet is chosen to ensure that
packets within a TCP stream are all sent to the same receive queue – this ensures
that packet‐ordering within each stream is maintained. Each receive queue has its
own dedicated MSI‐X interrupt which ideally should be tied to a dedicated CPU core.
This allows the receive side TCP processing to be distributed amongst the available
CPU cores, providing a considerable performance advantage over a conventional
adapter architecture in which allreceived packetsfor a given interface are processed
by just one CPU core. RSS can be restricted to only process receive queues on the
NUMA node local to the Solarflare adapter. To configure this the driver module
option rss_numa_local should be set to 1.
By default the driver enables RSS and configures one RSS Receive queue per CPU
core. The number of RSS Receive queues can be controlled via the driver module
parameter rss_cpus

## RFS

RFS will attempt to steer packets to the core where a receiving application is
running. This reduces the need to move data between processor caches and can
significantly reduce latency and jitter. Modern NUMA systems, in particular, can
benefitsubstantially from RFS where packets are delivered into memory local to the
receiving thread.
Unlike RSS which selects a CPU from a CPU affinity mask set by an administrator or
user, RFS will store the application's CPU core identifier when the application
process calls recvmsg() or sendmsg().
• A hash is calculated from a packet’s addresses or ports (2‐tuple or 4‐tuple) and
serves as the consistent hash for the flow associated with the packet.
• Each receive queue has an associated list of CPUs to which RFS may enqueue
the received packets for processing.
• For each received packet, an index into the CPU list is computed from the flow
hash modulo the size of the CPU list.
There are two types of RFS implementation; Soft RFS and Hardware (or Accelerated)
RFS.
Soft RFS is a software feature supported since Linux 2.6.35 that attempts to
schedule protocol processing of incoming packets on the same processor asthe user
thread that will consume the packets.
Accelerated RFS requires Linux kernel version 2.6.39 or later, with the Linux sfc
driver or Solarflare v3.2 network adapter driver.
RFS can dynamically change the allowed CPUs that can be assigned to a packet or
packetstream and thisintroducesthe possibility of out of order packets. To prevent
out of order data,two tables are created that hold state information used in the CPU
selection.
