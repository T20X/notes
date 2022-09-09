1. operator delete
Weather implementations are using std::free is not specified
   it is unspecified wheather operator delete(void* ptr) or operator delete(void* ptr, size_t sz) is called, BUT ANY OF THEM MAY BE CALLED!
      from standart "
      f a function without a size parameter is defined, the program should also define the corresponding function with a size parameter. If a function with a size parameter is defined, the program shall also define the corresponding version without the size parameter
      "
      also 
      "A call to an operator delete with a size parameter may be changed to a call to the corresponding operator delete without a size parameter, without affecting memory allocation"



 -      The effect of indirecting through a pointer returned from a request for zero size is undefined
 - A global allocation function is only called as the result of a new expression, or called directly using the function call syntax, or called indirectly to allocate storage for a coroutine state ([dcl.fct.def.coroutine]), or called indirectly through calls to the functions in the C++ standard library.
[Note 2: In particular, a global allocation function is not called to allocate storage for objects with static storage duration, for objects or references with thread storage duration, for objects of type std​::​type_­info, or for an exception object. — end note]
-A deallocation function that is not a class member function shall belong to the global scope and not have a name with internal linkage
-If a deallocation function terminates by throwing an exception, the behavior is undefined. The value of the first argument supplied to a deallocation function may be a null pointer value; if so, and if the deallocation function is one supplied in the standard library, the call has no effec


2. operator new

Wheather implementations are using std::malloc is not specified

void* operator new(std::size_t size);
[[nodiscard]] void* operator new(std::size_t size, std::align_val_t alignment);
1
#
Effects: The allocation functions called by a new-expression ([expr.new]) to allocate size bytes of storage. The second form is called for a type with new-extended alignment, and the first form is called otherwise