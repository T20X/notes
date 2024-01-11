- stl containers store allocators by value, including when doing copying / moving
- concept requires clause expect expression type!(value category) { a } -> int& (int won't match)
- built-in operator+ expect lvalue / prvalue but return expression must be prvalue
- non-class prvalues technically don't occpy storage
- you should not be able to take the address of rvalue
- x = y, return type and expression type is int&
- prefer non-friend non-class operator overloads!
- value categories are compile time properies
- when you bind refrence to refrence-related type , the refrence shall be as convsertative aa the initializing type
- when copying standard container they call allocator_traits<Alloc>::select_on_container_copy_construction in order to tell allocator that it is been copies so that allocator may return the allocator it wants the container to use for copying by calling Alloc::select_on_container_copy_construction. Note that it would return allocator copy
- refrence is really const pointer, int * const p. in fact it is so implicit, that you are not allowed to define const (refrence), so this won't compile even int const& r
- const T p in template parameter deduction is pointless as p would be deduce as by value

- you can specialize std templates in std namespace IF AND ONLY IF it depedns on user-defined type!
- cas 16 byte is twice slower to cas 8 bytes
- now itel uses lower 57 bits in the newest CPUs
  
