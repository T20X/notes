
# global allocation function

- it is cannot allocate data for static objects, exceptions, thread-local objects
- courutine state on the other hand gets allocted by global allocation function

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

IMPORTANT !!!! The invocation of the allocation function is sequenced before the evaluations of expressions in the new-initializer. Initialization of the allocated object is sequenced before the value computation of the new-expression.

If any part of the object initialization described above63 terminates by throwing an exception and a suitable deallocation function can be found, the deallocation function is called to free the memory in which the object was being constructed, after which the exception continues to propagate in the context of the new-expression. If no unambiguous matching deallocation function can be found, propagating the exception does not cause the object's memory to be freed.
[Note 13: This is appropriate when the called allocation function does not allocate memory; otherwise, it is likely to result in a memory leak. — end note]


An implementation is allowed to omit a call to a replaceable global allocation function ([new.delete.single], [new.delete.array]). When it does so, the storage is instead provided by the implementation or provided by extending the allocation of another new-expression.


## reuse
When evalua ting a new-expression, storage is considered reused after it is returned from the allocation function, but before the evaluation of the new-initializer ([expr.new]).

```
[Example 1: 
struct S {
  int m;
};

void f() {
  S x{1};
  new(&x) S(x.m);   // undefined behavior
}
```

# delete

 *** IMPORTANT ***
The value of the operand of delete may be a null pointer value, a pointer to a non-array object created by a previous new-expression, or a pointer to a subobject representing a base class of such an object. If not, the behavior is undefined

  void *ptr = new int;
  delete ptr; // bad ! undefined behaviour as cannot call delete on pointer to void reliably ! no hard error by compiler!
  struct S;
  S *s;
  delete s; // bad ! undefined behaviour as type S is incomplete (only it has non-trivial destructor), no hard-error
            // by compiler!


If the object being deleted has incomplete class type at the point of deletion and the complete class has a non-trivial destructor or a deallocation function, the behavior is undefined.

[Note 2: A pointer to a const type can be the operand of a delete-expression; it is not necessary to cast away the constness ([expr.const.cast]) of the pointer expression before it is used as the operand of the delete-expression. — end note]



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

# allocator_traits

The minimal interface for a type conforming to the allocator requirements is that it have a value_type type, allocate and deallocate member functions, and equality comparison operators. The allocator_traits class template provides many other types and functions such as pointer, rebind, and construct. **Generic types that use allocators are required to access the allocator through std::allocator_traits** The latter requirement was intended to allow the allocator interface to be extended without necessarily changing every existing allocator

the idea is to keep std::allocator as lean as possible and than add any other extended functionality to std::allocator_traits! Since C++23 not specializatoin of std::allocator_traits is allowed anymore

https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2022/p2652r0.html
p2652r0

## std::allocator_traits<Alloc>::select_on_container_copy_construction

```
static constexpr Alloc select_on_container_copy_construction( const Alloc& a );
```

If possible, obtains the copy-constructed version of the allocator a, by calling a.select_on_container_copy_construction(). If the above is not possible (e.g. Alloc does not have the member function select_on_container_copy_construction()), then returns a, unmodified.

This function is called by the copy constructors of all standard library containers. It allows the allocator used by the constructor's argument to become aware that the container is being copied and modify state if necessary.

vector calls this for example

```
_Alloc_traits::_S_select_on_copy(__x._M_get_Tp_allocator()))
```

# allocator

An implementation is allowed to omit a call to a replaceable global allocation function ([new.delete.single], [new.delete.array]). When it does so, the storage is instead provided by the implementation or provided by extending the allocation of another new-expression.


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
   it only needs to support - 
   std::allocator::allocate -
   std::allocator::deallocate -
   std::allocator::value_type -
   constructor accepting allocator<U>&
   operator equal

IMPORTANT: std::allocator::construct is now removed in c++20 so if you want allocator_traits to call construct you must create your own allocator which has construct
   You can create your own allocator type A, which has a construct function. That function will be called by std::allocator_traits<A>::construct

```
template <class T> class custom_allocator {
public:
  typedef T value_type;

  custom_allocator() {}
  template <class U> custom_allocator(const custom_allocator<U> &u) {}

  T *allocate(std::size_t n) {
    return static_cast<T *>(
        ::operator new(sizeof(T) * n, std::align_val_t(alignof(T))));
  }
  void deallocate(T *obj, std::size_t n) {
    ::operator delete(static_cast<void *>(obj), std::align_val_t(alignof(T)));
  }

  friend bool operator==(const custom_allocator &a1,
                         const custom_allocator &a2) {
    return false;
  }
};
```
   


  The rest can be done by std::allocator_traits


  Move assignment: vector& operator=( vector&& other );
     If std::allocator_traits<allocator_type>::propagate_on_container_move_assignment() is true, the target allocator is replaced by a copy of the source allocator
     f propagate_on_container_move_assignment is false, then the allocator has specified that when the container is moved, the moved-to container should retain its previous allocator. It can't take other's allocator, because that would contradict the propagate flag

    Note that vectors stores allocator instances by copy!
    Allocators are supposed to have value semantics, which means the vector stores it by value (notice that get_allocator() returns by value). So the constructor can easily take the allocator by const reference and just copy it.
    In fact all allocators in std lib are stored by values!

## Why comparions operator is required for allocators ?

    [operator==(a1, a2)] returns true only if the storage allocated by the
        allocator a1 can be deallocated through a2.Establishes reflexive, symmetric,
    and transitive relationship.Does not throw exceptions.

    Although note that still most implemenation also look for is_always_equal member of the allocator in order to determine if the allocator can be reused !Though standart deprecates is_always_equal from C++ 23 in favour of operator==



# operator new

The invocation of the allocation function is sequenced before the evaluations of expressions in the new-initializer. Initialization of the allocated object is sequenced before the value computation of the new-expression.


Even though the non-allocating placement new (9,10) cannot be replaced, a function with the same signature may be defined at class scope as described above. In addition, global overloads that look like placement new but take a non-void pointer type as the second argument are allowed, so the code that wants to ensure that the true placement new is called (e.g. std::allocator::construct), must use ::new and also cast the pointer to void*.



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



using `std::allocator<T>::allocate` works by fiat; the
function is defined to return a pointer, not to the array, but to the
first element of the array

After calling allocate() and before construction of elements, pointer arithmetic of T* is well-defined within the allocated array, but the behavior is undefined if elements are accessed.


# ::operator delete

::operator delete does not call destrcutor because type info is lost
::operator [] new is meant to implement special array indexing strategy where allocated size won't be exactly  N * sizeof(T)
bad_alloc can also happen because of heap corruption



