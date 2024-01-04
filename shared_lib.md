# global variables

with default visibility global variables of shared lib would be shared with the host process. With hidden visibility, shared lib will have its own copy of the symbols & variables

constructors and destructors for static and global variables would run when the library is loaded / unloaded

# cxa_atexit

__cxa_atexit() is defined in the Itanium C++ ABI. The document explained the motivation behind this function:

The C++ Standard requires that destructors be called for global objects when a program exits in the opposite order of construction. Most implementations have handled this by calling the C library atexit routine to register the destructors. This is problematic because the 1999 C Standard only requires that the implementation support 32 registered functions, although most implementations support many more. More important, it does not deal at all with the ability in most implementations to remove [Dynamic Shared Objects] from a running program image by calling dlclose prior to program termination.

The API specified below is intended to provide standard-conforming treatment during normal program exit, which includes executing atexit-registered functions in the correct sequence relative to constructor-registered destructors, and reasonable treatment during early DSO unload (e.g. dlclose).

So:

__cxa_atexit() is not limited to 32 functions.
__cxa_atexit() will call the destructor of the static of a dynamic library when this dynamic library is unloaded before the program exits.

however linux seems to support a lot of slots for atexit and able to call it for shared libaries as well
