


# pointers to member functions

  class Fred { /*...*/ };
typedef  int (Fred::*FredMemFn)(int i, double d);  // use a typedef!!! please!!!
void sample(Fred x, Fred& y, Fred* z, FredMemFn func)
{
  x.*func(42, 3.14);
  y.*func(42, 3.14);
  z->*func(42, 3.14);
}
BUT please consider using a std::invoke or macro instead:

void sample(Fred x, Fred& y, Fred* z, FredMemFn func)
{
  std::invoke(func, x, 42, 3.14);
  std::invoke(func, y, 42, 3.14);
  std::invoke(func, *z, 42, 3.14);
}

note that they cannot be converted to void*

```
template <class... Args>
struct type_list
{
};
template <class FuncSig>
struct extractor
{
};
template <class R, class... Args>
struct extractor<R(Args...)>
{
  using return_type = R;
  using argument_list = type_list<Args...>;
};

template <class S, class C>
struct Functor<S C::*>
{
  using FuncPtr = S C::*;
  template <class... Args>
  // using ResultType = decltype((std::declval<C>().*std::declval<FuncPtr>())(std::declval<Args>()...));
  using ResultType = decltype((std::declval<C>().*std::declval<FuncPtr>())(std::declval<Args>()...));

  template <class... Args>
  using ResultType2 = decltype(std::declval<S>()(std::declval<Args>()...));

  void print() { std::cout << "\n Class Func Ptr \n"; };
  template <class... Args>
  void call(C& c, FuncPtr f, Args... args)
  {
    static_assert(std::is_same_v<FuncPtr, int (C::*)(Args...)>);
    //--------------------------------------------------------------->
    static_assert(std::is_same_v<typename extractor<S>::return_type, int>);
    //<---------------------------------------------------------------
    static_assert(std::is_same_v<ResultType<Args...>, int>);
    static_assert(std::is_same_v<ResultType2<Args...>, int>);
    (c.*f)(std::forward<Args>(args)...);
    wow(typename extractor<S>::argument_list{});
  }

  template <class... Args>
  void wow(type_list<Args...> in)
  {
    std::cout << "\n WOWWWOOWOWWOW extract argument list worked \n";
  }
};
```

 # union
 
 A union is a special class type that can hold only one of its non - static data members at a time

 A union can have member functions(including constructors and destructors), but not virtual functions.

 A union cannot have base classes and cannot be used as a base class.

 A union cannot have non-static data members of reference types.

IMPORTANT!!!! All union objects would start at the same address and would have the same alignment (largest possible)
IMPORTANT!!!! Union members are pointer interconvertible, but derefercing those pointer may not be valid depending on what active member of the union is
IMPORTANT!!!  Innitial common sequence can be read from any union member!

If a union contains a non-static data member with a non-trivial special member function(copy / move constructor, copy / move assignment, or destructor), that function is deleted by default in the union and needs to be defined explicitly by the programmer.If a
union contains a non - static data member with a non - trivial default constructor, the default constructor of the union is deleted by default unless a variant member of the union has a default member initializer .

At most one variant member can have a default member initializer.

If members of a union are classes with user - defined constructors and destructors, to switch the active member, explicit destructor and placement new are generally needed

union S {
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
  // now, s.vec is the active member of the union
  s.vec.push_back(10);

If two union members are standard-layout types, its well-defined to examine their common subsequence on any compiler

[ all non-static data members will have the same address ]

The implicitly-defined copy/move constructor for a union X copies the object representation ([basic.types.general]) of X. For each object nested within ([intro.object]) the object that is the source of the copy, a corresponding object o nested within the destination is identified (if the object is a subobject) or created (otherwise), and the lifetime of o begins before the copy is performed.

union A{
  struct {
    char a;
    int b;
  };
};

A a;
A b = a;
will contain a's padding bits as well

# constructors

user-provided constructor deletes default one
If no user-declared constructors of any kind are provided for a class type (struct, class, or union), the compiler will always declare a default constructor as an inline public member of its class.

user-declared copy constructor delete move operators 

For an object with a non-trivial constructor, referring to any non-static member or base class of the object before the constructor begins execution results in undefined behavior. For an object with a non-trivial destructor, referring to any non-static member or base class of the object after the destructor finishes execution results in undefined behavior.

## Trivial default constructor

The default constructor for class T is trivial (i.e. performs no action) if all of the following is true:

The constructor is not user-provided (i.e., is implicitly-defined or defaulted on its first declaration).
- T has no virtual member functions.
- T has no virtual base classes.
- T has no non-static members with default initializers.
(since C++11)
- Every direct base of T has a trivial default constructor.
- Every non-static member of class type (or array thereof) has a trivial default constructor.

A trivial default constructor is a constructor that performs no action. 

## Trivially copyable class 
has at least one eligible copy constructor, move constructor, copy assignment operator, or move assignment operator,
each eligible(the one choosed) copy constructor is trivial
each eligible(the one choosed) move constructor is trivial
each eligible(the one choosed) copy assignment operator is trivial
each eligible(the one choosed) move assignment operator is trivial, and
has a non-deleted trivial destructor.

note that you can delete everything above but say copy constructor & destructor and class will still be trivial


## Trivial copy constructor 

The copy constructor for class T is trivial if all of the following are true:

it is not user-provided (that is, it is implicitly-defined or defaulted) ;
T has no virtual member functions;
T has no virtual base classes;
the copy constructor selected for every direct base of T is trivial;
the copy constructor selected for every non-static class type (or array of class type) member of T is trivial;
A trivial copy constructor for a non-union class effectively copies every scalar subobject (including, recursively, subobject of subobjects and so forth) of the argument and performs no other action. However, padding bytes need not be copied, and even the object representations of the copied subobjects need not be the same as long as their values are identical.

The implicitly-declared or defaulted copy constructor for class T is defined as deleted if any of the following conditions are true:

- T has non-static data members that cannot be copied (have deleted, inaccessible, or ambiguous copy constructors);
-T has direct or virtual base class that cannot be copied(
                     has deleted, inaccessible, or ambiguous copy constructors);
-T has direct or virtual base class or
    a non - static data member with a deleted or inaccessible destructor;
-T is a union - like class and has a variant member with non -
    trivial copy constructor;
-T has a data member of rvalue reference type;
- T has a user-defined move constructor or move assignment operator (this condition only causes the implicitly-declared, not the defaulted, copy constructor to be deleted).


## A trivial class is a class that

- is trivially copyable, and
- has one or more eligible default constructors such that each is trivial


## Standard-layout class

A standart-layout class is a class that

has no non-static data members of type non-standard-layout class (or array of such types) or reference,
has no virtual functions and no virtual base classes,
has the same access control for all non-static data members,
has no non-standard-layout base classes and just one of such classes with data memberes,
has all non-static data members and bit-fields in the class and its base classes first declared in the same class,
arrays of standart layout classes are standart layout too
 Informally, none of the base classes has the same type as the first non-static data membe
---> only one class in the hierarchy has non-static data members <----
struct A {
  int a;
  bool aa;
};
struct B : A {
  bool b;
};

B is not standart-layout because two classesin the hierarchy got memebers!

No tail padding from the Base POD(Standard Layout and Trivial) classes can be re-used. 
gcc layout is such that it can implement the copy constructor / assignment for A as copying 64bit in one instruction, which would overwrite B::b if it used the padding. Functions taking an A* might also memcpy() into it, again overwriting the padding. A common usage with POD types. But I think that is just a niceness of the ABI. Note: Windows ABI doesnt use tail padding at all giving

An implementation already can't store to the tail padding of an object if that object might be a base subobject of some other object.




Two standard-layout unions are called layout-compatible if they have the same number of non-static data members and corresponding non-static data members (in any order) have layout-compatible types.

Standard-layout types have the following special properties:

- In a standard-layout union with an active member of non-union class type T1, it is permitted to read a non-static data member m of another union member of non-union class type T2 provided m is part of the common initial sequence of T1 and T2 (except that reading a volatile member through non-volatile glvalue is undefined).

- A pointer to an object of standard-layout class type can be reinterpret_cast to pointer to its first non-static non-bitfield data member (if it has non-static data members) or otherwise any of its base class subobjects (if it has any), and vice versa. In other words, padding is not allowed before the first data member of a standard-layout type. Note that strict aliasing rules still apply to the result of such cast.

- The macro offsetof may be used to determine the offset of any member from the beginning of a standard-layout class.



 Otherwise, if the braced-init-list has no elements, T is value-initialized.


The implicitly-defined copy/move constructor for a non-union class X performs a memberwise copy/move of its bases and member, hence padding bits may not be coppied actually with =! However still not clear if memcpy preservers padding bits - normally GCC / clang do it, but there are bug reports when it does not happen!


standart-layout types are not always trivally copybale and care needs to be taken when doing memcopy on them. The main point is that they can be used by C code as they got single access. If there are multi-access sections in a class than the compiler can re-order *sections* which won't work with C.

Section 9.2.12:

Nonstatic data members of a (non-union) class declared without an intervening access-specifier are allocated so that later members have higher addresses within a class object. The order of allocation of nonstatic data members separated by an access-specifier is unspecified (11.1)"


## Empty base class optimization
The Empty Base Optimization is what will allow an empty base to not "inflate" the object, as you call it. However, you must be careful so that an object does not inherit from the same empty base twice or this optimization won't be allowed

class S{
  std::string s;
  S() : s("sadf")
};
< --member initializer list int n = 7;

default member initialize

# construction

 For an object with a non - trivial constructor, referring to any non - static member or base class of the object before the constructor begins execution results in undefined behavior.For an object with a non - trivial destructor, referring to any non - static member or base class of the object after the destructor finishes execution results in undefined behavior.

 During the construction of an object, if the value of the object or any of its subobjects is accessed through a glvalue that is not obtained, directly or indirectly, from the constructor's this pointer, the value of the object or subobject thus obtained is unspecifie

 A constructor that is not declared with the specifier explicit and
 which can be called with a single parameter(until C++ 11)
 is called a converting constructor.

 Unlike explicit constructors, which are only considered during direct initialization( which includes explicit conversions such as static_cast), converting constructors are also considered during copy initialization, as part of user - defined conversion sequence
                .

 It is said that a converting constructor specifies an implicit conversion from the types of its arguments(if any) to the type of its class.Note that non - explicit user - defined conversion function also specifies an implicit conversion

An expression e is said to be implicitly convertible to T2 if and only if T2 can be copy - initialized from e,
that is the declaration T2 t = e; is well-formed (can be compiled), for some invented temporary t. Note that this is different from direct initialization (T2 t(e)), where explicit constructors and conversion functions would additionally be considered.


#  Implicitly-defined default constructor

If the implicitly-declared default constructor is not defined as deleted, it is defined (that is, a function body is generated and compiled) by the compiler if odr-used or needed for constant evaluation (since C++11), and it has the same effect as a user-defined constructor with empty body and empty initializer list. That is, it calls the default constructors of the bases and of the non-static members of this class. Class types with an empty user-provided constructor may get treated differently than those with an implicitly-defined or defaulted default constructor during value initialization.


If no user-declared constructors of any kind are provided for a class type (struct, class, or union), the compiler will always declare a default constructor as an inline public member of its class

# Memory

The order in which the base class subobjects are allocated in the most derived object ([intro.object]) is unspecified.

# virtual base classes

First, and only for the constructor of the most derived class ([intro.object]), virtual base classes are initialized in the order they appear on a depth-first left-to-right traversal of the directed acyclic graph of base classes, where “left-to-right” is the order of appearance of the base classes in the derived class base-specifier-list.

# copy constructors are not inhereted

CWG issue 2356: Base class copy and move constructors should not be inherited.

[over.match.funcs]/p9 now says:

A constructor inherited from class type C ([class.inhctor.init]) that has a first parameter of type “reference to cv1 P” (including such a constructor instantiated from a template) is excluded from the set of candidate functions when constructing an object of type cv2 D if the argument list has exactly one argument and C is reference-related to P and P is reference-related to D.


