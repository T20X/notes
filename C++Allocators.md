# dynamic memory

Objects can be created dynamically during program execution, using new-expressions ([expr.new]), and destroyed using delete-expressions ([expr.delete]). A C++ implementation provides access to, and management of, dynamic storage via the global allocation functions operator new and operator new[] and the global deallocation functions operator delete and operator delete[].

## new expression
[Note 1: The non-allocating forms described in [new.delete.placement] do not perform allocation or deallocation. — end note]

The new-expression attempts to create an object of the type-id or new-type-id ([dcl.name]) to which it is applied. The type of that object is the allocated type. This type shall be a complete object type ([basic.types.general]), but not an abstract class type ([class.abstract]) or array thereof ([intro.object]).


If the allocation function is a non-allocating form ([new.delete.placement]) that returns null, the behavior is undefined. Otherwise, if the allocation function returns null, initialization shall not be done, the deallocation function shall not be called, and the value of the new-expression shall be null.

note that placement new don't do allocation or dealocatiion but they still (imaginary) call allocation / deallocation functions


When a new-expression calls an allocation function and that allocation has not been extended, the new-expression passes the amount of space requested to the allocation function as the first argument of type std​::​size_t. That argument shall be no less than the size of the object being created; it may be greater than the size of the object being created only if the object is an array and the allocation function is not a non-allocating form ([new.delete.placement]). For arrays of char, unsigned char, and std​::​byte, the difference between the result of the new-expression and the address returned by the allocation function shall be an integral multiple of the strictest fundamental alignment requirement of any object type whose size is no greater than the size of the array being created.
[Note 9: Because allocation functions are assumed to return pointers to storage that is appropriately aligned for objects of any type with fundamental alignment, this constraint on array allocation overhead permits the common idiom of allocating character arrays into which objects of other types will later be placed. — end note



A new-expression that creates an object of type T initializes that object as follows:
(23.1) If the new-initializer is omitted, the object is default-initialized ([dcl.init]).
[Note 12: If no initialization is performed, the object has an indeterminate value. — end note]
(23.2) Otherwise, the new-initializer is interpreted according to the initialization rules of [dcl.init] for direct-initialization.
24

The invocation of the allocation function is sequenced before the evaluations of expressions in the new-initializer. Initialization of the allocated object is sequenced before the value computation of the new-expression.

# operator delete
Weather implementations are using std::free is not specified
   it is unspecified wheather operator delete(void* ptr) or operator delete(void* ptr, size_t sz) is called, BUT ANY OF THEM MAY BE CALLED!
      from standart " f a function without a size parameter is defined, the program should also define the corresponding function with a size parameter. If a function with a size parameter is defined, the program shall also define the corresponding version without the size parameter
      " also "A call to an operator delete with a size parameter may be changed to a call to the corresponding operator delete without a size parameter, without affecting memory allocation"



 - The effect of indirecting through a pointer returned from a request for zero size is undefined
 - A global allocation function is only called as the result of a new expression, or called directly using the function call syntax, or called indirectly to allocate storage for a coroutine state ([dcl.fct.def.coroutine]), or called indirectly through calls to the functions in the C++ standard library.
[Note 2: IMPORTANT ------------- > In particular, a global allocation function is not called to allocate storage for objects with static storage duration, for objects or references with thread storage duration, for objects of type std​::​type_­info, or for an exception object. — end note] <---------------------------

-A deallocation function that is not a class member function shall belong to the global scope and not have a name with internal linkage
-If a deallocation function terminates by throwing an exception, the behavior is undefined. The value of the first argument supplied to a deallocation function may be a null pointer value;
if so , and if the deallocation function is one supplied in the standard library, the call has no effec

# allocator

std::allocator<T>::allocate returns T[n] and starts the lifetime of the array but not objects themselves __allocator_base = __gnu_cxx::new_allocator<_Tp>;

std::allocator_traits<std::allocator>::allocate calls std:;
allocator<T>::allocate which in turn would could some form of ::operator new(
    new / ext /
    new_allocator.h does that) 

allocator_traits<std::allocator>::deallocate
calls std::allocator<T>::deallocate
allocator_traits<std::allocator>::construct calls std::construct_at
allocator_traits<std::allocator>::destroy calls std::destroy_at

    **** vector internals *********

gcc using in vector _gnu_cxx::_allocator_traits which is based on
    std::allocator_traits it is using alloc traits from gcc to allocate
    / deallocae its internal storage

    which by default is
    using std::allocator __allocator_base = __gnu_cxx::new_allocator<_Tp>;

 Since C++ 20 std::allocator requirenments got relaxes.Now,
   it only needs to support - std::allocator::allocate -
   std::allocator::deallocate - std::allocator::value_type -
   std::allocator::size_type - std::allocator::difference_type -


  The rest can be done by std::allocator_traits

7. Why comparions operator is required for allocators ?

    [operator==(a1, a2)] returns true only if the storage allocated by the
        allocator a1 can be deallocated through a2.Establishes reflexive, symmetric,
    and transitive relationship.Does not throw exceptions.

    Although note that still most implemenation also look for is_always_equal member of the allocator in order to determine if the allocator can be reused !Though standart deprecates is_always_equal from C++ 23 in favour of operator==

8.
An implementation is allowed to omit a call to a replaceable global allocation function ([new.delete.single], [new.delete.array]). When it does so, the storage is instead provided by the implementation or provided by extending the allocation of another new-expression.




# operator new []
This overhead may be applied in all array new-expressions, including those referencing the library function operator new[](std​::​size_­t, void*) and other placement allocation functions. The amount of overhead may vary from one invocation of new to another.


inline
void*
operator new[](std::size_t n, void* p, std::size_t limit)
{
    if (n <= limit)
        std::cout << "life is good\n";
    else
        throw std::bad_alloc();
    return p;
}

int main()
{
    alignas(std::string) char buffer[100];
    std::string* p = new(buffer, sizeof(buffer)) std::string[3];
}


10. The invocation of the allocation function is sequenced before the evaluations of expressions in the new-initializer. Initialization of the allocated object is sequenced before the value computation of the new-expression.


11. 

using `std::allocator<T>::allocate` works by fiat; the
function is defined to return a pointer, not to the array, but to the
first element of the array

After calling allocate() and before construction of elements, pointer arithmetic of T* is well-defined within the allocated array, but the behavior is undefined if elements are accessed.


12. 

::operator delete does not call destrcutor because type info is lost
::operator [] new is meant to implement special array indexing strategy where allocated size won't be exactly  N * sizeof(T)
bad_alloc can also happen because of heap corruption


13.  *** IMPORTANT ***
The value of the operand of delete may be a null pointer value, a pointer to a non-array object created by a previous new-expression, or a pointer to a subobject representing a base class of such an object. If not, the behavior is undefined

  void *ptr = new int;
  delete ptr; // bad ! undefined behaviour as cannot call delete on pointer to void reliably ! no hard error by compiler!
  struct S;
  S *s;
  delete s; // bad ! undefined behaviour as type S is incomplete, no hard-error
            // by compiler!

