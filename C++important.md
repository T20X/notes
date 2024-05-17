# [16/05/24]

- concepts are evaluated to true / false
- requires clause returns return type which is substituted into another concept as the first template parameter!
- Signature C::* and R (C::*)(Args...) are both class member pointers!

# [4/01/24]
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

# [16/01/24]
- C++ forward iterators with value initialization can be used to compare against the same type with value initialization as if the iterator comes from an empty container!
- C++20 got new iterator concepts which STL container use together with legacy one. Few examples where they are diffrent:
   input iterators don't have comparison operators anymore
   forward iterators don't need to be derefrenced by value
- function call is an expression and parameters been operands
- destrucor for objects created in sub-expressions are called at the end of the full expression which contains the given sub-expression. it is not specified in what order they would get called though. 
- all implicitly created objects also belong to a given full expression 
- init declarotors and member initializers are been considered full expressions
- implicit calls made by an expression are also sub expressions . but not any explicit calls made by the called function
- default initializers for sub-expressins are also considered sub-expressions of a given full expression
- no arithmetic operations on std::byte, but you can call to_integer to convert byte into integer
- number conversions
    * note that integer conversions between integer types do preserve values, so it is kind of safe to conver usnged and signed integers as long as usage does not oveflow
    * char implicitly gets converted into int when used in arithmetic operations
    * when double used in arithmetic conversions than the whole expression would have double type 
    * otherwise, float would be used if it is been reffered to in the expression
    * otherwide if unsinged long has been mentioned , than it would be used
    * otherwise if long was mentioned, than it would be used
    * unsigned int otherwise,
    * int otherwise for anything else
- on windows long is 32 bit long, on Linux it is 64 bit long
- char can be singed char on unsinged char depending on the platform, the standard allows both as ascii range is from 0 to 127
- std::ranges::owning range does take incoming range by copy or move
- some filtering views do cache their first element so you cannot very safely modify it if run more than once
- std::views::all just tries to convert a given range into a view:
   * if its type is prvalue, it would be prvalue view itslef
   * ref_view if it is well formed
   * owning view otherwise
- some range algorith return either in / out iterators as result(remove,copy) , or actual value (min/max). if you pass rvalue and range is expected to return in/out iterators , than you'd just get special override, result of which you cannot use
- views return views!
- temporaries binded to const lvalue / rvalue ref extended only once, although it can be recursive through the composition of diffrent classes!