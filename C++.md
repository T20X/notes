﻿
# Virtual functions, vptr, table

## vptr and table

Each class gets VPTR created for it. Note that when object is initially created its own VPTR is set rather than the one of the most derived. note that an extra vptr for any class which is deviated in the hiaricy is created as it would not be able to re-use the very first vptr since itis members may be diffrent !

Extra data in VPTR:
- offset to virtual base classes if any present
- offset to the most derviced object start -to allow dynamic_cast conversions
- RTTI pointer if enabled!
![](/images/c++/2023-08-26-15-47-01.png)


```
Another example:
Vtable for C
C::_ZTV1C: 14 entries
0     (int (*)(...))0 //offset to most derived object start
8     (int (*)(...))(& _ZTI1C)
16    (int (*)(...))C::~C
24    (int (*)(...))C::~C
32    (int (*)(...))C::f1
40    (int (*)(...))A::f2
48    (int (*)(...))A::f12
56    (int (*)(...))C::f3
64    (int (*)(...))C::f6
72    (int (*)(...))-72 //offset to most derived object start, note that B's virtual table is embeded here!
80    (int (*)(...))(& _ZTI1C)
88    (int (*)(...))C::_ZThn72_N1C2f3Ev
96    (int (*)(...))B::f4
104   (int (*)(...))B::f5

Class C
   size=80 align=8
   base size=80 base align=8
C (0x0x7fd1cc514310) 0
    vptr=((& C::_ZTV1C) + 16)
A (0x0x7fd1cc4ff8a0) 0
      primary-for C (0x0x7fd1cc514310)
B (0x0x7fd1cc4ff900) 72 nearly-empty
      vptr=((& C::_ZTV1C) + 88)
```
## pointer adjustment 

can be done if suboject is at diffrent offset and does not require vptr. Also can be done when you got non-linear subobject like B in 
class C : public A, public B {

![](/images/c++/2023-08-26-15-49-41.png)


## thunk

a way to re-adjust this pointer for classes not linearly represented in the hiarechy. Note that even for destructor it would get generated too so that most derived class destructor gets called

![](/images/c++/2023-08-26-14-26-00.png)

## virtual base classes

virtual base classes can introduce another indirection if they contain data since normally they located at the end of object and their vptr would contain an offset to them which requires another indirectoin to access their data.

![](images/c++/2023-08-26-15-12-01.png)


![](images/c++/2023-08-26-15-12-55.png)



# smart pointers

## owner_less 

template <class T> struct owner_less; /* undefined */
(since C++ 11)(until C++ 17) template <class T = void>
struct owner_less; /* undefined */
(since C++ 17) template <class T> struct owner_less<std::shared_ptr<T>>;
(2)(since C++ 11) template <class T> struct owner_less<std::weak_ptr<T>>;
(3)(since C++ 11) template <> struct owner_less<void>;
(4)(since C++ 17) This function object provides owner -
 based(as opposed to value - based) mixed - type ordering of both std::weak_ptr and std::shared_ptr .The order is such that two smart pointers compare equivalent only if they are both empty or if they share ownership,
 even if the values of the raw pointers obtained by get() are different(
 e.g.because they point at different subobjects within the same object)

# Name lookup, ADL

inline namespaces dont hide symbols and also allow ADL to work !

ADL is been performed for unqualified calls (and for explicit templates since C++20).
ADL considers only function arguments, not template arguments
say f<A::X>() won't look at A::X for us because of template argument

From the set of argument types in the call, we break each type down further. Each argument type produces zero or more associated types and associated namespaces, via a complicated ad-hoc process.
 For the simplest cases, you can think of it as essentially “write down the name of the type as unambiguously as possible and then extract all the class-names and all the innermost namespace-names from that string

An argument of type SomeClass::NestedClassOrEnum gives us two associated types: SomeClass::NestedClassOrEnum itself, and the class SomeClass of which it is a member.
When ADL performs lookup in an associated class type, what it’s considering are the (namespace-scope) friends of that class. It won’t consider the member functions of that class — not even the static member functions

perhaps most importantly, ADL won’t kick in if the thing being called is not a function! That is, before we do ADL for a call to f, we’ll do an ordinary unqualified lookup of f, which means we’ll look in our current scope and all enclosing scopes. If this ordinary unqualified lookup finds something called f, and that f is not a function (or a function template), then we’ll just use that f; we won’t let ADL drag in any other namespaces. It’s only if we find a function (or function template) named f, or if we don’t find anything at all, that we’ll move on to argument-dependent lookup

ADL isnt a fallback mechanism to use when "normal" overload resolution fails, functions found by ADL are just as viable as functions found by normal lookup

ALD looks up argument type after all lookups have been done:"using",templates,extra namespace references etc


if ADL finds something which is not a function, ADL will stop completely

So in short, since unqualified lookup will always search in the global namespace, ADL will never apply to global namespace.


The friend functions that are found by ADL might have been declared in the namespace enclosing the associated type, or they might be declared nowhere else (the so-called “hidden friend” idiom, about which I hope to write more later). However, when the associated type declares a friend function using explicit namespace-qualification (as in friend void NS::f(int)), ADL will ignore that declaration. So even though it is technically possible to befriend functions from other namespaces, those functions will not thereby become ADL candidates


## Lookup details

name lookup in C++ works from innermost scope to outtermost scope. for example if you do this
void f()

void p()
{
  using other::f;
  f();
}

Then compiler wont even look at f() from above,
    but rather would execute other::f;


# auto

auto type deduction is usually the same as template type deduction, but auto type deduction assumes that a braced initializer represents a std::initializer_list, and template type deduction doesn’t

another diffrence is that with templates to have a non-type paramter you need to do this 

```
template< class T, T v >
struct integral_constant;
```

but not with auto

```
template<auto v >
struct integral_constant;
```

## structured binding
structured binding is just a way to introduce refrences to destructured elements it is pointing to
```
auto [a,b] = f();
```

it really does this under the hood auto _e_hidden_ = f();
```
auto& a = std::get<0>(_e_hidden_)
auto& b = std::get<1>(_e_hidden_)
```


# Lambdas 

## As function pointers
auto *ptr = +[](int, int) {} // possible, but it has to be stateless

# Intresting features 

-------------
you can make your ::operator new and ::operator delete private to your library by building with -fvisibility=hiddnen and explicitly marking the functions you do want to export with __attribute__((visibility("default")))
------------------------
std::isnorm - returns false if double is 0.0!
---------------------------
remove_if does not work for associative containers! nothing to move the end for them
---------------------
c++ to_string has 6 decimal point precision by default
------------------------
In ISO C and C++, you can freely use inline void f(void) {} in header files -- although for different reasons!

In ISO C, it does not provide an external definition at all. In ISO C++ it does provide an external definition;
however C++ has an additional rule(which C doesnt),
    that if there are multiple external definitions of an inline function,
    then the compiler sorts it out and picks one of them.

    extern inline void
    f(void);
in a.c file in ISO C is meant to be paired with the use of inline void f(void) {
}
in header files
    .It causes the external definition of the function to be emitted in that
        translation unit.If you dont do this then there is no external
            definition,
    and so you may get a link error(
            it is unspecified whether any particular call of f links to the
                external definition or
            not )
            .

        In other words,
    in ISO C you can manually select where the external definition goes;
or suppress external definition entirely by using static inline everywhere;
 but in ISO C++ the compiler chooses if and where an external definition would g


namespace-scope inline variables are no longer restricted to being const, and constexpr on a namespace-scope variable no longer implies inline. This was mildly controversial, but was adopted nonetheless.

    static inline function make sense because then if a function is declared just inline in more then one translation unit, then if it is called from another translation unit, it would violate ODR without diagnostic!

    Remember that static keyword has a different meaning when it comes for a class.
Edit: As you may know that static function inside a class doesnt have internal linkage, in other words a class cannot have different copies of its static method depending on the translation (.cpp) units.

    A.cpp
      inline int foo() {
   return 0;
 }
 B.cpp inline int foo() { return 1; }
 main.cpp extern int foo();
     ...
     foo() = ? could be randomly 0/1 depending on how compiler built it!

    Also static inline will make sure that no function is even generated!


An inline function or variable (since C++17) with external linkage (e.g. not declared static) has the following additional properties:
There may be more than one definition of an inline function or variable (since C++17) in the program as long as each definition appears in a different translation
 unit and (for non-static inline functions and variables (since C++17)) all definitions are identical. For example, an inline function or an inline variable (since C++17) may be defined in a header file that is #include'd in multiple source files.
It must be declared inline in every translation unit.
It has the same address in every translation unit.

If an inline function or variable (since C++17) with external linkage is defined differently in different translation units, the behavior is undefined

C++ does not allow types and pointers/referenes to objects or functions with internal linkage (static) to be used as template parameters. Replacing static with anonymous namespace still "hides" the things from other translation units
 but they keep external linkage (mostly) and can so be used as template arguments

inline marks the symbol WEAK, which hints linker to choose arbitary one of definition in object files
static marks the symbol LOCAL, which restricts the symbol in current translation unit, and the linker may keep multiple instances of (possibly different) definition.
static inline generally works as static, but the inline keyword suggest compiler trying to inline this function

    ------------------------
    it is possible to forward declare class inside namespeace but need to do liek below
    namespace test
    {
       class Strategy;
     }
    ------------------------
    An operator ∇ is left-associative if a ∇ b ∇ c = (a ∇ b) ∇ c, and right-associative if a ∇ b ∇ c = a ∇ (b ∇ c). Mathematically, addition and multiplication are both left- and right-associative—it doesn’t matter whether you start multiplying arguments from the left or from the right. But in C++, addition and multiplication are specified as left-associative so that the language can guarantee the order of evaluation for the expression.

This still doesn’t mean C++ guarantees the order of evaluation of the arguments in most cases;
    it just guarantees the order in which the operator will be applied to
        them.For example,
        in the expression a *b *c(where a, b, and c are nested expressions),
        you don’t know which one will be evaluated first,
        but you know exactly the order in which the multiplication will be
            performed
        : the product of a and b will be multiplied by
              c.a-- -- -- -- -- -- -- -- -- -- --std::fpclassify Categorizes
                  floating point value arg into the following categories : zero,
        subnormal, normal, infinite, NAN,
        or implementation -
                defined category.-- -- -- -- -- -- -- -- --SmartPtr &operator=(
                    SmartPtr &&other) {
      if (this == &other)
        return *this;
      DeletionPolicy::operator=(std::move(other));
      MovePolicy::operator=(std::move(other));
      p_ = other.p_;
      other.release();
      return *this;
    }
    -- -- -- -- -- -- -- -- -- -- -- -- -- -- --be carefull with modulo like -
        (%, mod) operations.

        - 3 % 5 = -3 - 3 % 3 = 0

        dont mix unsigned with singed with singed !int a;
    unsigned int b;
a % b - > undefined behaviour, correct thing to do is int a % (int)b
-------------------------
The lifetime extension works only once—the reference bound to a temporary object extends its lifetime. Another reference thats bound to the first one does nothing else and may be left dangling if the object is destroyed. Therefore, if the policy object needs to be stored as a data member of the smart pointer, it has to be copied

The behavior is undefined if more than one replacement is provided in the program for any of the replaceable allocation function, or if a replacement is defined with the inline specifier. The program is ill-formed if a replacement is defined in namespace other than global namespace, or if it is defined as a static non-member function at global scope.


class SmartPtr
{
  const Big &b_;
  SmartPtr(const Big& b = Big()):b_(b) // b_ is a dangling reference
  
---------------
Non-template functioon overloading rules summary
----------------
when considering non-member, non-template functions, the compiler will look for all functions with the given name (operator+, in our case), then check if they accept the right number of parameters (possibly considering default arguments), then check if, for each such function, for each of its parameters, a conversion from the supplied argument to the specified parameter type exists (the rules on which conversions, exactly, are considered, and again fairly complex, but lets say that both user-given implicit conversions and built-in conversions such as non-const to const are considered). If this process yields only one function, that function is called (otherwise the compiler either selects the best overload or complains that several candidates are equally possible and the call is ambiguous)

----------
Template function overloading rules
--------
For template functions, this process would again yield an almost unlimited number of candidates—every template function with the name operator+() would have to be instantiated on every known type just to check if enough type conversions are available to make it work. Instead, a much simpler process is attempted—in addition to all non-template functions described in the previous paragraph (in our case, none), the compiler also considers instantiations of template functions with the given name (again, operator+) and the types of all parameters equal to the types of the function arguments at the call site (so-called trivial conversions, like adding a const, are allowed)
----------------
template <typename T> class C
 {
    friend T;
    template <typename U>
    friend C<U> operator+(const C<U> &lhs, const C<U> &rhs) {
      return C<U>(lhs.x_ + rhs.x_);
    }
  };

  template <typename, typename> friend class ConversionPolicy;
  -- -- -- -- -- -- -- -- -- -- -- -- -- -- --void (isInteger<int>::*ptr)() =
      &isInteger<int>::require;
  isInteger<int> v2;
  (v2.*ptr)();
  -- -- --C++ 11 ONLY guarantees that llist(c++ 17),
      vector(c++ 17), forward_list,
      unique_ptr<incomplete_type> and shared_ptr<incomplete_type> will work,
      although there are strict limits.For example wherever
      ~unique_ptr() is executed,
      the type has to be complete there.But you can usually outline such code to
      a source and#include the complete type at that point

      -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --Call lambda as
      if member function pointer
      -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

      int first = 5;
  auto lambda = [=](int x, int z) { return x + z + first; };
  int (decltype(lambda)::*ptr)(int, int) const = &decltype(lambda)::operator();
  std::cout << "test = " << (lambda.*ptr)(2, 3) << std::endl;
  -- -- -- -- -- -- -- -- -- --int numerator, denominator;
  double r = (double)numerator / denominator;
  < -- --needs converstion to double otherwise int will be
           produced-- -- -- -- -- -- -- -- -- -- -- -- -- --It should be clear
               after reading the last section that this is an example of method
                   chaining,
      and a more general one than what we saw earlier—each method returns an
          object of the same type,
      but not the same object.The difference between chaining and
          cascading is very clear in this example—a cascade of methods would
              filter and
          sort the original collection(
              assuming we decided to support such operations)
              .-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --the
                  difference between these two kinds of initialization :

      T v1 = u; // copy-initialization
  T v2(u);      // direct-initialization
  A common -
      sense answer would be “the latter is more powerful because it additionally
          considers explicit constructors and conversion operators.” True,
      it considers more constructors,
      but to ‘consider’ an additional constructor
          does not necessarily mean ‘successfully use it construction’.

      -- -- -- -- -- -- -- -- -- --std::decay-- -- -- -- -- -- -Basically,
      its this,
      in pseudo - code
      : Let U = std::remove_reference<T>::type

          Choose one of the following : If U is an array,
            then std::decay<T> =
                std::remove_extent<U>::type * If U is a function,
            then std::decay<T> = std::add_pointer<U>::type Otherwise,
            std::decay<T> =
                std::remove_cv<U>::
                    type-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -direct -
                list - initialization T object{arg1, arg2, ...};

  elements have sequences side - effects - ony by one,
      not like function call parameters-- -- -- -- -- --Union-- -- -- --(
          1) if T is not trivial then inside union one must
              define default constructor and
          destructor union {
    T() {}
    ~T() {}
    T nonTrivial;
}
Note that compiler would mandsate const/dtor for union which contains  T, but it just mandates you to put them so that you are aware what are you doing! But note it wont automatically call the corresponding cont/dtro for T itself from ctro/dtor of the union!

Union can be default initialized with the help of initializer_list!
union Storage
{
  Storage() {}
  Storage(initializer_list<MyType> l) {}
  ~Storage() {}
  MyType t_;
};

Storagge s{MyType()};

(2) If members of a union are classes with user
    - defined constructors and destructors,
    to switch the active member,
    explicit destructor and placement new are generally needed : union S {
  std::string str;
  std::vector<int> vec;
  ~S() {
  } // needs to know which member is active, only possible in union-like class
};  // the whole union occupies max(sizeof(string), sizeof(vector<int>))

int main() {
  S s = {"Hello, world"};
  // at this point, reading from s.vec is undefined behavior
  std::cout << "s.str = " << s.str << '\n';
  s.str.~basic_string();
  new (&s.vec) std::vector<int>;

---- 

A variable declared inline has the same semantics as a function declared inline: it can be defined, identically, in multiple translation units, and in fact each trasnlation unit would still only have a unique definiton of it, and the behavior of the program is as if there was exactly one variable.
--------------
zero initialization does not mean litterly memset(0)! it could be in fact set to all bits 1!
An object of static storage duration is zero initialized
If T is a scalar type, the objects initial value is the integral constant zero explicitly converted to T
If T is array type, each element is zero-initialized.
If T is reference type, nothing is done.

----
vtable
-----

--
0offset to top of the object!
8RTTI std::type_info
16 ~ with delete after
24 ~ without delete after
32 func1
....
N offset to top of the object!
N+8 RTTI std::type_info
A,B, C : A,B
.....

[vptr A,C]  <---this for A and C
data A
[vptr B]   // B* b = new C;  <----- this for B!
data B
data C

                           +-----------------------+
                           |     0 (top_offset)    |
                           +-----------------------+
c --> +----------+         | ptr to typeinfo for C |
      |  vtable  |-------> +-----------------------+
      +----------+         |         A::v()        |
      |     a    |         +-----------------------+
      +----------+         |         C::w()        |
      |  vtable  |---+     +-----------------------+
      +----------+   |     |    -8 (top_offset)    |
      |     b    |   |     +-----------------------+
      +----------+   |     | ptr to typeinfo for C |
      |     c    |   +---> +-----------------------+
      +----------+         |    thunk to C::w()    |
                           +-----------------------+

Now, when w() is called on an instance of C through a pointer-to-B, the thunk is called. What does the thunk do? Lets disassemble it (here, with gdb):

0x0804860c <_ZThn8_N1C1wEv+0>:  addl   $0xfffffff8,0x4(%esp)
0x08048611 <_ZThn8_N1C1wEv+5>:  jmp    0x804853c <_ZN1C1wEv>

The entries for virtual destructors are actually pairs of entries. The first destructor, called the complete object destructor, performs the destruction without calling delete() on the object. The second destructor, called the deleting destructor, calls delete() after destroying the object. Both destroy any virtual bases; a separate, non-virtual function, called the base object destructor, performs destruction of the object but not its virtual base subobjects, and does not call delete().

The offset to top holds the displacement to the top of the object from the location within the object of the virtual table pointer that addresses this virtual table, as a  ptrdiff_t. It is always present. The offset provides a way to find the top of the object from any base subobject with a virtual table pointer. This is necessary for dynamic_cast<void*> in particular. So basically this offset is an offest insdie the top object! !!!!!!!!!! ---------- 

(1) Used by  dynamic_cast<void*>(b);(returns the most derived pointer! so no RTTI needed) for example. One last point, you might wonder what's offsetToTop doing in vtables. It's used when casting to void* so that you can simply substract the pointer by the offset to get the base of the most derived class ----------------
 
 NOW VIRTUAL BASE INHERITANCE WORKS THE SIMILAR WAY because at any any given base vtable the location of virtual base cannot be known for sure since earlier / later classes might have created it! That is why each vtable having virtual base classes has an offset to the virtual base object
 !!!!!!!!!!!!!!!!!!!!

dynamic_cast<void*> will return the pointer to the most dervided

The typeinfo pointer points to the typeinfo object used for RTTI. It is always present. All entries in each of the virtual tables for a given class must point to the same typeinfo object. A correct implementation of typeinfo equality is to check pointer equality, except for pointers (directly or indirectly) to incomplete types. The typeinfo pointer is a valid pointer for polymorphic classes, i.e. those with virtual functions, and is zero for non-polymorphic classes

destructors change the vtable pointer to point to the their own class’s rather than keep it pointing to the concrete class


SUGGESTION TO IMPROVE DYNAMIC CAST PERFORMANCE:
Our suggestion for an F function is integer modulo. To construct the type ID, a different prime
number is associated with each class. The type ID for that class will be that prime number times the
prime number for each of its base classes. Thus, the type ID for a class will be evenly divisible by the
type ID of any of its base classes, and only by its base classes. This is guaranteed by the uniqueness of
the factorization of integers into prime factors. Given an arbitrarily large integer type ID, any hierarchy
can be represented this way

----------------
to allow implicit conversions (if any are provided) on both the left-and right-hand side of the == sign, we have to declare operator== as a non-member function.
-----------------------
overloading is ad-hoc polymorphism
----------------
C++filt can be used to demangle names
abidiff can be used to compare two executable on ABI compatibility
---------------
in clang and gcc can hide symbols and make them private ... can make it by deafult as well

can also limit symbol visibility per translation unit
------
inline namespace changes mangled names!
-----------
For a default constructor and destructor being “trivial” means literally “do nothing at all”. For copy-constructor and copy-assignment operator, being “trivial” means literally “be equivalent to simple raw memory copying” (like copy with memcpy
--------------------
Simply, the closure type defined by the lambda expression will have a templated call operator rather than the regular, non-template call operator of C++11's lambdas (of course, when auto appears at least once in the parameter list).

So your example:

auto glambda = [] (auto a) {
                  return a; };
Will make glambda an instance of this type :

    class /* unnamed */
{
public:
  template <typename T> T operator()(T a) const { return a; }
};
-----------------------------
std::variant
- no dynamic memory allocation
- if initialized by value the first type is used
- not permitted to hold refrences

-------
- for function overload resolution with multiple paramters, the less conversion operations are required the better candidate is
- The more specific, narrower overload is considered a better match and is selected. This is a new notion, specific to templates—instead of choosing better conversions (in general, fewer or simpler conversions), we select the overload that is harder to instantiate. 
-Note that certain conversions are considered trivial and are included in the notion of the exact match, for example, the conversion from T to const T
-One important detail to note here is that conversions are not considered when deducing template types—the deduction of T as int would have yielded f(int, int*) for the second call, which is a viable option for calling f(long, int*) with the conversion of the first argument. However, this option is not considered at all, and instead, type deduction fails as ambiguous
-The type of the default argument does not matter—if it can be converted to the specified parameter type, then thats the value that is used
---------
We could do with some guidelines for when to consider type erasure. There are two types of issues to keep in mind—design considerations and performance considerations
---------------
providing default implementation for pure virtual function can be done, but derived class still has to provide an implementation regardless

---------------
a valid use case for static_cast
------------
Base* b = new Derived;    // *b is really DerivedDerived* d = b;
Derived *d = static_cast<Derived *>(b); // Explicit conversion

- Remember that the public interface is like a contract—we promise to the clients of the class that it supports certain operations, maintains some invariants, and obeys the specified restrictions
- public inheritance implies the is-a relationship
- that private inheritance implements a has-a relationship—the derived object has an instance of the base class contained inside of i

The programming technique where some code operates on any number of base objects and invokes the same methods, but the results depend on the actual type of these objects, is known as runtime polymorphism, and the objects that support this technique are polymorphic

-------------------

.  values are mental model(platonic type) of things which could then be mapped/approximated into C++.
    values are recursively defined in terms of operations that are possible on a given type.
We refer to stateful objects that do not represent a value as "Mechanisms" - say C++ allocators for example.

-values are interpretation of subset of instance state in terms of operations(values they return)
-values semantic objects state is only changed via objects interface!
-if a and b have the same value and then the same operations on the value interface have been called on them
then a and b values have to be the same again!
-(advice) if two objects have the same value then individual observeable attributes that contribute to the value also need to have the same value
(advice) when deciding if object has value semantics the question is really to how to compare two objects of this value type!

data members are for variation in values
virtual functions are for variations in behavior.
----------------------------------------------------------
- For an unqualified name, that is a name that does not appear to the right of a scope resolution operator ::
- A qualified name is a name that appears on the right hand side of the scope resolution operator :: (see also qualified identifiers). A qualified name may refer to a
      - class member (including static and non-static functions, types, templates, etc)
      - namespace member (including another namespace)
      - enumerator
If there is nothing on the left hand side of the ::, the lookup considers only declarations made in the global namespace scope (or introduced into the global namespace by a using declaration). This makes it possible to refer to such names even if they were hidden by a local declaration:

Because of argument-dependent lookup, non-member functions and non-member operators defined in the same namespace
 as a class are considered part of the public interface of that class (if they are found through ADL)

ADL is the reason behind the established idiom for swapping two objects in generic code:
using std::swap;
swap(obj1, obj2);
because calling std::swap(obj1, obj2) directly would not consider the user-defined swap() functions that could be defined in the same namespace as the types of obj1 or obj2, and just calling the unqualified swap(obj1, obj2) would call nothing if no user-defined overload was provided. In particular, std::iter_swap and all other standard library algorithms use this approach when dealing with Swappable types.

hidden friend - member function can be declared friend! it is not a memember function of a class, however it can access classes private members - it is defined outside class. However ADL can find it. Exactly for the type of input arguments.
----------------------------------------------------------
template <typename T>
void func(size_t sz, size_t pos)
{
  static constexpr std::align_val_t __al = std::align_val_t(alignof(T));
  T *data = static_cast<T *>(::operator new(
      sz * sizeof(T),
      __al)); // wrong way if T is not trivially default constructible
  // T* data = ::new(static_cast<void*>(buffer + pos*sizeof(T))) T(1, 2);
  // //correct way!
  data[pos] = T(1, 2); // is this legal? thechnically object at data[pos] is in
                       // some undetermenistic state.
}

This is valid only for scalar types (like numeric types or any pointer type), or class types (including unions) that have a "trivial" default constructor, or arrays of such types. If T is a class type with non-trivial default constructor or no default constructor, or an array of such class types, it is undefined behavior to call any member function on memory where no object was created, even if that member is a copy assignment operator

---------------
malloc reports failure as a nullpointer result, operator new reports failure via exception, (5) malloc is called directly by user code
If you want operator new without throwing exceptiosn then use this
void* operator new  ( std::size_t count, const std::nothrow_t& tag);
-- -- -- -- -- -template <class C> friend class FIFO;
----------------------------------
epsilon is the diffrence between 1.0 and the next representable floating point number
std::numeric_limits<T>::epsilon
Used for programmatic almost equal compuations

-------------------
The result of the expression static_­cast<T>(v) is the result of converting the expression v to type T. If T is an lvalue reference type or an rvalue reference to function type, the result is an lvalue;
if T
  is an rvalue reference to object type, the result is an xvalue;
otherwise,
    the result is a
        prvalue.The static_­cast
        operator shall not cast away constness
            .-- -- -- -- -- -- -- -- -- -- -- -- -
    The effect of any implicit conversion is the same as performing the
        corresponding declaration and initialization and
    then using the temporary variable as the result of the conversion.The result
        is an lvalue if T is an lvalue reference type or
    an rvalue reference to function type([dcl.ref]),
    an xvalue if T is an rvalue reference to object type,
    and a prvalue otherwise.The expression e is used as a glvalue if and
        only if the initialization uses it as a glvalue

            For class types,
    user - defined conversions are considered as well;
see[class.conv].In general,
    an implicit conversion sequence([over.best.ics])
            consists of a standard conversion sequence followed by a user -
        defined conversion followed by another standard conversion
            sequenc-- -- -- -- -- -- -- -- -- -- -The result of a glvalue is the
                entity denoted by the expression.The result of a prvalue is the
                    value that the expression stores into its context;
a prvalue that has type cv void has no result.A prvalue whose result is the
        value V is sometimes said to have or
    name the value V.The result object of a prvalue is the object initialized by
        the prvalue;
a non-discarded prvalue that is used to compute the value of an operand of a built-in operator or a prvalue that has type cv void has no result object. [ Note: Except when the prvalue is the operand of a decltype-specifier, a prvalue of class or array type always has a result object. For a discarded prvalue that has type other than cv void, a temporary object is materialized
------------------
A function call is an lvalue if the result type is an lvalue reference type or an rvalue reference to function type, an xvalue if the result type is an rvalue reference to object type, and a prvalue otherwis
------------------------------------
As far as I know, ABI requires GCC and Clang to delay the destruction of function parameters to the end of full expression
-----------------------
void foo(Bar value);
Bar bar;
foo(bar); // here bar is lvalue but get's converted to the prvalue with copy initialization

Once the preconditions are met the conversion is following:
For non-class types (class types: class, struct, union) the resulting type is cv unqualified. The resulting object is obtained by copy initialization
Otherwise (for class types) the resulting type preserves it’s const and volatile qualificatio

Such C++ feature, that actually does the object instantiation and casts prvalue type to the xvalue (that belongs to the glvalue group) is called temporary materialization conversion. 

---------------------------
A glvalue shall not have type cv void
-----------------
So typically, an lvalue-to-rvalue conversion will just read the value from an object. It isnt just a no-op conversion between value (expression) categories; it can even create a temporary by calling a copy constructor. And the lvalue-to-rvalue conversion always returns a prvalue value, not a (temporary) object.
================
Every expression belongs to exactly one of the fundamental classifications in this taxonomy: lvalue, xvalue, or prvalue. This property of an expression is called its value category
--------------------
A type cv void is an incomplete type that cannot be completed; such a type has an empty set of values
-----------------
The copies and moves of polymorphic classes cannot be trivial because then base objects may copy vtable from derived object - that would just break value semantics!
-----------------
extern int arr[]; //incomplete!
in arr[10]; now comlete!
--------------------------------------
The evaluation of a lambda-expression results in a prvalue  .result object is called the closure object
----------------------
Pre-C++17, prvalues are temporary objects. In C++17, a prvalue expression is merely something which can materialize a temporary, but it isnt a temporary yet

In C++17, there is a dual to this conversion, known as temporary materialization (but it could just as well have been called “prvalue-to-xvalue conversion”): Any time a prvalue validly appears where a glvalue (which includes the xvalue case) is expected, a temporary object is created and initialized with the prvalue (recall that prvalues are primarily “initializing values”), and the prvalue is replaced by an xvalue designating the temporar

With the keyword decltype (introduced in C++11), it is possible to check the value category of any C++ expression. For any expression x, decltype((x)) (note the double parentheses) yields:
• type if x is a prvalue
• type& if x is an lvalue
• type&& if x is an xvalue

Every expression belongs to exactly one of the fundamental classifications in this taxonomy: lvalue, xvalue, or prvalue. This property of an expression is called its value category

if constexpr (std::is_lvalue_reference<decltype((e))>::value)
 {  std::cout << "expression is lvalue\n";}
else if constexpr (std::is_rvalue_reference<decltype((e))>::value) 
{  std::cout << "expression is xvalue\n";}
else
 {  std::cout << "expression is prvalue\n";}
---------------------
Whenever an rvalue expression appears as an operand of an operator that expects a glvalue for that operand, the temporary materialization conversion ([conv.rval]) is applied to convert the expression to an xvalue.
----------------------------------
The difference between xvalue and prvalue is that xvalue has identity and prvalue does not

f(C&& other)
    C&& c2 = C();
    f(std::move(c2)); //c2 is still lvalue! and move is required
    But c2 still can be modified because c2 is lvalue !

n brief , a prvalue is now like a "delayed evaluation" expression; if the prvalue is passed across function calls and returns, there is not actually an object created until it reaches its final destination. This avoids the possibility of intermediate temporaries and the need for some copy elision cases; it enables you to have a function that creates a non-copyable non-movable object and returns it
------------------
•A glvalue is an expression whose evaluation determines the identity of an object, bit-field, or function (i.e., an entity that has storage).
• A prvalue is an expression whose evaluation initializes an object or a bit-field, or computes the value of the operand of an operator. Say temporaries
• An xvalue is a glvalue designating an object or bit-field whose resources can be reused (usually because it is about to “expire”—the “x” in xvalue originally came from “eXpiring value”).
• An lvalue is a glvalue that is not an xvalue.
• An rvalue is an expression that is either a prvalue or an xvalue.

It’s worth emphasizing that glvalues, prvalues, xvalues, and so on, are expressions, and not values2 or entities. For example, a variable is not an lvalue even though an expression denoting a variable is an lvalue

Every expression belongs to exactly one of the fundamental classifications in this taxonomy: lvalue, xvalue,
or prvalue. This property of an expression is called its value category.

------------------------
template<typename T> using id = T;
int i = 1;
int&& a = id<int[3]>{1, 2, 3}[i];          // temporary array has same lifetime as a
const int& b = static_cast<const int&>(0); // temporary int has same lifetime as b
int&& c = cond ? id<int[3]>{1, 2, 3}[i] : static_cast<int&&>(0);
                                           // exactly one of the two temporaries is lifetime-extended
const int& x = (const int&)1;  // temporary for value 1 has same lifetime as x
struct S { const int& m; };
const S& s = S{1};             // both S and int temporaries have lifetime of s
A temporary bound to a reference in a new-initializer ([expr.new]) persists until the completion of the full-expression containing the new-initializer. [ Note: This may introduce a dangling reference. — end note ] [ Example:
struct S { int mi; const std::pair<int,int>& mp; };
S a { 1, {2,3} };
S* p = new S{ 1, {2,3} };      // creates dangling reference because of "new" - works for automatic storage duration !

A { string a_ }
auto&& v = A().a_; //v now extends lifetime of A!

In C++17 the references are [expr.ref]/4.2 "If E1 is an lvalue, then E1.E2 is an lvalue; otherwise E1.E2 is an xvalue.",
 and [class.temporary]/6 covers that binding to E1.E2 actually extends E1 in this circumstance

----------------
 Treat placement new as making the pointer invalid for access use. launder may help though sometimes.

The effect of indirecting through a pointer returned from a request for zero size is undefined
In particular, a global allocation function is not called to allocate storage for objects with static storage duration, for objects or references with thread storage duration, for objects of type std​::​type_­info, or for an exception object. 

  ----------
|virtual base class |

for each distinct baseclass thatf is specified virtual, the most derived object shall contain a single base class subobject of that type. In your case this means that when constructing the base, your class F immediately calls A's default constructor, not B's.
-------------------
glvalue - is either lvalue or xvalue
struct X { int n; };
X foo();

X x;
x; // has a name, so it's a glvalue
std::move(x); // has a name (we're moving from "x"), so it's a glvalue
              // can be moved from, so it's an xvalue not an lvalue

foo(); // has no name, so is a prvalue, not a glvalue
X{};   // temporary has no name, so is a prvalue, not a glvalue
X{}.n; // HAS a name, so is a glvalue. can be moved from, so it's an xvalue

int x = 7; // x is an lvalue. x is also a glvalue.
           // 7 is a literal, so it is a prvalue. 7 is not a glvalue.

auto foo = static_cast<int&&>(x); // foo is an lvalue, so it is a glvalue
                                  // the cast is an rvalue but not a prvalue,
                                  // it is an xvalue. so it is a glvalue.

class Foo
{
    std::string name;

public:

    Foo(std::string some_name) : name(std::move(some_name))  {   }
    std::string& original_name()  {     return name;   }
    std::string copy_of_name() const  {    return name;   }
};
The expression some_foo.copy_of_name() is a prvalue, because copy_of_name returns an object (std::string), not a reference. Every prvalue is also an rvalue. (Rvalues are more general.)

The expression some_foo.original_name() is an lvalue, because original_name returns an lvalue reference (std::string&). Every lvalue is also a glvalue. (Glvalues are more general.)

The expression std::move(some_name) is an xvalue, because std::move returns an rvalue reference (std::string&&). Every xvalue is also both a glvalue and an rvalue


--------------------------
clang linker  process libraries left to right like g++, but once it gets a defined symbol it keeps it, so it does not require symbol to be defined *AFTER* it found a refenrece to it. So in g++ you may end up putting the same library multiple times!

symbol definitions get copies from static libraries
---------
Every expression belongs to exactly one of the fundamental classifications in this taxonomy: lvalue, xvalue, or prvalue. This property of an expression is called its value category
-----------------
sizeof(int) = 4
sizeof(long) = 8
---------------------
inside constexpr functions, variables may not be constexpr, but called functions must be!
only the most outter must constexpr functions needs to return a value, inner functions may return void!
-------------------
once values gets binded to constexpr variables, it cannot be changed!
-------------------
constexpr float in compile-time and runtime may be diffrent!
---------------
undefined behaviour is hard-error in constepxr! great to diagnos ones!
------------------
constexpr declares an object as fit for use in what the Standard calls constant expressions. 
constepxr arguments and return value must also be litteral type!
constexpr int f(double d)
{
     return d * 2.51;
}

constexpr double d1 = 6.6;
int arr[f(d1)]; // forces constexpr to be at compile-time!
constexpr int = f(d1);// forces constexpr to be at compile-time!
static_assert(array_size(arr) == f(d1), "asdas");

----------------
refrences are not really values, they just an alias to another object!
--------------------------------
 it currently unusable in constexpr evaluations:

Destructors cannot be constexpr - available in C++20
Dynamic memory allocation/deallocation isn't available - available in C++20
In-place construction using placement-new isn't available - available in C++20
---------------
To catch all allocations / deallocations, just override malloc and free in your own libarary. And by loading these with LD_PRELOAD your malloc and free will be executed rather than the standard one. It turns out that malloc and free are specifically designed in glibc to allow this and the stock calloc manages to call your imported malloc. Don't try this with any other functions. It won't work so good

class MyHeap {   
   public:    ...   
    void* allocate(size_t size);   
     void deallocate(void* p);    
   ...};

    void* operator new(size_t size, MyHeap* heap) {    return heap->allocate(size);}

// Example of allocating int on the custom storage using placement new!
    MyHeap heap;
    std::unique_ptr<int> p(new(&heap) int(0));

if the allocation function returns a null pointer, which is possible if the non-throwing overload was selected, e.g. with new(std::nothrow) T;


    placement operator new can also be overriden
    struct X {
    X() { throw std::runtime_error("");
}
// custom placement new
static void *operator new(std::size_t sz, bool b) {
  std::cout << "custom placement new called, b = " << b << '\n';
  return ::operator new(sz);
}
// custom placement delete
static void operator delete(void *ptr, bool b) {
  std::cout << "custom placement delete called, b = " << b << '\n';
  ::operator delete(ptr);
}
};
int main() {
  try {
    X *p1 = new (true) X;
  } catch (const std::exception &) {
  }
}

Array allocation may supply unspecified overhead, which may vary from one call to new to the next. The pointer returned by the new-expression will be offset by that value from the pointer returned by the allocation function. Many implementations use the array overhead to store the number of objects in the array which is used by the delete[] expression to call the correct number of destructors. In addition, if the new-expression is used to allocate an array of char, unsigned char, or std::byte, it may request additional memory from the allocation function if necessary to guarantee correct alignment of objects of all types no larger than the requested array size, if one is later placed into the allocated array.

-----------------
non-trivially destructuble classes cannot be constexpr at compile time

literal type: a type whose objects can be used as compile-time constants. One constraint that the Standard imposes on literal types is that they should provide a trivial destructor: a destructor that does nothing. As we can see above, optional’s destructor has to do something, so we cannot achieve our goal in general. However, we can achieve our goal for certain Ts that have a trivial destructor themselves. Imagine that T is int: we do not have to call int’s destructor — because it is trivial. Here we also arrive at the practical definition of a trivial destructor: it is a destructor that we can skip (simply not call it) without any harm.

Another requirement for literal types is that they should have at least one constexpr constructor. This constructor(s) will be used to create compile-time constants. However, in order to avoid “compile-time undefined behavior”, the Standard imposes a number of constraints on constexpr constructors and their types to make sure that no data member or base-class sub-object is left uninitialized
----------
When using rvalue references as return types, returning temporaries or function local variables still creates a dangling reference, just like with lvalue references. So it is only really applicable if you’re returning something like member variables or reference parameters and you’re moving them out of place. So should you use rvalue return types? I’d say you should only do it if you have an rvalue qualified member function but think about the consequences
type {
  string s;
  auto &&value() { return s; }
  auto &&rvalue = type().value();
  auto s = type().value(); // ok
  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -temporary bounded to reference extends
                                                    temporary lifetime except !(
                                                        1)a temporary bound to
                                                        a return value of a function
                                                            in a return statement
                                                                is not extended
      : it is destroyed immediately at the end of the return expression
            .Such function always returns a dangling referenc auto &
      s() {
    return string("asd");
  }; // bad!
  auto s(){retuirn string("asd")} is fine because only the first temporary gets
      destoryed *after *it copies into the rreturn value !(
          2)a temporary bound to a reference parameter in a function call exists
          until the end of the full expression containing that function call
      : if the function returns a reference,
        which outlives the full expression,
        it becomes a dangling reference string
            &s = f(string() / s /)-- -- -Dynamic initialization of a non -
                     local variable with static storage duration is
                         unordered if the variable is an implicitly or
                 explicitly instantiated specialization,
            is partially - ordered if the variable is an inline variable that
                               is not an implicitly or
                explicitly instantiated specialization,
            and otherwise is ordered.[ 
                Note:An explicitly specialized non -
                         inline static data member or
                     variable template specialization has ordered initialization
                         . — end note ]-- -- -- -- -- -- -- -- -- --If the
                        initialization of a non -
                    local variable with static or
                thread storage duration exits via an exception,
            std::terminate is called-- -- -- -- -- -An aggregate is an array or
                a class(Clause 9) with no user -
                    provided constructors(12.1)(including those inherited(7.3.3)
                                                    from a base class),
            no private or protected non - static data members(Clause 11),
            no base classes(Clause 10) and no virtual functions(10.3),
            and no virtual,
            private or protected base classes(10.1).

                       In all cases,
            if the empty pair of braces {} is used and T is an aggregate type, aggregate-initialization is performed instead of value-initialization

(1)T t, new T - default intialization(calls default constructor / set intermediate values - like not good ones)! static and per thread variables will get zero-initialized!

(2)T t{}, new T{}, T() - value initialization. if default constructor declared, deleted object gets default initialized,otherwise zero-initialized (for trivial members) and default-initialoized for non-trivial
****In all cases, if the empty pair of braces {}
  is used and T is an aggregate type,
      aggregate - initialization is performed instead of value -
          initialization If T is a class type that has no default constructor
              but has a constructor taking std::initializer_list,
      list -
          initialization is performed *****

          (3)direct initialization -
          Initializes an object from explicit set of constructor arguments(4)
              aggregate initialization struct B {
    int x;
    int y
  };
  B1 a2 = {1, 1} B1 a2{1, 1}

          (5)list initialization -
          if type is an aggregate type,
     aggregate initialozion is performed B1 a2 = B1 {
    1, 1.1
  } // prohibited because of narrowing conversion
  -- -- -- -- -- -- -- -- -- -A braced - init - list is not an expression and
      therefore has no type-- -- -- -- -- -- -- --tempalte<class M, class T> M
              T::*pm -
          function object variable !M - function !T - class !void(A::*f1)(int) =
      &A::func; // declares member function variable f1!
  void (A::*)(int) -
      declares type !is_same<decltype(f1), void (A::*)(int)>::value =
      true decltype(&A::func1) decltype(&func1)

          sjtruct funcnameprinter<Ret (Class::*)(Args)> // has priority!
      struct funcnameprinter<Func(Class::*)> struct funcnameprinter<
          Func(*)> struct funcnameprinter<Func (*)(Args...)> // has priority!
      struct funcnameprinter<Func(Args...)>

      -- -- -- -- --

      is_nothrow_move_constructible !

      if allocators
          don't compare equal then move/copy operators need to use the corresponding allocator! (
              1)For copy say nodes are deleted using target allocator and
      allocated using the source allocator(2) For move operator elements are
          getting moved one by one if allocators don't comapre equal!

      -- -- -- -- -- -- -- -- -- -- -- -- -- A trick a wrap an int into another
          type !enum class align_val_t : std::size_t {};
------
The type does not have to implement move assignment operator in order to satisfy this type requirement: a copy assignment operator that takes its parameter by value or as a const Type&, will bind to rvalue argument.

If a MoveAssignable class implements a move assignment operator, it may also implement move semantics to take advantage of the fact that the value of rv after assignment is unspecified

--------------
The noexcept operator performs a compile-time check that returns true if an expression is declared to not throw any exceptions.

    _S_construct(_Alloc&, _Tp* __p, _Args&&... __args)
    noexcept(noexcept(::new((void*)__p)


--------------------------
- once value is moved from, something can be moved in!
- currently move semantics is non-destructive. there potentially may be a destructive move semantics!
(Another solution is to just declare that an object can not have a destructive move constructor unless all of its base classes and members have a non-destructive move constructor. Thus the destructive move constructor can non-destructively move construct the bases and members before destructively move constructing itself)
- The only requirement is that the object remain in a self consistent state (all internal invariants are still intact)
- For trivial classes, move and copy are identical operations (right down to the machine instruction level)

an object state that is not specified except that the object’s invariants are met and operations on the object behave as specified for its type [ Example: If an object x of type std::vector<int> is in a valid but unspecified state, x.empty() can be called unconditionally, and x.front() can be called only if x.empty() returns false. —end example ]

Therefore, the object is live. You can perform any operation that does not require a precondition (unless you verify the precondition first).


it is often more efficient to perform a destructive move operation
than a non-destructive move construction. In the case of a string, for example, a destructive move would
simply copy pointers. Since pointers are trivially copyable, the entire move operation becomes a trivial copy
that can be implemented as a memcpy. This optimization is magnified when operating on arrays of strings; the
entire array can be destructively moved with a single memcpy or memmove. This optimization was implemented
at Bloomberg before move constructors were even invented and has yielded significant performance gains. It
turns out that a large number of classes, like string, can be destructively moved using byte copies. Such
classes model a concept I call trivially destructive-movable

-------------
prvalue - pure rvalue
xvalue - rvalue from lvalue
glvalue - it is kidn of lvalue
----------------
Two pointers can be compared with using the comparison function objects less,  greater etc. Otherwise, using blanket operator< etc, this is only possible if the pointers point to elements of the same array object or one past the end. Otherwise, results are unspecifie
----------------------------------
destructors are recursive by default
-------------------
std::quick_exit  - quick ternination without static destructores called!
use std::at_quick_exit to register functions if you still want something to be called! Up to 32 functions can be registed

Aslo _exit can do the job!

-----
rvalue references (&&) mean we can move from it! That is important to realize as by value
parameters are similar in this concept!
---------------------------
decltype(auto) - is good for forwarding functions as if we want to deduce the return type for free and at the same time pass EXACT values.

becuase forwarding refrences have the name they must be used with std::forward otherwise theyll be always treated like lvalues!
To put it another way, the compiler can figure out which forward to call based on whether the input is an lvalue or rvalue, but that is unrelated to whether you want the desired output to be an lvalue or rvalue. The compiler knows the type of the input, but you need to explicitly tell it the type of the output, because they are not the same! Like complier could figure it out, BUT what if you want first to use that forwarding reference as lvalue first and then move from it in the end!
like 
temaplte <T> f()(T&& t_) {
                g(t_);
                x(forward<T>(t_)); }

-------------
by value parameters can avoid copies with std::move!
f(vector<int> v)
...
f(std::move(vec)) - parameter is constructed using move rather then by copy!
--------------------
Method chaining is fine as long as the methods are sensible and only modify the invocant (for which the effects are well ordered, because the second method is called on the result of the first) - changed in c++17 a bit
---------------------
 For any object (other than a base-class subobject) of trivially copyable type T, whether or not the object
holds a valid value of type T, the underlying bytes (6.6.1) making up the object can be copied into an array of
§ 6.7 57
©ISO/IEC N4713
char, unsigned char, or std::byte (21.2.1).39 If the content of that array is copied back into the object,
the object shall subsequently hold its original value. [ Example:
#define N sizeof(T)
char buf[N];
T obj; // obj initialized to its original value
std::memcpy(buf, &obj, N); // between these two calls to std::memcpy, obj might be modified
std::memcpy(&obj, buf, N); // at this point, each subobject of obj of scalar type holds its original value
 -------
 Strict ALIASING Rules
---------------------
 If a program attempts to access the stored value of an object through a glvalue of *OTHER* than one of the following types the behavior is undefined:

the dynamic type of the object,

a cv-qualified version of the dynamic type of the object,

a type similar (as defined in [conv.qual]) to the dynamic type of the object,

a type that is the signed or unsigned type corresponding to the dynamic type of the object,

a type that is the signed or unsigned type corresponding to a cv-qualified version of the dynamic type of the object,

an aggregate or union type that includes one of the aforementioned types among its elements or non-static data members (including, recursively, an element or non-static data member of a subaggregate or contained union),

a type that is a (possibly cv-qualified) base class type of the dynamic type of the object,

a char, unsigned char, or std​::​byte type.

-----------------------
 the only way union can be used for type punning is if union is standart layout and it has standart layout struct. then these standarat layout structs can share the common memeber sequence!
 ----------------------
 integer represention of a pointer might not be the same but sequence  as the pointer itself
 it is ok to convert a pointer to integer and then integer back to pointer. But modifying that integer in the middle is kind of on the endge according to c++ standarrt!
 ---------
 trivial destructors can be skipped

 pretty bad to write constructors like this! as it can make trivial type untrivial and call unncessry contrustruct and destructor!
B() {}
 -------------
 Launder is defined in terms of a pointer to a pre-existing object. That pointer is then destroyed, and a new object with the same address is created (which is well defined).  std::launder then lets you take the pointer to the object that no longer exists and get a pointer to the existing object.

 only works on placement new

 --------------
 C++ does not allow you to do in-place type punning. Not in a general sense. The closest you can get is the new wording for unions, but that doesnt allow you to access the value through an object of a different type. C++ only supports type punning by copy or using unsigned, char* or byte!
 ----------------------------
 void - type with an empty set of values. It is an incomplete type that cannot be completed (consequently, objects of type void are disallowed). There are no arrays of void, nor references to void. However, pointers to void and functions returning type void (procedures in other languages) are permitted
 ----------------
 One pointer is said to alias another pointer when both refer to the same location or object. 
 Strict aliasing is an assumption, made by the C (or C++) compiler, that dereferencing pointers to objects of different types will never refer to the same memory location (i.e. alias eachother.)

 ------------------------

The versions (1-4) are implicitly declared in each translation unit even if the <new> header is not included. Versions (1-8) are replaceable: a user-provided non-member function with the same signature defined anywhere in the program, in any source file, replaces the default version. Its declaration does not need to be visible.

The behavior is undefined if more than one replacement is provided in the program for any of the replaceable allocation function, or if a replacement is defined with the inline specifier. The program is ill-formed if a replacement is defined in namespace other than global namespace, or if it is defined as a static non-member function at global scope.

size operator delete exist in C++ becuase user may pass the size himself to speed up performance and not to store size along with chunk of allocated data

 operator new can hounr alingment up to 16 bytes, but if more required,aligned_alloc shall be used!

 C++17 new operators support now alingment to whatever size is set and it does respect alingas for heap allocation! Pre C++17 an option is too overallocate and then aling
with std::align!

So in your case with C++17 or -faligned-new, since Foo has new-extended alignment, Foo* f = new Foo(); will call void* operator new(size_t, align_val_t) to allocate memory and return a pointer to a Foo object that is properly aligned on a 64-byte boundary. Under earlier standards that was not the case.

For example:
void * RingBuffer::operator new(size_t request)
{
                static const size_t ptr_alloc = sizeof(void *);
                static const size_t align_size = 64;
                static const size_t request_size =
                    sizeof(RingBuffer) + align_size;
                static const size_t needed = ptr_alloc + request_size;

                void *alloc = ::operator new(needed);
                void *ptr = std::align(align_size, sizeof(RingBuffer),
                                       alloc + ptr_alloc, request_size);

                ((void **)ptr)[-1] = alloc; // save for delete calls to use
                return ptr;  
}

void RingBuffer::operator delete(void * ptr)
{
                if (ptr) // 0 is valid, but a noop, so prevent passing
                         // negative memory
                {
                  void *alloc = ((void **)ptr)[-1];
                  ::operator delete(alloc);
                }
}

--------------------------
Regular std::malloc aligns memory suitable for any object type (which, in practice, means that it is aligned to alignof(std::max_align_t)). aligned_alloc(size,alingment) function is useful for over-aligned allocations, such as to SSE, cache line, or VM page boundary

 ----------
 An alignment is an implementation-defined integer value representing the number of bytes between successive addresses at which a given object can be allocated.

Another unexpected penalty to unaligned accesses is that it breaks atomics on some architectures. Atomic stores may not appear atomic to other threads on x86 if they are misaligned

If during pointer conversion, the resulting pointer breaks pointer alingment requirnments, it is undefined behaviour!

char arr[4] = { 0x0F, 0x0, 0x0, 0x00 }; // Could be allocated on a 1 or 2 byte boundary
int x = *reinterpret_cast<int*>(arr);   // Undefined behavior we have an unaligned pointer

Fix -> alignas(alignof(int)) char arr[4] = { 0x0F, 0x0, 0x0, 0x00 }; 

the standard blessed method for type punning in both C and C++ is memcpy. This may seem a little heavy handed but the optimizer should recognize the use of memcpy for type punning and optimize it away and generate a register to register move. For example if we know int64_t is the same size as double. Well even though c++ standard does not explicitly confirm it. Another way to go is modifying an object through a pointer to unsgined char - that way it does not violate strict aliasing rules.
*****reinterpret_cast NOR union will work!*****
void func1( double d ) {
                std::int64_t n;
                std::memcpy(&n, &d, sizeof d);

***putting an object at something other than its proper alignment ISSSSS UB***

the only way union can be used for type punning is if union is standart layout and it has standart layout struct. then these standarat layout structs can share the common memeber sequence!
---------------------
explicit constructors are not allowed in copy initiliztion!
explicit A() {}
A obj = {} - not allowed !A f() { return {}; } - not allowed!
------------------------------
C++ cannot re-order data fields declared in struct! But compiler adds padding automatically to ensure alignment
-----------------
Pointers returned by allocation functions such as malloc are suitably aligned for any object, which means they are aligned at least as strictly as max_align_t.

max_align_t is usually synonymous with the largest scalar type, which is long double on most platforms, and its alignment requirement is either 8 or 16.

Note: of course, the reason why the alignas keyword has to fix all alignment, including heap alignment, is that it's undefined behavior to dereference an unaligned pointer. So if alignas didn't fix heap alignment, it would be impossible to dereference heap object of alignas types. The attribute is different as it's only a compiler hack, not a real part of the language, so it's not tightly spec'd

If a request for a specific extended alignment in a specific context is not supported by an implementation, the program is ill-formed. Additionally, a request for runtime allocation of dynamic storage for which the requested alignment cannot be honored shall be treated as an allocation failure

-------------
alignment of class struct is the max alingment of its member!
-----------
 void* r = mmap(NULL, fileSize, PROT_READ, MAP_PRIVATE | MAP_POPULATE, fd, 0);
if ((void *)-1 == r) // ticky!
  -- -- -during the construction of the complex object which uses multiple
            inheretance,
      the most derived virtual based class would be initialized first !

      -- -- -- -- -- -- -- -- -- -- --class Foo {
    Foo(const Foo &other) = delete;

#stil valid !becaose only Foo copy constructor was delted !tempalted onces     \
    are not affected
    template <typename T> Foo &operator=(const Foo &other) {}
    template <typename T> Foo(const Foo &other) {}
  } -- -- -- -- -- -- -- -- -- -- -The private -
      inheritance variant allows access to the protected members of the
          base class
              .Basically protected and public members of the base class become
                  the private members of the derived class.And still they can be
                      manually exposed !class D_prot : protected B {
  public:
    using B::f; // Note: Not using B::f(int,float)
  };
--------------------
A legitimate, long-term use for private inheritance is when you want to build a class Fred that uses code in a class Wilma, and the code from class Wilma needs to invoke member functions from your new class, Fred. In this case, Fred calls non-virtuals in Wilma, and Wilma calls (usually pure virtuals) in itself, which are overridden by Fred. This would be much harder to do with composition
---------------
destrcturo has to be virtual not just because class has virtual functions but also when somebody creates dervied class and transfers it to a pointer or reference of the base class and then delete on base class object is called!
---------------
A virtual function allows derived classes to replace the implementation provided by the base class
The derived class can either fully replace (�override�) the base class member function, or the derived class can partially replace (�augment�) the base class member function. The latter is accomplished by having the derived class member function call the base class member function, if desire

---------
implicit default constructor
struct B {
  explicit B() = default;
};
int main() {
  // B b1 = {}; // Error, would use explicit default constructor
  B b2{};
}
-- -- -- -- -- -- -std::ios_base::sync_with_stdio(false) -
        turns off string synchronization with C !That is means streams
            are not longer thread -
        safe and
    C++ streams buffer their own I / O independetnly.Turned on my default and
    enables to mix C++ and
    C streams

        cin.tie(nullptr);
-unties cin and cout streams which makes I /
    O faster !Basically by defualt(if tied) cin /
    cout would flush each other before proceeding.

    while (cin >> x) { // code } reads until no more data found!

  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -

                                            struct T {
    int a;
    int b;
  };
  if zero
    initialized objects
            don 't have a constructor then members and base clases would be also zero-initiazlied! If T does define default constructor then it won' t
                be zero -
        initialized !static onbjects are also can be zero initialized !{
      T();
      T t = {};
      T{};
    }
  static T t;
  always zero initialized !

      A zero -
      initialized pointer is the null pointer value of its type,
      even if the value of the null pointer is not integral zero zero
              initializing examples above and
          some real examples below !

          namepsace {
    static T t1; // a=0; b=0
    T *p;        // p = nullptr
    int i;       // i = 0
    double f;    // f = 0
  }

  T t1 = {}  // a= 0, b =0
  T t1 = T() // a = 0, b = 0

      struct TX {
    TX() {}
    int a;
    int b;
  }; // constructor defined
  void func() {
    TX t1 = {}  // a= garbage, b =garbage
    TX t1 = T() // a = garbage, b = garbage
  }
  static TX t1 {} // a= 0, b =0 //zero-initialized BECAUSE STATIC!

  -- -- -- -- -- -- -- -- -- -- -if class has a user defined destructor then
                                     move operators will be implicitly deleted
                                         .Copy sitll can live !-- -- -- -- -- --

                                 vector<bool>
                                     vec;
  auto ret = vec[4];
  auto will give you prob here as vec[4] returens a proxy object !

      -- --forwarding reference type deduction examples !DO REMEMBER THIS !

      template <typename T>
      void f(T && param);

  int a = 27;
  int &b = x;
  const int &c = b;
  int &&d = 0;

  f(a)->T deduced to int &and param type to int &f(b)
          ->T deduced to int &and paramt type to int &f(c)
          ->T deduced to const int &and param type to const int &f(12)
          ->T deduced to int and param type to int &&f(d)
          ->T deduced to int &&and param type to int &&

      -- -- -- -- -- --Temporaries life until the end of the full expression in
                           which they are created
                               .A full expression
                       " is an expression that's not a sub-expression of another expression.
                       "Temporary objects are destroyed as the last step in evaluating the full-expression (1.9)
                       that(lexically) contains the point where they were
                           created.This is true even if that evaluation ends in
                               throwing an exceptio baz(bar(foo().c_str()));
  string returned from c_str still lives !-- -- -- -- -- --

      The overloads that do not use std::piecewise_construct_t assume that each
          tuple argument becomes the element of a pair.The overloads that use
              std::piecewise_construct_t assume that each tuple argument is used
                  to construct,
      piecewise, a new object of specified type,
      which will become the element of the pair IT WORKS ONLY WITH
          TUPLES !-- -- -- -- -- -- -- -- -- -- -- --std::forward_as_tuple If
              the arguments are temporaries,
      forward_as_tuple does not extend their lifetime;
  they have to be used before the end of the full expression
      std::map<int, std::string>
          m;
  m.emplace(std::piecewise_construct, std::forward_as_tuple(10),
            std::forward_as_tuple(20, 'a')) std::tie -
          Creates a tuple of lvalue references to its arguments or
      instances of std::ignore.

      -- -- -- -- --

      -- -- -- -- --const Fred &
      operator[](unsigned index)
          const;                    // Subscript operators often come in pairs
  Fred &operator[](unsigned index); // Subscript operators often come in pai
----------
Sometimes its safe to throw an exception even while std::uncaught_exception() == true. For example,
 if stack unwinding causes an object to be destructed,
 the destructor for that object could run code that throws an exception as long as the exception 
is caught by some catch block before escaping the destructor.

----------
template <typename... Args>
void print_size(Args... args)
{
  cout << sizeof...(args) << endl;
}

int main(void) {
  print_size();                          // outputs 0
  print_size("Is the answer", 42, true); // outputs 3
}

-- -- -- -- -CERT coding standart https
    : // wiki.sei.cmu.edu/confluence/display/cplusplus/2+Rules

      -- -- -- -- -- -- -lambdas diffrences between C++ 11 and
    C++ 14 -- -- -- -- -- -- -In C++ 14 all of these possible.

    Move example :

    auto f1 = [v = std::move(v)](auto arg) { f(42, arg, std::move(v)); };
Expression example :

    auto f1 = [sum = a + b](auto arg) { f(42, arg, sum); };
See question

    Perfect forwarding
    : You can write

      auto f1 = [=](auto &&arg) { f(42, std::forward<decltype(arg)>(arg)); };

----------------
copy elision
(RVO, no copy/move of thrown/catch objects if they have no-sideeffects)

----------------
move semantics
--------------
std::vector has an overload for a const A & and a A &&, so the overload with const A & will get chosen 
and the const A && is implicitly casted to const A &

(1) const objects cannot be moved, copy construct will be called
(2) only one of these functions can be declared
    void f(Base other)
    void f(Base&& other)
(3) Remember that �&&� indicates a universal reference only
 where type deduction takes place

universal ref: ->
template<typename T>
void f(T&& param);

not universal ref->template <typename T> void f(vector<T> && param);

Universal references can only occur in the form �T &&�!Even the simple addition
        of a const qualifier is enough to disable the interpretation
            of �&&� as a universal
            referenc

    (4) &&
    has preference when taking a temporary Base() as input,
    then template <T>
    (T &&),
    then const Base & !coz overloading has a precedence over templates !

                      void
                      f(Base && other) template <typename T>
                      void f(T && other) {}
void f(const Base& other) 
void f(const Base&& other)

(5) C++ does not guarantee any order for the evaluation of function arguments

(6) exception is a virtual class


(7) The lifetime of the temporary may be extended by binding a reference to it 
(in which case its lifetime is extended until the end of the lifetime of the reference),
 or by using it as an initializer in a constructors initializer list
 (in which case its lifetime is extended until the object being constructed is fully constructed).

Temporary objects are destroyed as the last step in evaluating the full-expression (1.9) 
that (lexically) contains the point where they were created. This is true even if that 
evaluation ends in throwing an exception

(8) TEMPLATE CANNOT BE COPY CONSTRUCTOR! That is why when declaring template copy and assinge operators,
 also need to declare default ones as well!

(9) std::shared_ptr<int> sp( new int[10], std::default_delete<int[]>() );
std::shared_ptr<std::array<int, 6>> ptr(
    std::make_shared<std::array<int, 6>>(std::array<int, 6>()));
make_shared cannot be used with a custom deleter(10) std::string get_string() {
  return std::string();
}

BOTH ARE NOT EQUAL std::string get_string() {
  return {}; // an empty string is returned
}

If T is an aggregate, then each member is initialized from its brace-or-equal-initializer if it had one,
 otherwise as if by {} (so apply these steps recursively).
If T is a class type with a user-provided default constructor, that constructor is called.
If T is a class type with an implicitly-defined, or = defaulted default constructor, <!!!!!!the object is zero-initialized and then !!!!!!!!!!!!!!!!!!!>>>>>
the default constructor is called.
If T is a std::initializer_list, the return value is an empty such list.
Otherwise (i.e. T is a non-class type -- return types cannot be arrays), the return value is zero-initialized


(11) constructors can be chained
A(int)
A(int,int):A(int)!
  
(12) if (this != _STD addressof(_Right))

(13) make_transform_iterator
     make_filter_iterator

(14) default parameters are re-evaluated on each call!
     void f(const A& a = Time::now())
     ....
     f(Time::now())
     f(Time::now())

     for virtual functions default parameters are taken from the type it is bein syntactically 
     called from.

(15) Captureless lamba types are implicitly convertetable to function pointer type. However + is needed for it to happen
     template <typename R, typename A>
     void foo(R (*fptr)(A)) {
  puts(__PRETTY_FUNCTION);
}
     foo( [](double x) {
                    return int(x); } // WRONG!
     foo( +[](double x) {
                    return int(x); } //CORRECT

(16) declare stack variables as auto
      auto a = type;
      auto b = type_final { type}

(17) in C++98 throw() would cause the stack to unwind for the caller of it, but from c++11
     noexcept() does not require that and program can terminatate!

(18) struct Point
     {
	constexpr Point(int x, int y):x_(x), y_(y) {}
	constexpr getX() { return x_;
}
constexpr getY() { return x_; }
constexpr setX(int x) noexcept { x_ = x; }
constexpr setY(int y) noexcept { y_ = y; }
     };
}

(19) struct A {
  A(int x) {}

private:
  A(A const &);
};

A f() { return {10}; }

(20) You can have UTF - 8 strings,
    Raw strings and more.Here I want to show raw strings.We define a raw string
        as below :

    string s = R"(Hello, World!)"; // Stored: "Hello, World!"
A raw string skips all escape characters like \n or \". e.g.

    string str = "Hello\tWorld\n";
string r_str = R"(Hello\tWorld\n)";
cout << str << r_str;
          Output:

            Hello World Hello\tWorld\n You can also have multiple line raw
                string :

                string r_str =
                    R"(Dear Programmers,
I'm using C++11
Regards, Swift!)";
            cout << r_str;

            (21) Use std::addressof instead of &to get an address in templated
                    code since &
                    operator can be overloaded and
                hence a proper address of an object may not be retrieved !

                (22) Padding aligns structure members to
                "natural" address boundaries -
                say,
                int members would have offsets,
                which are mod(4) ==
                    0 on 32 - bit platform.Padding is on by default.It inserts
                                  the following "gaps" into your first structure
                :

                struct mystruct_A {
              char a;
              char gap_0[3]; /* inserted by compiler: for alignment of b */
              int b;
              char c;
              char gap_1[3]; /* -"-: for alignment of the whole struct in an
                                array */
            } x;
            Packing, on the other hand prevents compiler from doing padding -
                         this has to be explicitly requested -
                         under GCC it's __attribute__((__packed__)), so the following:

                         struct __attribute__((__packed__)) mystruct_A {
              char a;
              int b;
              char c;

              (23)

                      vector::push_back If the new size() is greater than
                  capacity() then all iterators and references(
                      including the past - the -
                      end iterator) are invalidated.Otherwise only the past
                  - the - end iterator is invalidated
            };

            (24) copy constructor that accepts const T &arguments,
                satisfy std::is_move_constructible copy assingment that accepts
                    constT &argument satifsfy std::is_move_assingable !

                (25)The operands of the operators typeid,
                sizeof, noexcept,
                and decltype(since C++ 11) are expressions that
                    are not evaluated(unless they are polymorphic glvalues and
                                      are the operands of typeid),
                since these operators only query the compile -
                    time properties of their operands.Thus,
                std::size_t n = sizeof(std::cout << 42);
            does not perform console output

                (26) variadic functions copy values !!!!drainByCopy(...) A a;
            drainByCopy(a)->A MUST have a copy constructor !

                (27)be carefull how you define copy constructors !class members
                    have to be copied manually if the copy constructor is
                        overriden !Object t_;
            TestThis(const TestThis &o)
                : t_(o.t_){}

                  (28)C -
                  style operators can take away const &
                volation in conversion !

                (29)That is if we want to collaps lots of data
                    into union !template <std::size_t Len, class... Types>
                    struct aligned_union;
(since C++11)
Provides the nested type type, which is a trivial standard-layout type of a size and alignment suitable for use as uninitialized storage for an object of any of the types listed in Types. The size of the storage is at least Len. std::aligned_union also determines the strictest (largest) alignment requirement among all Types and makes it available as the constant alignment_value

template <std::size_t Len, class... Types>
struct aligned_union
{
  static constexpr std::size_t alignment_value = std::max({alignof(Types)...});

  struct type {
    alignas(alignment_value) char _s[std::max({Len, sizeof(Types)...})];
  };
};

(30) void operator()(_Ty *_Ptr) const noexcept { // delete a pointer
  static_assert(0 < sizeof(_Ty), "can't delete an incomplete type");
  delete _Ptr;
}
If the object being deleted has incomplete class type at the point of deletion,
    and the complete class has a non - trivial destructor or
        a deallocation function,
    the behavior is undefined

    (31) Because the default constructor is constexpr,
    static shared_ptrs are initialized as part of static non -
        local initialization,
    before any dynamic non -
        local initialization begins
            .This makes it safe to use a shared_ptr in a constructor of
                any static objec

        (32) atomics has constexpr constructor and constexpr function cannot
            have side effects such as aotmic semantics.

        (33)For example : given x = 1;
f(++x, ++x);, you may expect a call to f(2, 3) but it's actually undefined behaviour. This behaviour is left undefined so the compiler's optimiser has more freedom to arrange operations with side effects to run in the most efficient order possible - perhaps even in parallel

(34) A sequence point defines any point in a computer program execution at which it is guaranteed that all side effects of previous evaluations will have been performed, and no side effects from subsequent evaluations have yet been performed
WITHING SINGLE EXPRESSION!

(35) for shared pointers constructor
 std::bad_alloc if required additional memory could not be obtained. May throw implementation-defined exception for other errors. delete ptr if T is not an array type, delete[] ptr otherwise) (since C++17) is called if an exception occurs.


(36) It is particularly difficult to detect overflow in signed integers because the C standard specifies signed integer overflow as 'undefined behavior' which means that the compiler can assume that overflow does not happen, and some compilers will even optimize away an explicit overflow check for this reason

Writing overflow checking code is difficult. gcc have built in functions to perfom such checks...https://gcc.gnu.org/onlinedocs/gcc/Integer-Overflow-Builtins.html

(37) //exception classes are not polymorphic
class MyExceptionBase { };
class MyExceptionDerived : public MyExceptionBase {};
void f(MyExceptionBase & e) {
  // ...
  throw e;
}
void g() {
  MyExceptionDerived e;
  try {
    f(e);
  } catch (MyExceptionDerived &e) {
    // ...code to handle MyExceptionDerived...   // THIS IS NOT CALLED!
  } catch (...) {
    // ...code to handle other exceptions...
  }
}

to correct class MyExceptionBase {
public:
  virtual void raise();
};
void MyExceptionBase::raise() { throw *this; }
class MyExceptionDerived : public MyExceptionBase {
public:
  virtual void raise();
};
void MyExceptionDerived::raise() { throw *this; }
void f(MyExceptionBase & e) {
  // ...
  e.raise();
}
void g() {
  MyExceptionDerived e;
  try {
    f(e);
  } catch (MyExceptionDerived &e) {
    // ...code to handle MyExceptionDerived...
  } catch (...) {
    // ...code to handle other exceptions...
  }
}

(38) null pointers can be deleted safely!
(39) C++11 states integer division and modulo to be non Ecludian. The same goes for flotas/doubles!
5 % -3 = -2! The reminder signs rounds towards zero!


(41) move_iterator!
vector<string> v{"asd", "asd"}
vector<string> v2(make_move_iterator(v.begin()), make_move_iterator(v.end()));
items from v gets moved!

(42) Type pack is super usefull when composition not going to work, but type pack will for inheretance
for example
template<class... T>
class D : public T...
{
}

(43) Overload resolution tries to minimize the operations performed to convert an argument to the corresponding parameter

(44) initializer_list have preference over any other constructor! even if implicit conversion will be involved!
say
clas Foo
foo(int, double, bool)
foo(initializer_list<int>& l) 

foo(2,2.3,true) -> foo(initializer_list<int>& l) is called!

(45) C++14 also removes the C++11 rule that constexpr member functions are implicitly const

(45) input iterator got dereferencing by value!

(46) set containers since C++14 now can accept key params as something which is like a key!
for example: template<typename T> iterator find(const T& key) -- this specializiotn added!
Now Compare struct can be done like this:
struct Compare
{
  using is_transpararent = void;
  bool operator(const std::string &l, const std::string &r) bool
  operator(const std::string &l, int l);
  {

    (47) std::piecewise_construct,
        std::vector<std::pair<std::string, std::string>> vs;
    // vs.emplace_back(std::make_pair(std::string(10, 'c'), std::string(10,
    // 'c')));
    vs.emplace_back(std::piecewise_construct,
                    std::forward_as_tuple("dsfsdfsdfsdfsdfsdfsdfsdfsdfsdf"),
                    std::forward_as_tuple(10, 'c'));

    (48) auto f1 =
        [this](
            auto l,
            decltype(l) r) { // second argument type is taken from the first one
(49) This usage of std::forward may seem strange, but it’s nothing more than perfectly forwarding the argument passed to the call operator as the forwarding reference. You do the perfect forwarding because error() could be implemented differently for lvalue and rvalue references
(50)
Since C++14, you can omit the type when using the operator wrappers from the standard library. Instead of writing std::greater<int>(), you can write just std::greater<>(), which automatically deduces the argument type when called.For example, when calling std::sort, you can write this:std::sort(numbers.begin(), numbers.end(),            std::greater<>());
(51) It's not that C++ disables the copy-assignment operator for lambda per-se, but that by default members in a lambda object are saved as const, and then the assignment operator can basically do nothing to assign to them, and so it is not generated. If you want lambdas to not hold members as const, you use the [...](...) mutable {...} syntax

Solution (1)
template <class T>
struct LambdaContainer {
    LambdaContainer(const T& lambda)
        : lambda{lambda} {}

    void resetLambda(const T& lambda) {
        this->lambda.~T();
        new (&this->lambda) T{lambda};
    }

    T lambda;
};

Solution (2) 
If this is still what you're going for, I think dynamic allocation (e.g. using unique_ptr) is fair game

(52) inline namespace
 namespace std {

#if __cplusplus < 1997L // pre-standard C++
    inline
#endif

    namespace pre_cxx_1997 {

(53) is_convertable! To test() { return std::declval<From>(); } Well-formed if implicit conversion can happen
(54) If T is an object or reference type and the variable definition T obj(std::declval<Args>()...); is well-formed
(55) Direct-initialization is more permissive than copy-initialization: copy-initialization only considers non-explicit constructors and non-explicit user-defined conversion functions, while direct-initialization considers all constructors and all user-defined conversion functions

  static_assert(!std::is_constructible<X, int>::value &&
                !std::is_convertible<int, X>::value, "");
  X x {1};  
***even though X is not either constructable / nor convertible, but it can make an object of X!***

In case of ambiguity between a variable declaration using the direct-initialization syntax (1) (with round parentheses) and a function declaration, the compiler always chooses function declaratio

(56) The effects of constant initialization are the same as the effects of the corresponding initialization, except that its guaranteed that it is complete before any other initialization of a static or thread-local object begins, and it may be performed at compile time


The compiler is permitted to initialize other static and thread-local objects using constant initialization, if it can guarantee that the value would be the same as if the standard order of initialization was followed

(57) access restricution does not change how overload works! make constructor privet is not going to remove it from the overload resolution set!
(58) that is how compiler will transform class member function call!
void Zazxdfun_managled(class* this)
{  this->a + this->b; }

(59) vtable pointer is set in the constructor
A::A()
{
this->Base::Base();
vtable = AVtalbe;
}

(60) data_(new T[N]{}) - value initilize array! note if T is integral type that means zero initializtion!
(61) 
       (1) const auto sum = [](int a, int b) {
  auto sum_impl=[](int a,int b,auto& sum_ref) mutable {
    if(a>b){
      return 0;
    }
    return a + sum_ref(a+1,b,sum_ref);
  };
  return sum_impl(a,b,sum_impl);
};
recursive lambda!

(2) template <class F>
struct y_combinator {
    F f; // the lambda will be stored here

    // a forwarding operator():
    template <class... Args>
    decltype(auto) operator()(Args&&... args) const {
        // we pass ourselves to f, then the arguments.
        // [edit: Barry] pass in std::ref(*this) instead of *this
        return f(std::ref(*this), std::forward<Args>(args)...);
    }
};

// helper function that deduces the type of the lambda:
template <class F>
y_combinator<std::decay_t<F>> make_y_combinator(F&& f) {
    return {std::forward<F>(f)};
}
auto sum2 = make_y_combinator([](auto sum2, int a, int b) {
  if (a>b) {
    return 0;
  }
  else {
    return  a+sum2(a+1,b);
  }
});


(62) lambdas which don't capture anything become free functions and can be casted to usual function pointers.

int (func:*)(int, int)
func = [](int,int){
};

(63) templated copy constructors are very powerfull, even normal value initialization constructors would be called
struct A {  template <typename T>  A(const T& o) {  s1 = o.s1;  } };
A a1("asdfasdf");

(64) A typical mistake in implementing the Coercion by Member Template Idiom is failing to provide the non-template copy constructor or copy assignment operator when introducing the templated copy constructor and assignment operator. A compiler will automatically declare a copy constructor and a copy assignment operator if a class does not declare them, which can cause hidden and non-obvious faults when using this idiom.

(65) template< class T, class Compare >
constexpr T max( std::initializer_list<T> ilist, Compare comp );
(since C++14

(66) After that, whether or not an exception was thrown by any destructor, the delete expression invokes the deallocation function: either operator delete (for the first version of the expression) or operator delete[] (for the second version of the expression), unless the matching new-expression was combined with another new-expression (since C++14)

In the first alternative (delete object), if the static type of the object to be deleted is different from its dynamic type, the static type shall be a base class of the dynamic type of the object to be deleted and the static type shall have a virtual destructor or the behavior is undefined. !!!!!!!!!!!!!!!!In the second alternative (delete array) if the dynamic type of the object to be deleted differs from its static type, the behavior is undefined!!!!!!!!!!!!!!!!!!! 

REASON: If you have an array of elements of the derived class and assign that to a pointer to an array of the base class, you no longer have any info to tell you what the element size is

(67) Pointer arithmetic does not account for polymorphic object sizes, and attempting to perform pointer arithmetic on a polymorphic object value results in undefined behavior.

The C++ Standard, [expr.sub], paragraph 1 [ISO/IEC 14882-2014], defines array subscripting as being identical to pointer arithmetic. Specifically, it states the following:

The expression E1[E2] is identical (by definition) to *((E1)+(E2)).

Do not use pointer arithmetic, including array subscripting, on polymorphic objects.

(68) vector<T[]> v1(34); Cannot use array in C++ contaienrs because they must be coppieable / moveable and assinable!