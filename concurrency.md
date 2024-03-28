
# Terms & notions

invariants - statements that are always true about data structures
race condition - anything that changes outcome the outcome based on relative order of operations
lock-free programming - a technique that allows to find ndivisible parts in data structures and ensure those indivisible parts are executes atomicly in such a way that invariants remain true
software transaction memory - a technique where shared data sync achieved by grouping load and stores into a group and then applying it - if something changed since the transaction was first attempted , then it is redone
data race - race condition in c++ which is causing undefined behaviour

## race condition
race condition can come from interface itself. for example top() and pop() methods in the stack. both neeed to execute atomically in a stack that is shared acorsss diffrent threads


## obstraction-free 

obstraction-free means that if the thread cannot aquicker resources due to them been locked it would still make forward progress instead of causing contention

# signals

you cannot have mutex in signal handlers for simple reason: if the current thread acuirqed mutex and generated signal in the same thread, than it would be a deadlock. Also if two signals come after another, it may 


POSIX.1 distinguishes the notions of signals that are directed to the process as a whole and signals that are directed to individual threads. According to POSIX.1, a process-directed signal (sent using kill(2), for example) should be handled by a single, arbitrarily selected thread within the process.

If you want a dedicated thread in your process to handle some signals, here is an example from pthread_sigmask(3) shows you how to do it:



For a signal handler that is not executed as a result of a call to the std​::​raise function, it is unspecified which thread of execution contains the signal handler invocation

If a signal handler is executed as a result of a call to the std​::​raise function, then the execution of the handler is sequenced after the invocation of the std​::​raise function and before its return.
[Note 7: When a signal is received for another reason, the execution of the signal handler is usually unsequenced with respect to the rest of the program. — end note]

# std::jthread

- allows to request stop at any time through std::stop_token mechanism
- auto joins when goes out of scope!