
------------------------
CPU common optimizations
------------------------

- A second related hardware optimization is cache prefetching, in which the hardware
reacts to consecutive accesses by prefetching subsequent cachelines, thereby evading
speed-of-light delays for these subsequent cachelines. Of course, the hardware must use
simple heuristics to determine when to prefetch, and these heuristics can be fooled by
the complex data-access patterns in many applications. Fortunately, some CPU families
allow for this by providing special prefetch instructions. Unfortunately, the effectiveness
of these instructions in the general case is subject to some dispute

- A third hardware optimization is the store buffer, which allows a string of store
instructions to execute quickly even when the stores are to non-consecutive addresses
and when none of the needed cachelines are present in the CPU’s cache. The dark side
of this optimization is memory misordering

- A fourth hardware optimization is speculative execution, which can allow the hardware
to make good use of the store buffers without resulting in memory misordering. The
dark side of this optimization can be energy inefficiency and lowered performance if the
speculative execution goes awry and must be rolled back and retried

-A final hardware optimization is read-mostly replication, in which data that is
frequently read but rarely updated is present in all CPUs’ caches. This optimization
allows the read-mostly data to be accessed exceedingly efficientl

--------------

The processor always (even today) starts in real mode. To enter protected mode one has to create GDT and set up gdtr;

------------------------------------------------
in reality, registers rax, eax, and all others do not exist as fixed physical entities.

The part of a CPU called instruction decoder is constantly translating commands from an older CISC system to a more convenient RISC one. Pipelines allow for a simultaneous execution of up to six smaller instructions. To achieve that, however, the notion of registers should be virtualized. During microcode execution, the decoder chooses an available register from a large bank of physical registers. As soon as the bigger instruction ends, the effects become visible to programmer: the value of some physical registers may be copied to those, currently assigned to be, let’s say, rax

------
Skylake
---
Instruction Decoder Queue can hold up to 64 micor ops !
decode icache can supply 6 micro ops per cycle to IDQ and DecodePipeline can supply 5 micors ops to IDQ!

The L1 data cache can handle two 256-bit load and one 256-bit store operations each cycle. The unified L2 can service one cache line (64 bytes) each cycle. Additionally, there are 72 load buffers and 42 store buffers available to support micro-ops execution in-flight.

The microprocessor can rename any of these temporary registers to represent a logical register such as EAX.


My interpretation of the Haswell L1 Data Cache is that it is (for reads) a dual-ported cache with 64-Byte (512-bit) full-cache-line read ports.  The core can request up to 2 32-Byte (256-bit) reads and 1 32-Byte (256-bit) store per cycle.  The L1 Data Cache can service two reads per cycle for any size and any alignment -- as long as neither of the reads crosses a cache line boundary.  In other words, it looks like each of the two read ports of L1 Data Cache reads (up to) the full cache line containing the requested data and can return anywhere between 1 Byte (8-bits) and 32 Bytes (256-bits) for any alignment as long as all of the data requested is within that 512-bit cache line.   The cache can also service any two loads from the same cache line in one cycle (independent of alignment and/or overlap).   For loads that cross a cache line boundary, it looks like the L1 Data Cache must use both read ports -- one port to read the cache line containing the "lower" part of the requested data, and the other port to read the next cache line that contains the "upper" part of the requested data.  This limits throughput to one load per cycle (from the core's perspective).     It took a lot of experiments and a lot of thinking to figure out the read behavior of the cache.  Because of the potential for cycle skew between loads and stores, I don't know how to extend the methodology to understand how the L1 Data Cache handles stores.

A branch instruction is recognized as having loop behavior if it goes one way n-1 times and then goes the other way one time. A loop counter makes it possible to predict branches with  loop behavior perfectly if the period n is no longer than 64. The loop counters are stored for each branch without using the global history table. Instead, the loop counters have their own buffer with 128 entries. Therefore, the prediction of a loop does not depend on the number of other branches inside the loop. Nested loops are predicted perfectly.

Indirect jumps and indirect calls (but not returns) are predicted using the same two-level predictor principle as branch instructions. Branches without loop behavior and indirect jumps/calls share the same history buffer and history pattern table, but apparently not the same BTB. An indirect jump/call gets a new BTB entry every time it jumps to a new target. It can have more then four BTB entries even though the BTB has only four ways. The history buffer stores more than one bit for each entry, probably 6 or 7, in order to distinguish between more than two targets of an indirect jump/call. This makes it possible to predict a periodic jump pattern that switches between several different targets.

Apparently, there are two 18-bit global history buffers. The first history buffer includes all jumps and branches including unconditional jumps but not including never-taken branches. The second history buffer includes only some branches, presumably the most important ones. This improves the prediction of some branches inside loops that contain up to nine jumps.

note undconditional jumps also part of history table as they invert program flow!

The high throughput for taken branches of one per clock was observed for up to 128 branches with no more than one branch per 16 bytes of code. If there is more than one branch per 16 bytes of code then the throughput is reduced to one jump per two clock cycles. If there are more than 128 branches in the critical part of the code, and if they are spaced by at least 16 bytes, then apparently the first 128 branches have the high throughput and the remaining have the low throughput.
These observations may indicate that there are two branch prediction methods: a fast method tied to the μop cache and the instruction cache, and a slower method using a branch target buffer.

The processor is able to predict very long repetitive jump patterns with few or no mispredictions. I found no specific limit to the length of jump patterns that could be predicted. One study found that it stores a history of at least 29 branches. Loops are successfully predicted up to a count of 32 or a little more. Nested loops and branches inside loops are predicted reasonably well.


-------------
In the Pentium 4, Intel Xeon, and P6 family processors, the locking operation is handled with either a cache lock
or bus lock. If a memory access is cacheable and affects only a single cache line, a cache lock is invoked and
the system bus and the actual memory location in system memory are not locked during the operation. Here,
other Pentium 4, Intel Xeon, or P6 family processors on the bus write-back any modified data and invalidate
their caches as necessary to maintain system memory coherency. If the memory access is not cacheable
and/or it crosses a cache line boundary, the processor�s LOCK# signal is asserted and the processor does not
respond to requests for bus control during the locked operation

-------
for example a Haswell or Skylake core can do 2 loads and 1 store per cycle from/to L1d cache (See How can cache be that fast?). Obviously they can't have just one MBR. Instead, Haswell has 72 load-buffer entries and 42 store-buffer entries, which all together are part of the Memory Order Buffer which supports out-of-order execution of loads / stores while maintaining the illusion that only StoreLoad reordering happens / is visible to other cores

--------
To make speculative execution possible, the P6 processor microarchitecture decouples the
dispatch and execution of instructions from the commitment of results. The processor’s out-of-order execution
core uses data-flow analysis to execute all available instructions in the instruction pool and store the results in
temporary registers. The retirement unit then linearly searches the instruction pool for completed instructions
that no longer have data dependencies with other instructions or unresolved branch predictions. When
completed instructions are found, the retirement unit commits the results of these instructions to memory
and/or the IA-32 registers (the processor’s eight general-purpose registers and eight x87 FPU data registers)
in the order they were originally issued and retires the instructions from the instruction pool. 

-------

Reads are not reordered with other reads.
Writes are not reordered with older reads.
Writes to memory are not reordered with other writes, with the following exceptions:
writes executed with the CLFLUSH instruction;
streaming stores (writes) executed with the non-temporal move instructions (MOVNTI, MOVNTQ, MOVNTDQ, MOVNTPS, and MOVNTPD); and
string operations (see Section 8.2.4.1).
Reads may be reordered with older writes to different locations but not with older writes to the same location.
Reads or writes cannot be reordered with I/O instructions, locked instructions, or serializing instructions.
Reads cannot pass earlier LFENCE and MFENCE instructions.
Writes cannot pass earlier LFENCE, SFENCE, and MFENCE instructions.
LFENCE instructions cannot pass earlier reads.
SFENCE instructions cannot pass earlier writes.
MFENCE instructions cannot pass earlier reads or writes.
-------

For 64 set associative cache having 64 byte cacheline
[Tag Physical][Index][Offset]
[31........12][11...6][....]
|Fully associative|(every cache line can map any address!) caches have no index and the tag for the given physical address is being compared with each tag at a give cache to find the perfect match. Very difficult
and expensive to implement. Basically a given cache line can map to any physical address

|Direct mapped| (cache line maps a specific index only)! Very ineffcient if data is not uniformally distrbitued as too much data may be pointing to the same index!

[Set Associative]  Full associateive and Direct mapepd combined. basically the same 
index can now map to N-way tags!
-------

--------
L3 cache can be shared & sliced by APPLICATION!
Modern Intel (and AMD, I assume) L3 aka LLC aka last-level caches use an indexing function that isn't just 
a range of address bits.

On Slylake L3 cache it's no longer inclusive. Due to the non-inclusive nature of LLC, 
the absence of a cache line in LLC does not indicate that the line is not present in private caches of any of the cores
 Therefore, a snoop filter is used to keep track of the location of cache lines in the
 L1 or MLC of cores when it is not allocated in the LLC

For the Xeon Scalable Processors (Skylake Xeon), the L3 cache is no longer inclusive.   A cache line will be held in either a core (L1 or L2) or in the L3 cache, but not in both. (I don't know if Intel allows exceptions to this in the way that AMD has in the past with its "mostly-exclusive" L3 cache.)   In Xeon Scalable Processors, the L3 acts primarily as a victim cache for lines evicted from the L1 and L2 caches. The behavior is complex because there are dynamic prediction mechanisms in the L2 caches that decide whether a victim should be sent to L3 or to memory, and the heuristics controlling these predictors are not documented.  From my testing, it appears that if data is dirty, or remotely homed, or evicted due to a snoop filter eviction, it has a very high probability of being sent to the L3.  If data is clean and locally homed, the probability of being sent to the L3 appears to be based on the history of whether subsequent accesses to that address typically hit in the L3.   I have not attempted to determine what sorts of state are tracked by the hardware to make these decisions.

 It uses a mesh network instead of a ring bus to connect cores to each othe
---
Intel caches are physically tagged and virtually indexed
 L1D caches in most designs take their index bits from below the page offset, and thus are also VIPT allowing TLB lookup to happen
 in parallel with tag fetch, but without any aliasing problems. Thus, caches don't need to be flushed on context switches or anything.
------------
Aliasing: Multiple virtual addresses can map to a single physical address. Most processors guarantee that all updates to that single
 physical address will happen in program order. To deliver on that guarantee, the processor must ensure that only one copy of a physical 
address resides in the cache at any given time.
------------
for cache-able memory to work correctly with DMA on sepculative and prefetching hardware,
 DMA h as to be cache coherent. Particularly, since the effects and conditions for prefetching 
and speculative execution are largely left unsepcified, evicting data from the cache (for cache-able memory region) with explicit instructions is unreliable


----> IMPORTANT <-----
Intel Data Direct I/O can help with DMA to be coherent as it writes directly into caches! that is available on Xeon only
In general, Intel DDIO technology can enable NIC to do I/O operation directly to the CPU’s cache, and not to main memory as usually done
Integrated I/O is the new Intel Xeon processor E5 I/O architecture in which the
- PCI Express interface is integrated onto the processor itself rather than an I/O
hub or south bridge. The Intel Xeon processor E5 Integrated I/O implements the
- PCI Express 3.0 specification and also incorporates Intel DDIO
- is important to note that an I/O data consumption operation that does not find the data in
cache, sources it from memory; this operation does not cause data to be allocated into the
caching hierarchy (unlike the case of data delivery operations described below



Figure 3a shows a data delivery or write operation without Intel DDIO technology:
    Step 1: Data delivery operations have the NIC transferring data (packets or control)
    to host memory. If the data being delivered happens to be in the CPU’s caching
    hierarchy, it is invalidated.
    Step 2: SW running on the CPU reads the data to perform processing tasks. These
    data access operations misses cache (See step 1 above for explanation) and causes
    data to be read in from memory, into the CPU’s caching hierarchy.
Figure 3b illustrates the same set of operations on a platform with Intel DDIO technology.
    Step 1: I/O data delivery uses Write Update or Write Allocate operations (depending
    on cache residency of the memory addresses to which data is being delivered), which
    causes data to be delivered to the cache, without going to memory.
    Step 2: The subsequent read operations initiated by SW are satisfied by data from
    the cache, without causing cache misses.
    
Intel DDIO technology enables these data delivery operations to be accomplished with as
few trips (including none) to memory as possible with two modes of operation:
1. Write Update, which causes an in-place update in the cache. If the memory
addresses for the data being delivered already exists in the CPU’s caching
hierarchy, then no trips to memory are needed.
2. Write Allocate, which causes allocation in the last-level cache of the CPU’s
caching hierarchy. If the memory addresses for the data being delivered does
not exist in the CPU’s caching hierarchy, then no trips to memory are needed.
Write Allocate operations are restricted to 10% of the last-level cache; this is a trade-off to
prevent cache pollution from data streams that are not consumed by threads running on
CPUs or for scenarios where the incoming rate is much faster than the thread is capable of
handling. This is NOT a dedicated/reserved cache for I/O (or Intel DDIO). It is fully available
to applications running on the CPU. This is fixed and cannot be changed.
Thus, I/O device data delivery operations with Intel DDIO technology are achieved with
fewer trips to memory, and in the ideal case, without any trips to memory. Also, from a CPU
caching perspective, the data in cache is not disturbed by virtue of an I/O data delivery
operation, which creates opportunities for intelligent HW/SW design optimizations


PCIe feature (exposed in the Linux kernel apparently) that turns on cache snooping for a DMA request
Each read operation triggered by the NIC on its PCIe interconnect
(for example) causes data to be forwarded from the cache, if present;
however, it also causes the data to be evicted from cache, that is, the act of
reading by an I/O device causes data to be evicted


IO hub connects to the caches, not to memory, via the QPI.
I/O sees the data as the caches see it
cache snooping is by default turned on in the PCI(e) spec

----------------------
Intel uses MESIF protocol
----------------
*** Replacement policies *** To make room for the new entry on a cache miss, the cache may have to evict one of the existing entries. 
The heuristic it uses to choose the entry to evict is called the replacement policy.
LRU
Adaptive (used in Intel)

*** Write policies *** 
In a |write-through| cache, every write to the cache causes a write to main memory
In a |write-back| or copy-back cache, writes are not immediately mirrored to the main memory, and the cache instead
 tracks which locations have been written over, marking them as dirty
In a |Write combining| memory stores are combined by using temoral stores and then commited
in a burst!

---
L3 cache (not MESIF )
---
A physical tag, which is used to identify a particular cache line in the physical address space. On an incoming request to an L3 slice, the physical tag is compared against the physical tags of all of the cache entries in the cache set. There can be at most one match. The target line is present if a matching tag is found and the request is said to have hit in the cache. Otherwise, the request is said to have missed in the cache. Snoops may only be sent on hits.
A coherence state, which can be either Invalid, Clean, or Modified. Only the tags of the entries that are not Invalid are considered for comparison. The Clean state indicates that the line in the L3 cache is the same as the one in main memory. The Modified state indicates that the line in the L3 cache needs to be written back to memory when evicted.
A vector of coherence directory bits, where the number of bits is equal to the number of physical cores. Each bit corresponds to one physical core. The value of each bit indicates whether the line may be present in the private caches of the corresponding physical core. However, the coherence states of the line in the private caches of a core are not available at the L3 cache. The directory bits are also called the cross-core snoop filter (XSNP filter) and the core valid bits.

 A minor complication arises if the line in the L1 Data Cache is dirty -- when it is eventually chosen as the victim by the L1 Data Cache, there will not be an entry for that line in the L2 cache.  An implementation might allocate an entry in the L2 cache for the dirty line, but this is complex.  It is much easier to simply send the L1 victim to the L3

-----------------------
On x86, all atomic loads generate a MOV. SequentiallyConsistent stores generate an XCHG, 
other stores generate a MOV. SequentiallyConsistent fences generate an MFENCE, 
other fences do not cause any code to be generated. cmpxchg uses the LOCK CMPXCHG instruction.
 atomicrmw xchg uses XCHG, atomicrmw add and atomicrmw sub use XADD, 
and all other atomicrmw operations generate
 a loop with LOCK CMPXCHG. Depending on the users of the result, some atomicrmw operations
 can be translated into operations like LOCK AND, but that does not work in general.
-------------------------------
store buffer draining
---------------------
Generation of an Exception and/or Interrupt
Execution of a serializing instruction (CPUID, IRET, and RSM are the only non-privileged serializing instructions)
Execution of an I/O instruction
Execution of a LOCK operation
Execution of the BINIT operation (an external reset operation using the BINIT pin)
Execution of an SFENCE instruction
Execution of an MFENCE instruction

# CPU cache

https://lwn.net/Articles/252125/

Nearly all processors have “virtually indexed, physically tagged” first level caches.

The key point is that the address bits which address the L1 cache are the same in the virtual and physical address.

As an example, consider a 32KB 8-way set associative cache with 64 byte cache lines. Suppose the chip has a 48 bit virtual address and a 40 bit physical address.

Address bits 5–0 address the bytes within a cacheline

Address bits 11–6 address the cache. Each of these 64 slots have 8 places that might contain the line you want. Each one contains a tag containing physical address bits 39–12

Because the smallest page size is 4K, address bits 11–0 are <the same> for the virtual and physical address.

The CPU accesses the L1 cache and the TLB in parallel. The output of the TLB is exactly physical address bits 39–12 and they are used to compare against the physical tags in the 8 ways addressed.

When an instruction modifies memory the processor still has to load a cache line first because no instruction modifies an entire cache line at once (exception to the rule: write-combining as explained in Section 6.1).
 The content of the cache line before the write operation therefore has to be loaded. It is not possible for a cache to hold partial cache lines. 
A cache line which has been written to and which has not been written back to main memory is said to be �dirty�. Once it is written the dirty flag is cleared

More sophisticated cache implementations allow another possibility to happen. If the cache line which another processor wants to read from or write to is currently marked dirty
 in the first processor's cache a different course of action is needed. In this case the main memory is out-of-date and the requesting processor must,
 instead, get the cache line content from the first processor


write-through cache implementation;
write-back cache implementation.
write-combining; and
uncacheable.

direct mapped cached
fully assoc-iative cache
set associative cache

------------------
Exclusive (E)
-----------
The cache line is present only in the current cache, but is clean - it matches main memory. It may be changed to the Shared state at any time, in response to a read request. Alternatively, it may be changed to the Modified state when writing to it.

--------------
Modified
-----
This cache has the only valid copy of the cache line, and has made changes to that copy.
-----
Forward
-----
The F state in the MESIF protocol is simply a way to choose one of the sharers of a clean cache line to respond to a read request for data using a direct cache-to-cache transfer instead of waiting for the data to come from the main memory. This optimization makes sense in architectures where the cache-to-cache latency is much smaller in comparison to the latency of accessing the main memory. A key point to be noted here is that, similar to the MESI protocol, when data is in the shared state (with one of the caches in the F state) the data is clean.

The F state is a specialized form of the S state, and indicates that a cache should act as a designated responder for any requests for the given line. The protocol ensures that, if any cache holds a line in the S state, at most one (other) cache holds it in the F state


In a system of caches employing the MESI protocol, a cache line request that is received by multiple caches holding a line in the S state will be serviced inefficiently. It may either be satisfied from (slow) main memory, or all the sharing caches could respond, bombarding the requestor with redundant responses. In a system of caches employing the MESIF protocol, a cache line request will be responded to only by the cache holding the line in the F state.[3] This allows the requestor to receive a copy at cache-to-cache speeds, while allowing the use of as few multicast packets as the network topology will allow.


Because a cache may unilaterally discard (invalidate) a line in the S or F states, it is possible that no cache has a copy in the F state, even though copies in the S state exist. In this case, a request for the line is satisfied (less efficiently, but still correctly) from main memory. To minimize the chance of the F line being discarded due to lack of interest, the most recent requestor of a line is assigned the F state; when a cache in the F state responds, it gives up the F state to the new cache

-----
Shared
-----

The protocol ensures that, if any cache holds a line in the S state, at most one (other) cache holds it in the F state.
Indicates that this cache line may be stored in other caches of the machine and is clean - it matches the main memory. The line may be discarded (changed to the Invalid state) at any time.
This cache has the only copy of the line, but the line is clean (unmodified).

IMPORTANT:!!!!!
While MOESI can quickly share dirty cache lines from cache, 
it cannot quickly share clean lines from cache. If a cache line is clean with respect to memory 
and in the shared state, then any snoop request to that cache line will be filled from memory,
 rather than a cache.

If a processor wishes to write to an Owned cache line, it must notify the other processors
 that are sharing that cache line. Depending on the implementation it may simply tell them to
 invalidate their copies (moving its own copy to the Modified state), 
or it may tell them to update their copies with the new contents 
(leaving its own copy in the Owned state)

!!!!!!!!!!!!!!!!!!
This line is one of several copies in the system. 
This cache does not have permission to modify the copy.
 Other processors in the system may hold copies of the data in the Shared state,
 as well. Unlike the MESI protocol, a shared cache line may be dirty with respect to memory; if it is, some cache has a copy in the Owned state, and that cache is responsible for eventually updating main memory. If no cache hold the line in the Owned state, the memory copy is up to date. The cache line may not be written, but may be changed to the Exclusive or Modified state after invalidating all shared copies. (If the cache line was Owned before, the invalidate response will indicate this, and the state will become Modified, so the obligation to eventually write the data back to memory is not forgotten.) It may also be discarded (changed to the Invalid state) at any time. Shared cache lines may not respond to a snoop request with data.
Invalid This block is not valid; it must be fetched to satisfy any attempted access.

-------------------------------
The LOCK prefix originally stood for bus lock, which refers to a special signal
issued at the front-side bus to suppress any attempt of memory transfer of other processors.
Today it also refers to so-called cache locks, which lock one or more cache lines using the
cache coherency protocol. Unlike bus locks cache locks allow concurrent locking of disjoint
cache lines by different cores

---------------


The L1 DCache can handle multiple outstanding cache misses and continue to service incoming stores
and load

------

The general rule for a two-level adaptive predictor with an n-bit history is that it can predict any repetitive sequence with any period if all n-bit sub-sequences are different