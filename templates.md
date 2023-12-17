# Misc

extern templates can really save a lot of compilation time


## static templated variables
 template <class T>
struct B
{
    static T v;
}; 

template <typename T>
T B<T>::v = 0;  --each template instantiation will have its own static variable

## union templates

unions can also be templated
```
   template<typename _Up, bool = is_trivially_destructible_v<_Up>>
    union _Storage
    {
```

## template template

Remeber!

```
template <template < class...> class A,   class B>
struct test1;
using test1_short = test1<std::tuple <- /*class tempalte*/, std::tuple<int,int,int> /*just class!*/>;
```

## comma operator

int p = (0,0,2,1);
p=1!!!!!

----------------
# Debugging

##  print  type name by compiler

One of the neat tricks you can use is to declare a class template, but not implement it. Whenever you want to check for a specific type, you can try to instantiate that template, and the compiler will report an error specifying exactly which type you passed

  ```
  template <class T> test;
  test<decltype(...)>();
  ```

  error: invalid use of incomplete type
  'class test;std::string&>'

# Forwarding refrences

Be carefull with those as the the rules for deducing the type of T may be tricky
template <class T>
void f(T&& v) {
}

int v; f(v); //here T would be int&
int& v; f(v); //here T would be STILL int&, because fundamently it is, however we cannot know if people are passing refrences to us


# Template parameters

## friends

template <class D> class C { friend D; }

## Default template paramter

 in C++ defaulted template parameters are not part of the signature, so multiple versions of the function will look like duplicate versions of the same function to the compiler, resulting in a compile error.

```
template<class Msg, typename = std::enable_if_t<AnyOfType<Msg, SupportedMessages>::value>>
bool handle(DepthUpdate<Msg> const& update);
```

However, it turns out that when dealing with non-type template parameters then default template parameters are part of the signature so we can work around this by changing the type of template parameter accepted.  This will now compile but as this is an integer value then we must consider that integers can legitimately occur in non-type template parameters.

```
template<class Msg, std::enable_if_t<AnyOfType<Msg, SupportedMessages>::value, int> = 0> bool handle(DepthUpdate<Msg> const& update);
```

----------------------

default template argument - typename = std::enable_if_t<std::is_integral<Integer>::value>
    default template arguments are not part of function template's signature
    default template arugments in partial specialization get copied from the primary tempalte if not set!

## non-type template paramter requirnments

A non-type template-parameter shall have one of the following (optionally cv-qualified) types:
(4.1) — integral or enumeration type,
(4.2) — pointer to object or pointer to function,
(4.3) — lvalue reference to object or lvalue reference to function,
(4.4) — pointer to member,
(4.5) — std::nullptr_t.

## auto template parameter

it helps to deduce types for non-type template parameters as before it had to be like <class T, T n> for non-type template parameters

```
  template <auto v> struct S;                 // type of v is deduced
  template <auto x> constexpr auto constant = x;
  // List of heterogeneous constant values
  // same as template <auto v1, auto v2, auto v3, ...>
  template <auto... vs> struct value_list { };
```

```
template<std::size_t SIZE>
class Foo {};

template <template<auto> class T, auto K>
auto extractSize(const T<K>&) {
    return K;
}

int main() {
    Foo<6> f1;
    Foo<13> f2;
    std::cout << extractSize(f1) << std::endl;
    std::cout << extractSize(f2) << std::endl;
}

```


```
template <auto... vs> struct HeterogenousValueList {};
using MyList1 = HeterogenousValueList<42, 'X'>;
```

## Template paramter as function pointer

```
typedef void (*delete_int_t)(int*);
void delete_int(int* p) { delete p; }
SmartPtr<int, delete_int_t> p(new int(42), delete_int);

A template instantiation is also a function and can be used in the same way:
template <typename T> void delete_T(T* p) { delete p; }
SmartPtr<int, delete_int_t> p(new int(42), delete_T<int>);
```


# Class templates

## Misc

* struct can be declared inside template parameter itself as incomplete types

```
template <class T> struct A {}; A<struct t1> t;
```

* class template shortcuts

```
template <typename T>
struct iterator
{
   iterator& oprerator++(); // here can just use iterator without putting T in here   
}
```

# Specializations

## std specialization

Unless explicitly prohibited, a program may add a template specialization for any standard library class template to namespace std provided that
(2.1) the added declaration depends on at least one program-defined type, and
(2.2) the specialization meets the standard library requirements for the original template


in partial specialization, default arguments cannot appear in the argument list

## Specialization by function type so that its return type and signature can be taken out

class specializations can have parameters function types Result(Args...)

```
template <class Sig, class F> class memoize_helper;
template <class Result, class... Args, class F>
class memoize_helper<Result(Args...), F>
{}

int f() {return 1;}
  memoize_helper<decltype(f), factory> m;

```

## Specialization by function pointer

```
template <class Result, class... Args>
class memoize_helper<Result (*)(Args...)> 
```

## Specifialization by member function

```
template<class B, class MT>
    struct invoke_impl<MT B::*>

struct A { int fun() const&; };
 
template<typename>
struct PM_traits {};
 
template<class T, class U>
struct PM_traits<U T::*> { using member_type = U; };

using T = PM_traits<decltype(&A::fun)>::member_type; // T is int() const&
```

## Fetching member function signature

```
template <class R, class C, class... Args>
class memoize_helper<R (C::*)(Args...)> {
public:
  void operator()(C &c, R (C::*mf)(Args...)) { (c.*mf)(100); }
};
```

## Most advanced specialization of class function members

```
template <auto> class memoize_helper2 {};
template <class R, class C, class... Args, R (C::*mf)(Args...)>
class memoize_helper2<mf> {
public:
  void whoami() { std::cout << "\n Bingo! \n"; }
  void operator()(C &c) { (c.*mf)(100); }
};

 memoize_helper2<&Foo::p> m;
```

## Specialization by another template

template specifalizing by another template are also possible!

```
template<class T>
class Foo;

template<class T>
class Foo<Bar<T>>
{
     /* Your implementation of this partial specialization. */
};
```

# Function templates

*  std function template specialization has been disabled since C++20. Only std classed may be specialized - > only if they depend on user-defined type

## Convert any callable to function pointer

```
template <typename... t, typename callable>
auto wrap_closure(callable const &) -> void(*)(t..., void*)
{
    return +[](t... data, void* userdata) {
        (*static_cast<callable*>(userdata))(data...);
    };
}
```

## Function overloading

 function template specialisations are not overloading(they still take part in "function overloading", but not overload between specializations). Basically during function overloading, best match is been looked for and "overloading" for the template candidates only consider "base templates". Once that base template has been selected, its specialisations will be considered for the best match - which would be exact match as templates don't allow type promotion.

Overload resolution does not take into account explicit specializations of function templates. Only after overload resolution has chosen a function template will any explicit specializations be considered


### rules for functions overloading:

Nontemplate functions are first-class citizens. A plain old nontemplate function that matches the parameter types as well as any function template will be selected over an otherwise-just-as-good function template.

If there are no first-class citizens to choose from that are at least as good, then function base templates as the second-class citizens get consulted next. Which function base template gets selected depends on which matches best and is the "most specialized" (important note: this use of "specialized" oddly enough has nothing to do with template specializations; it's just an unfortunate colloquialism) according to a set of fairly arcane rules:

  * If it's clear that there's one "most specialized" function base template, that one gets used. If that base template happens to be specialized for the types being used, the specialization will get used, otherwise the base template instantiated with the correct types will be used.

  * Else if there's a tie for the "most specialized" function base template, the call is ambiguous because the compiler can't decide which is a better match. The programmer will have to do something to qualify the call and say which one is wanted.

  * Else if there's no function base template that can be made to match, the call is bad and the programmer will have to fix the code.

```
template<class T> // (a) same old base template as before 
void f( T );

template<>        // (c) explicit specialization, this time of (a)
void f<>(int*);

template<class T> // (b) a second base template, overloads (a) 
void f( T* );

// ...

int *p; 
f( p );           // calls (b)! overload resolution ignores 
                  // specializations and operates on the base 
                  // function templates only

```

If this surprises you, you're not alone; it has surprised a lot of experts in its time. The key to understanding this is simple, and here it is: Specializations don't overload.


# Tricks

- For clarity, we will use an explicit return type, but we could use auto. Using an explicit return type has the advantage of generating a compilation error if your type conversions are incorrect.

-  variadic functions

```
 int sum() { return 0; }
template<typename T, typename... Args>
T sum(T a, Args... args) { return a + sum(args...); }
```

In C++14 you can also use auto sum(T a, Args... args) in order to get sum of mixed type

# Concepts

## to be used with expressions (combined with SFINAE)

```
    template <class F>
    bool consume(F &&f) requires(
        std::is_void_v<decltype((std::forward<F>(f)(std::declval<T>()),
                                 void()))>) {
```                                  

## inside if constexpr

```
template<class T>
constexpr auto to_address(const T& p) noexcept
{
    if constexpr (requires{ std::pointer_traits<T>::to_address(p); })
        return std::pointer_traits<T>::to_address(p);
    else
        return std::to_address(p.operator->());
}
```

## requires

```
template <typename T>
requires Addable<T> // requires-clause, not requires-expression
    T add(T a, T b) {
  return a + b;
}
```

## requires requires

infamous `requires requires`. First `requires` is requires-clause,
second one is requires-expression. Useful if you don't want to introduce
new concept.

```
template <typename T>
requires requires(T a, T b) { a + b; }
class Foo {
}
```

## Concepts vs SFINAE


They are not equivalent. Concepts can appear in more places and are partially ordered by subsumption. Some examples:

### Concept subsumption may be used to rank overloads. With SFINAE, this is an error:

```
template <typename T>
auto overload(T) -> std::enable_if_t<std::is_copy_constructible_v<T>>;
template <typename T>
auto overload(T) -> std::enable_if_t<std::is_move_constructible_v<T>>;

void test() {
  overload(1); // error: ambiguous
}
With concepts, this works:

void overload(std::copy_constructible auto);
void overload(std::move_constructible auto);

void test() {
  overload(1);
}
```

### Similarly, concept subsumption may be used to rank partial specializations.

### Concepts are allowed on non-template member functions, so they can constrain special member functions.

Since a copy constructor is not a template, SFINAE never applies. When one needs conditional behavior before concepts (e.g. trivial copy constructor if the template argument of the class template is itself trivial), one had to conditionally introduce different base classes.

### Concepts can constrain deduction.

One can statically assert that a type returned satisfies your requirements without asserting the precise type.

std::integral auto i = 1;

### Concepts can be used in abbreviated function templates.

void f(std::integral auto);


# Constexpr

## no undefined behaviour in constexpr

Evaluation of a constant expression ([expr.const]) never exhibits behavior explicitly specified as undefined in [intro] through [cpp]. — end note]

## Allocations are possible

C++20 lays foundation for constexpr containers. First, it allows constexpr and even virtual constexpr destructors for literal types(types that can be used as a constexpr variable). Second, it allows calls to std::allocator<T>::allocate() and new-expression which results in a call to one of the global operator new if allocated storage is deallocated at compile time. That is, memory can be allocated at compile-time but it must be freed at compile-time also. This creates a bit of friction if final data has to be used at run-time. 
"this means that calling something like **new A** is fine, but if got a custom allocator override than it would not work"

Since C++20, you can use new operator in constexpr expressions in the condition that you only use a replaceable global allocation function (it means that you don't use a placement new or user-defined allocation function) and that you deallocate the data in the same expression

## Storing the data from constexpr

There’s no choice but to store it in some non-allocating container like std::array and get compile-time value twice: first, to get its size, and second, to actually copy it(thanks to arthur-odwyer, beached and luke from cpplang slack for explaining this to me):

``` 
constexpr auto get_str()
{
    std::string s1{"hello "};
    std::string s2{"world"};
    std::string s3 = s1 + s2;
    return s3;
}

constexpr auto get_array()
{
    constexpr auto N = get_str().size();
    std::array<char, N> arr{};
    std::copy_n(get_str().data(), N, std::begin(arr));
    return arr;
}

static_assert(!get_str().empty());

// error because it holds data allocated at compile-time
constexpr auto str = get_str();

// OK, string is stored in std::array<char>
constexpr auto result = get_array();

```


## Try-catch blocks in constexpr functions

It is possible to throw and catch exceptions in constexpr func

```
constexpr int f(int x) {
  try {
    int r = x + 1;
    if (r == 100)
      throw 4;
    return r;
  } catch (...) {
    return 0;
  }
}
```

### is_constant_evauluated

this function can check if the current context in constexpr, but it is not realible at least as of gcc11 and clang14

## Lambdas

lambdas can be declared as template variables! but not constexpr until c++20!

```
template <typename T> auto v = []() { T t1; return 0; };
auto v1 = v<int>();
```

# Metaprogamming 


## Metaprogramming disadvantages

why template programming aka duck typing is bad
 - massive error messages if something goes wrong
 - latent bugs - say if later some client code instantiates template with not expected types we got an issue
 - they don't state intent

## Types of metaprogramms

-compile-time numeric calcualations
-self-adapting code given input types & constexpr variables
-type manipulations
-type-dependement code
-code generation driven by type collecton
-domain languge specific tools


## integral_constant

```
template< class T, T v >
struct integral_constant;
```

Specializations
Two typedefs for the common case where T is bool are provided:

```
true_type	std::integral_constant<bool, true>
false_type	std::integral_constant<bool, false>
```

## std::conjunction, just returns the first type which is false, or last one if everythong got T::value == true 

```
template<class...> struct conjunction : std::true_type {};
template<class B1> struct conjunction<B1> : B1 {};
template<class B1, class... Bn>
struct conjunction<B1, Bn...>
    : std::conditional_t<bool(B1::value), conjunction<Bn...>, B1> {};

```

##  std::common_type

Determines the common type among all types T..., that is the type all T... can be implicitly converted to. If such a type exists (as determined according to the rules below), the member type names that type. Otherwise, there is no member type

## decay

(1) remove reference
(2) if array converts to pointer
(3) if function converts to func pointer
{4} removes const

## decval

decval - Converts any type T to a reference type, making it possible to use member functions in decltype expressions without the need to go through constructors
Note that std::declval can only be used in unevaluated contexts and is not required to be defined; it is an error to evaluate an expression that contains this function. Formally, the program is ill-formed if this function is odr-used.

std::remove_reference<decltype(declval<DeclvalStruct>())>::type

## decltype

no deduction inside decltype!
template<typename T> void f(decltype(*std::declval<T>()) arg);
not GONNA WORK!


f the argument is any other expression of type T, and
a) if the value category of expression is xvalue, then decltype yields T&&;
b) if the value category of expression is lvalue, then decltype yields T&;
c) if the value category of expression is prvalue, then decltype yields T.


## decltype(auto)

decltype(auto) - is good for forwarding functions as if we want to deduce the return type for free and at the same time pass EXACT values.

becuase forwarding refrences have the name they must be used with std::forward otherwise theyll be always treated like lvalues!
To put it another way, the compiler can figure out which forward to call based on whether the input is an lvalue or rvalue, but that is unrelated to whether you want the desired output to be an lvalue or rvalue. The compiler knows the type of the input, but you need to explicitly tell it the type of the output, because they are not the same! Like complier could figure it out, BUT what if you want first to use that forwarding reference as lvalue first and then move from it in the end!
like

## void_t

void_t It can also be used to detect validity of an expression rather than just a type. the idea is to have a base template and a specialized one. If specialized one is ill-formed than SFINA excludes it

```

void_t!
template< class... >
using void_t = void;
(since C++17)
This metafunction is used in template metaprogramming to detect ill-formed types in SFINAE context

// primary template handles types that have no nested ::type member:
template< class, class = std::void_t<> >
struct has_type_member : std::false_type { };
 
// specialization recognizes types that do have a nested ::type member:
template< class T >
struct has_type_member<T, std::void_t<typename T::type>> : std::true_type { };

OR

template< class T >
struct has_type_member<T, std::void_t<decltype(T::member)> : std::true_type { };

```

## std::type_identity

std::type_identity can be used to establish non-deduced contexts in template argument deduction.

```
template<class T>
T foo(T a, T b) { return a + b; }
 
template<class T>
T bar(T a, std::type_identity_t<T> b) { return a + b; }
 
int main()
{
    // foo(4.2, 1); // error, deduced conflicting types for 'T'
    std::cout << bar(4.2, 1) << '\n';  // OK, calls bar<double>
}
```

In the following cases, the types, templates, and non-type values that are used to compose P do not participate in template argument deduction, but instead use the template arguments that were either deduced elsewhere or explicitly specified. If a template parameter is used only in non-deduced contexts and is not explicitly specified, template argument deduction fails.


## std::index_sequence

template <class... T> is not iqual index_sequence<I...>!
index_sequence is template 

```
<class... T> index_sequence 
```

so basically <class... T> can be deduced from index sequence type



he class template std::integer_sequence represents a compile-time sequence of integers. When used as an argument to a function template, the parameter pack Ints can be deduced and used in pack expansion.

  size_t... SZ - this can be made with just std::make_index_sequence<N>

```
  template <class IS, class... T>
    struct indexed_types;
  template <size_t... SZ, class... T>  <---SUPER TRICK because SZ is concrete type, it possible to have another variadic pack after it!
    struct indexed_types<std::index_sequence<SZ...>,T...> : IndexedType<SZ, T>...
  {
  };   

```


### index_sequence standard utils

complier internal functions to quickly make index sequence from a number - very fast

```
template<class T, T N>
using make_integer_sequence = std::integer_sequence<T, /* a sequence 0, 1, 2, ..., N-1 */ >;
template<std::size_t N>
using make_index_sequence = std::make_integer_sequence<std::size_t, N>;
```

A helper alias template std::index_sequence_for is defined to convert any type parameter pack into an index sequence of the same length:

```
template<class... T>
using index_sequence_for = std::make_index_sequence<sizeof...(T)>;
```


### array into tuple
```
// convert array into a tuple
template<typename Array, std::size_t... I>
auto a2t_impl(const Array& a, std::index_sequence<I...>)
{
    return std::make_tuple(a[I]...);
}
 
template<typename T, std::size_t N, typename Indices = std::make_index_sequence<N>>
auto a2t(const std::array<T, N>& a)
{
    return a2t_impl(a, Indices{});
}
```



### tuple printer

```
// pretty-print a tuple
template<class Ch, class Tr, class Tuple, std::size_t... Is>
void print_tuple_impl(std::basic_ostream<Ch,Tr>& os,
                      const Tuple& t,
                      std::index_sequence<Is...>)
{
    ((os << (Is == 0? "" : ", ") << std::get<Is>(t)), ...);
}
 
template<class Ch, class Tr, class... Args>
auto& operator<<(std::basic_ostream<Ch, Tr>& os,
                 const std::tuple<Args...>& t)
{
    os << "(";
    print_tuple_impl(os, t, std::index_sequence_for<Args...>{});
    return os << ")";
}

```

## Example 

### std::less

```
template<>
    struct less<void>
    {
      template <typename _Tp, typename _Up>
	constexpr auto
	operator()(_Tp&& __t, _Up&& __u) const
	noexcept(noexcept(std::forward<_Tp>(__t) < std::forward<_Up>(__u)))
	-> decltype(std::forward<_Tp>(__t) < std::forward<_Up>(__u))
```

### base defining all basic operators + specializations

```
  // Defines move construction + assignment as either defaulted or deleted.
  template <typename _Tp, typename _Dp,
	    bool = is_move_constructible<_Dp>::value,
	    bool = is_move_assignable<_Dp>::value>
    struct __uniq_ptr_data : __uniq_ptr_impl<_Tp, _Dp>
    {
      using __uniq_ptr_impl<_Tp, _Dp>::__uniq_ptr_impl;
      __uniq_ptr_data(__uniq_ptr_data&&) = default;
      __uniq_ptr_data& operator=(__uniq_ptr_data&&) = default;
    };

  template <typename _Tp, typename _Dp>
    struct __uniq_ptr_data<_Tp, _Dp, true, false> : __uniq_ptr_impl<_Tp, _Dp>
    {
      using __uniq_ptr_impl<_Tp, _Dp>::__uniq_ptr_impl;
      __uniq_ptr_data(__uniq_ptr_data&&) = default;
      __uniq_ptr_data& operator=(__uniq_ptr_data&&) = delete;
    };
```

### Partial binder

```
  //partial binder!
  template <template <class...> class F, class... TS>
  struct part_call
  {
    template <class... US>
    using type = typename F<TS...,US...>::type;
  }

  template <template <class...> class F, class... TP>
  constexpr bool find_if(type_list<TS...> tp>
  {
    bool bp[] = {F<TS>::value...} # calls part_call<....>::type<TS>
    ....    
  }

  find_if<part_call<is_base_of, std::exception>::type>(tp);

```

###  template tempplate class specalization

template<class L> struct mp_size_impl;
template<template<class...> class L, class... T> struct mp_size_impl<L<T...>>
{  using type = std::integral_constant<std::size_t, sizeof...(T)>;
template<class L> using mp_size = typename mp_size_impl<L>::type;



## SFINAE

code parsing -> instantiation -> deduction -> substitution -> evaluation!

IMPORTANT ---> **SFINAE - is not only substitution failure it is also deduction failure**

//this works because vector T's get deduced properly and

```
 void deduce(const std::vector<T>& vec, typename Identity<T>::type v) 
```

 fails to be deduced! 

```
void deduce(const std::vector<T>& vec, typename Identity<T>::type v)
```


Template argument deduction takes place after the function template name lookup (which may involve argument-dependent lookup) and before template argument substitution (which may involve SFINAE) and overload resolution.

Alias templates are not deduced , except in class template argument deduction (since C++20)

All the expressions won't lead to a SFINAE. A broad rule would be to say that all the substitutions out of the function/methods body are "safes"

Substitution proceeds in lexical order and stops when a failure is encountered.

### immediate context

Only the failures in the types and expressions in the immediate context of the function type or its template parameter types or its explicit specifier (since C++20) are SFINAE errors.

If the evaluation of a substituted type/expression causes a side-effect such as instantiation of some template specialization, generation of an implicitly-defined member function, etc, errors in those side-effects are treated as hard errors. For example subsitutung T in typename T::type to some class A<O> may trigger instantiation of class A<O>, but it may fail...Hence it would be a hard error!

So the mental model I use is that substitution needs to do a "preparation" step first to generate types and members, which might cause hard errors, but once we have all the necessary generation done, any further invalid uses are not errors


--------------------

If a substitution results in an invalid type or expression, type deduction fails. An invalid type or expression is one that would be ill-formed, with a diagnostic required, if written in the same context using the substituted arguments.

[Note 5: If no diagnostic is required, the program is still ill-formed. Access checking is done as part of the substitution process. — end note]

 **Invalid types and expressions can result in a deduction failure only in the immediate context of the deduction substitution loci.**

[Note 6: The substitution into types and expressions can result in effects such as the instantiation of class template specializations and/or function template specializations, the generation of implicitly-defined functions, etc. Such effects are not in the “immediate context” and can result in the program being ill-formed. — end note]


Deduction and substitution also occur while determining whether a specialization of a class or variable (since C++14) template is generated by some partial specialization or the primary template. Compilers do not treat a substitution failure as a hard-error during such determination, but ignore the corresponding partial specialization declaration instead, as if in the overload resolution involving function templates.


Example where deduction becomes hard error because OnlyInt<X1>::type yields another template which fails and it can only work in the immediate context !

```
template <class T, typename = std::enable_if_t<std::is_same_v<T, double>>>
struct AnotherStructDouble {
  using type = T;
};


template <class T, typename = std::enable_if_t<std::is_same_v<T, int>>>
struct OnlyInt {
  using type = typename AnotherStructDouble<T>::type;
};


template <class X1, class X2> struct Element {};
template <class X1, class X2>
std::enable_if_t<std::is_same_v<X1, int>> f(const Element<X1, X2> &v, X1 p,
                                            typename OnlyInt<X1>::type d = {}) {
  std::cout << "\n f::int \n";
}
```

A way to fix it is to empose SFINAE in the immediate context:

```
template <class T, typename = std::enable_if_t<std::is_same_v<T, int>>,
          typename = typename AnotherStructDouble<T>::type>
struct OnlyInt {
  using type = typename AnotherStructDouble<T>::type;
};
```


### Super example with diffrent types of SFINAE

```
template <class X1, class X2> struct Element {};
template <class X1, class X2>
std::enable_if_t<std::is_same_v<X1, int>> f(const Element<X1, X2> &v, X1 p) {
  std::cout << "\n f::int \n";
}

template <class X1, class X2>
auto f(const Element<X1, X2> &v, X1 p)
    -> decltype(std::enable_if_t<std::is_same_v<X1, double>>()) {
  std::cout << "\n f::double \n";
}

template <class X1, class X2,
          typename = std::enable_if_t<std::is_same_v<X1, long>>>
auto f(const Element<X1, X2> &v, X1 p) {
  std::cout << "\n f::long \n";
}

int main() {
  f(Element<int, int>{}, int{});
  f(Element<double, int>{}, double{});
  f(Element<long, int>{}, long{});
}

```

### Lifting types to values

```
template <typename T> struct type {};
constepxr type<T*>{}add_pointer(type<T> v) { {}; }

template <typename T>  constexpr std::true_type is_pointer(type<T*> v) { return {}; }
template <typename T>  constexpr std::false_type is_pointer(type<T> v) { return {}; }

make_tuple(type<int>, type<double>)
```

### Expression SFINAE

```
template <class C, class F>
auto test(C c, F f) -> decltype((void)(c.*f)(), void())
{ std::cout << "Reference overload called\n"; }

template <class C, class F>
auto test(C c, F f) -> decltype((void)((c->*f)()), void())
{ std::cout << "Pointer overload called\n"; }


Another example

```

### Examples

#### old-school eclipsis trick!

```
  template<typename _Tp, typename... _Args>
    struct __construct_helper
    {
      template<typename _Alloc2,
        typename = decltype(std::declval<_Alloc2*>()->construct(
          std::declval<_Tp*>(), std::declval<_Args>()...))>
        static true_type __test(int); // declares a function!

      template<typename>
        static false_type __test(...); // declares a function!

      using type = decltype(__test<_Alloc>(0));
    };

      template<typename _Tp, typename... _Args>
    using __has_construct
      = typename __construct_helper<_Tp, _Args...>::type
```


### and

   template <typename... T> struct _and;
    template <> struct _and<> : std::true_type {};
    template <typename T1> struct _and<T1> : T1 {};
    template <typename T1, typename T2> struct _and<T1, T2> : std::conditional<T1::value, T2, T1>::type  {};
    template <typename T1, typename T2, typename... Tn> struct _and<T1, T2, Tn...> : _and<T1, _and<T2, Tn...>> {};



#### is_constuctible

```
  template <typename T,typename,typename... Args> struct mine_is_constructible : std::false_type {};
    template <typename T, typename... Args> struct mine_is_constructible<T, void_t<decltype(T(std::declval<Args>()...))>, Args...> : std::true_type {};
     template <typename T, typename... Args> using mine_is_constructible_v = mine_is_constructible<T, void_t<T>, Args...>;
```

#### expression template

template<typename T, typename Cont= std::vector<T> >
class MyVector
{
  // assignment operator for MyVector of different type
  template<typename T2, typename R2>
  MyVector& operator=(const MyVector<T2, R2>& other){
    assert(size() == other.size());
    for (std::size_t i = 0; i < cont.size(); ++i) cont[i] = other[i];
    return *this;
  }

template<typename T, typename Op1 , typename Op2>
class MyVectorAdd{
  const Op1& op1;
  const Op2& op2;

public:
  MyVectorAdd(const Op1& a, const Op2& b): op1(a), op2(b){}

  T operator[](const std::size_t i) const{ 
    return op1[i] + op2[i]; 
  }

  std::size_t size() const{ 
    return op1.size(); 
  }
};

// function template for the + operator
template<typename T, typename R1, typename R2>
MyVector<T, MyVectorAdd<T, R1, R2> >
operator+ (const MyVector<T, R1>& a, const MyVector<T, R2>& b){
  return MyVector<T, MyVectorAdd<T, R1, R2> >(MyVectorAdd<T, R1, R2 >(a.data(), b.data()));
}

}
#### make tuple

```
template<class... _Types> inline
	constexpr tuple<typename _Unrefwrap<_Types>::type...>
		make_tuple(_Types&&... _Args)
	{	// make tuple from elements
	typedef tuple<typename _Unrefwrap<_Types>::type...> _Ttype;
	return (_Ttype(_STD forward<_Types>(_Args)...));
	}
```

#### array rolling 

template<typename T>
class is_class 
{ 0, ( (void) bar(std::forward<Args>(args)), 0) ... };
  |       |       |                        |     |
  |       |       |                        |     --- pack expand the whole thing 
  |       |       |                        |   
  |       |       --perfect forwarding     --- comma operator
  |       |
  |       -- cast to void to ensure that regardless of bar()'s return type
  |          the built-in comma operator is used rather than an overloaded one
  |
  ---ensure that the array has at least one element so that we don't try to make an
     illegal 0-length array when args is empty

#### return void type from function

```
  template<class _FwdIt,
        class _Ty> inline
        typename _Fill_memset_is_safe_helper<_FwdIt, _Ty>::type
        _Fill_memset_is_safe(const _FwdIt&, const _Ty&)
    {	// type deduction for _Fill_memset_is_safe_helper
        return {};
    }

```

trick is that {} will force default construction! but not for void in which case SFINAE kicks in!

#### 3 rolling the array!

```
template<size_t... _Indices,
	class... _LockN> inline
	void _Lock_from_locks(const int _Target, index_sequence<_Indices...>, _LockN&... _LkN)
	{	// lock _LkN[_Target]
	int _Ignored[] = { ((static_cast<int>(_Indices) == _Target
		? (void)_LkN.lock()
		: void()), 0)... };
	(void)_Ignored;
	}
```

#### is_iterable

```
    template <typename C, 
        typename = void_t<>> 
        struct is_iterable 
        : std::false_type {}; 

        template <typename C> 
        struct is_iterable< 
        C, void_t<decltype(*begin(std::declval<C>())), 
        decltype(end(std::declval<C>()))>> 
        : std::true_type{};
```

# Instantiation

 template non-dependent symbol lookup just associats symbols with whatever it
 encounter so far! even it there is better definition later or after template instantiation. 

 ```
temlate <class T> struct B : D<T> { g() // global g() will be called, not one from D<T>! }; 
temlate <class T> struct B : D<T> { FromDT v; //won't compile because FromDT is defined in D<T>; D<T>::FromDt v will compile }; 
 ```


 For a non-dependent name used in a template definition, unqualified name lookup takes place when the template definition is examined. The binding to the declarations made at that point is not affected by declarations visible at the point of instantiation. For a dependent name used in a template definition, the lookup is postponed until the template arguments are known, at which time ADL examines function declarations with external linkage (until C++11) that are visible from the template definition context as well as in the template instantiation context, while non-ADL lookup only examines function declarations with external linkage (until C++11) that are visible from the template definition context (in other words, adding a new function declaration after template definition does not make it visible except via ADL). The behavior is undefined if there is a better match with external linkage in the namespaces examined by the ADL lookup, declared in some other translation unit, or if the lookup would have been ambiguous if those translation units were examined. In any case, if a base class depends on a template parameter, its scope is not examined by unqualified name lookup (neither at the point of definition nor at the point of instantiation).



# Deduction

Type deduction does not consider implicit conversions (other than type adjustments listed above): that's the job for overload resolution, which happens later. However, if deduction succeeds for all parameters that participate in template argument deduction, and all template arguments that aren't deduced are explicitly specified or defaulted, then the remaining function parameters are compared with the corresponding function arguments. For each remaining parameter P with a type that was non-dependent before substitution of any explicitly-specified template arguments, if the corresponding argument A cannot be implicitly converted to P, deduction fails.

-----------------------------

decltype is explicitly excluded from template argument deduction


Template argument deduction attempts to determine template arguments (types for type template parameters Ti, templates for template template parameters TTi, and values for non-type template parameters Ii), which can be substituted into each parameter P to produce the type deduced A, which is the same as the type of the argument A, after adjustments listed below

MENTAL MODEL FOR DEDUCTION IS REALLY FIND SUCH "T" WHICH IF SUBSTITUED WOULD BE EQUAL TO THE GIVEN ARGUMENT!

If there are multiple parameters, each P/A pair is deduced separately and the deduced template arguments are then combined. If deduction fails or is ambiguous for any P/A pair or if different pairs yield different deduced template arguments, or if any template argument remains neither deduced nor explicitly specified, compilation fails.

Before deduction begins, the following adjustments to P and A are made:

1) If P is not a reference type,
a) if A is an array type, A is replaced by the pointer type obtained from array-to-pointer conversion;
b) otherwise, if A is a function type, A is replaced by the pointer type obtained from function-to-pointer conversion;
c) otherwise, if A is a cv-qualified type, the top-level cv-qualifiers are ignored for deduction:
template<class T>
void f(T);
 
int a[3];
f(a); // P = T, A = int[3], adjusted to int*: deduced T = int*
 
void b(int);
f(b); // P = T, A = void(int), adjusted to void(*)(int): deduced T = void(*)(int)
 
const int c = 13;
f(c); // P = T, A = const int, adjusted to int: deduced T = int
2) If P is a cv-qualified type, the top-level cv-qualifiers are ignored for deduction.
3) If P is a reference type, the referenced type is used for deduction.
4) If P is an rvalue reference to a cv-unqualified template parameter (so-called forwarding reference), and the corresponding function call argument is an lvalue, the type lvalue reference to A is used in place of A for deduction (Note: this is the basis for the action of std::forward Note: in class template argument deduction, template parameter of a class template is never a forwarding reference(since C++17)):
template<class T>
int f(T&&);       // P is an rvalue reference to cv-unqualified T (forwarding reference)
 
template<class T>
int g(const T&&); // P is an rvalue reference to cv-qualified T (not special)
 
int main()
{
    int i;
    int n1 = f(i); // argument is lvalue: calls f<int&>(int&) (special case)
    int n2 = f(0); // argument is not lvalue: calls f<int>(int&&)
 
//  int n3 = g(i); // error: deduces to g<int>(const int&&), which
                   // cannot bind an rvalue reference to an lvalue
}
After these transformations, the deduction processes as described below (cf. section Deduction from a type) and attempts to find such template arguments that would make the deduced A (that is, P after adjustments listed above and the substitution of the deduced template parameters) identical to the transformed A, that is A after the adjustments listed above.

If the usual deduction from P and A fails, the following alternatives are additionally considered:

1) If P is a reference type, the deduced A (i.e., the type referred to by the reference) can be more cv-qualified than the transformed A:
template<typename T>
void f(const T& t);
 
bool a = false;
f(a); // P = const T&, adjusted to const T, A = bool:
      // deduced T = bool, deduced A = const bool
      // deduced A is more cv-qualified than A


2) The transformed A can be another pointer or pointer to member type that can be converted to the deduced A via a qualification conversions or a function pointer conversion(since C++17):
template<typename T>
void f(const T*);
 
int* p;
f(p); // P = const T*, A = int*:
      // deduced T = int, deduced A = const int*
      // qualification conversion applies (from int* to const int*)
      
# Substitution

Function template parameters are substituted (replaced by template arguments) twice:
 -explicitly specified template arguments are substituted before template argument deduction
 - deduced arguments and the arguments obtained from the defaults are substituted after template argument deduction

# Patterns

## template adaptor

```
template <typename T>
struct DeleteMyHeap : public DeleteHeap<T, MyHeap> {
    using DeleteHeap<T, MyHeap>::DeleteHeap;
};
```


# paramter is function pointer type
```
template<class T>
int f(T(*p)(T));
 
int g(int);
int g(char);
 
f(g); // P = T(*)(T), A = overload set
      // P = T(*)(T), A1 = int(int): deduced T = int
      // P = T(*)(T), A2 = int(char): fails to deduce T
      // only one overload works, deduction succeeds
```

## If P is not a reference type,
## if A is an array type, A is replaced by the pointer type obtained from array-to-pointer conversion;
## otherwise, if A is a function type, A is replaced by the pointer type obtained from function-to-pointer conversion;
## otherwise, if A is a cv-qualified type, the top-level cv-qualifiers are ignored for deduction:

```
template<class T>
void f(T);
 
int a[3];
f(a); // P = T, A = int[3], adjusted to int*: deduced T = int*
 
void b(int);
f(b); // P = T, A = void(int), adjusted to void(*)(int): deduced T = void(*)(int)
 
const int c = 13;
f(c); // P = T, A = const int, adjusted to int: deduced T = int
```

# Template instantiation

## Template aliases: alias tempalte  == original type

```
template <class T> using vecalias =  std::vector<int>;
is_same<std::vector, vecalias> == true!
```


## Templates are compiled (atleast) twice

Without Instantiation the template code itself is checked for syntax.
Eg: Any syntax errors such as ; etc.

At the time of instantiation(when the exact type is known), the template code is checked again to ensure all calls are valid for that particular type.
Eg: The template might in turn call to functions which might not be present for that particular type.

Also note that lookup for non-dependent names is done in the first phase, whereas lookup for names that depend on a template parameter is done in the second phase. The significance is that if you call sqrt(1), then sqrt needs to be declared before the template is defined. But if you call sqrt(t), where t is an instance of a type that's a template parameter, then sqrt needn't be visible until the template is instantiated.

##  evaluations without side-effects

decltype,sizeof,declval,alingof don't evaluate expression which means sife-effects may not leak if the overload candidates d gets removed by SFINAE!

# f(...)

Also, you may find the notion of the doubly universal template f(...) function curious—it takes any arguments of any type, even without the template, so why use the template? Of course, it is so a call with an explicitly specified type, like f<int>(), considers this function as a possible overload (remember that, by specifying the template parameter type, we also exclude all non-template functions from consideration



# Template template class

## template template classes can alos be specializead

```
template<typename T, typename S>
class SomeRandomClass
{
    //put something 
};

template<typename T>
class MyTemplateClass
{
    void DoSomething(T & t) {
       //...something
    }
};

template <>
template <typename T,typename S>
class MyTemplateClass <SomeRandomClass<T,S> >
{
    void DoSomething(SomeRandomClass<T,S>& t) { /* something */ }
};




    
