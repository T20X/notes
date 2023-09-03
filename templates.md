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

# Function templates

*  std function template specialization has been disabled since C++20. Only std classed may be specialized - > only if they depend on user-defined type


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


# Constexpr

## Allocations are possible

C++20 lays foundation for constexpr containers. First, it allows constexpr and even virtual constexpr destructors for literal types(types that can be used as a constexpr variable). Second, it allows calls to std::allocator<T>::allocate() and new-expression which results in a call to one of the global operator new if allocated storage is deallocated at compile time. That is, memory can be allocated at compile-time but it must be freed at compile-time also. This creates a bit of friction if final data has to be used at run-time. 

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

## std::index_sequence

template <class... T> is not iqual index_sequence<I...>!
index_sequence is template 

```
<class... T> index_sequence 
``````

so basically <class... T> can be deduced from index sequence type



he class template std::integer_sequence represents a compile-time sequence of integers. When used as an argument to a function template, the parameter pack Ints can be deduced and used in pack expansion.

  size_t... SZ - this can be made with just std::make_index_sequence<N>

```
  template <class IS, class... T>
    struct indexed_types;
  template <size_t... SZ, class... T>  <---SUPER TRICK because SZ is concrete type, it possible to have another variadic pack after it!
    struct indexed_types<std::index_sequence<SZ...>,T...> : IndexedType<IS, T>...
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
``````

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

## SFINAE

code parsing -> instantiation -> deduction -> substitution -> evaluation!

IMPORTANT ---> **SFINAE - is not only substitution failure it is also deduction failure**

Type deduction does not consider implicit conversions (other than type adjustments listed above): that's the job for overload resolution, which happens later. However, if deduction succeeds for all parameters that participate in template argument deduction, and all template arguments that aren't deduced are explicitly specified or defaulted, then the remaining function parameters are compared with the corresponding function arguments. For each remaining parameter P with a type that was non-dependent before substitution of any explicitly-specified template arguments, if the corresponding argument A cannot be implicitly converted to P, deduction fails.

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

## Lifting types to values

```
template <typename T> struct type {};
constepxr type<T*>{}add_pointer(type<T> v) { {}; }

template <typename T>  constexpr std::true_type is_pointer(type<T*> v) { return {}; }
template <typename T>  constexpr std::false_type is_pointer(type<T> v) { return {}; }

make_tuple(type<int>, type<double>)
```

## Expression SFINAE

```
template <class C, class F>
auto test(C c, F f) -> decltype((void)(c.*f)(), void())
{ std::cout << "Reference overload called\n"; }

template <class C, class F>
auto test(C c, F f) -> decltype((void)((c->*f)()), void())
{ std::cout << "Pointer overload called\n"; }


Another example

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

```


   
     --------------------
     template <typename T> class C {
    int x_;
    public:
    C(int x) : x_(x) {}
    friend C operator+(const C& lhs, const C& rhs) { defines non-template functions at the CLASS scope!
        return C(lhs.x_ + rhs.x_); 
    }

    static non-member functions can be generated in a similar manner, but the binary operators cannot be static functions—the standard explicitly forbids it



-------------

------------

------------
template <typename C,typename T = typename C::value_type>
std::unordered_map<T, unsigned int> 
count_occurrences(const C& collection)
-----
Function template parameters are substituted (replaced by template arguments) twice:
 -explicitly specified template arguments are substituted before template argument deduction
 - deduced arguments and the arguments obtained from the defaults are substituted after template argument deduction

--------------
decltype is explicitly excluded from template argument deduction
-----------------------

 default template argument - typename = std::enable_if_t<std::is_integral<Integer>::value>
     default template arguments are not part of function template's signature
     default template arugments in partial specialization get copied from the primary tempalte if not set!
--------------------------------------
. A substitution failure is any situation when the type or expression above would be ill-formed (with a required diagnostic), *IF WRITTEN* using the substituted argument
--------------------------------------
Only the failures in the types and expressions in the immediate context of the function type or its template parameter types or its explicit specifier (since C++20) are SFINAE errors.

If the evaluation of a substituted type/expression causes a side-effect such as instantiation of some template specialization, generation of an implicitly-defined member function, etc, errors in those side-effects are treated as hard errors. For example subsitutung T in typename T::type to some class A<O> may trigger instantiation of class A<O>, but it may fail...Hence it would be a hard error!

So the mental model I use is that substitution needs to do a "preparation" step first to generate types and members, which might cause hard errors, but once we have all the necessary generation done, any further invalid uses are not errors


--------------------------------------
. decval - Converts any type T to a reference type, making it possible to use member functions in decltype expressions without the need to go through constructors
--------------------------------------
. The compiler does overload resolution before it even looks at specialisations
--------------------------------------
Remeber!
template <template < class...> class A,   class B>
struct test1;
using test1_short = test1<std::tuple <- /*class tempalte*/, std::tuple<int,int,int> /*just class!*/>;

--------------------
For clarity, we will use an explicit return type, but we could use auto. Using an explicit return type has the advantage of generating a compilation error if your type conversions are incorrect.
-----------
The general behavior of the ... operator is to replicate the code fragment on its left for every type in the parameter pack
--------------
partial/full specializtion for templates must match EXACTLY, otherwise primary template
is used
-------------
The compiler does overload resolution before it even looks at specialisations
------------------------------------------------------------------
for explicit specializations, there is no argument deduction
----------------
4 type of templates
--------------
function templates
class templates
variable templates (since C++14)
alias templates
---------------------------------
can do specialization of a template template member!


template<class T> struct A { // enclosing class template
  template<class T2>
  struct B {}; // primary member template
  template<class T2>
  struct B<T2*> {}; // partial specialization of member template
};
 
template<>
template<class T2>
struct A<short>::B {}; // full specialization of primary member template
                       // (will ignore the partial)
-------------------

---------------------------------
in partial specialization, default arguments cannot appear in the argument list
---------------------------------

------------
conditional
-----------
template<bool B, class T, class F>
struct conditional { typedef T type; };
 
template<class T, class F>
struct conditional<false, T, F> { typedef F type; };


-----------------
SFINAE
------------------

(1) 

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

(2)
  template<class _FwdIt,
        class _Ty> inline
        typename _Fill_memset_is_safe_helper<_FwdIt, _Ty>::type
        _Fill_memset_is_safe(const _FwdIt&, const _Ty&)
    {	// type deduction for _Fill_memset_is_safe_helper
        return {};
    }

trick is that {} will force default construction! but not for void in which case SFINAE kicks in!

----------
declval
----------
returns rvalue ref even for expression which don't have def constructor!
std::remove_reference<decltype(declval<DeclvalStruct>())>::type

--------
eclipsis
--------
// this overload is always in the set of overloads
// ellipsis parameter has the lowest ranking for overload resolution
void test(...)

-----
decay
----
(1) remove reference
(2) if array converts to pointer
(3) if function converts to func pointer
{4} removes const


---
basics
---
no type deduction for class, alias and variable templates
no deduction, and specialization for alias templates
no partial specialization for function templates

---------
class template shortcuts
-----
template <typename T>
struct iterator
{
   iterator& oprerator++(); // here can just use iterator without putting T in here   
}

------------------
variadic functions
-----------------
int sum() { return 0; }
template<typename T, typename... Args>
T sum(T a, Args... args) { return a + sum(args...); }

In C++14 you can also use auto sum(T a, Args... args) in order to get sum of mixed type


-------
std::less
----
template< class T = void >
struct less;
since C++14 for automatic template parameter deduction
template<>
	struct less<void>
	{	// transparent functor for operator<
	typedef int is_transparent;

	template<class _Ty1,
		class _Ty2>
		constexpr auto operator()(_Ty1&& _Left, _Ty2&& _Right) const


----
decltype 
---
no deduction inside decltype!
template<typename T> void f(decltype(*std::declval<T>()) arg);
not GONNA WORK!

-----
-------
template <class... T> struct mp_list{};

 template<class A, template<class...> class B>
 using mp_rename = typename mp_rename_impl<A, B>::type;

IF this calls -> mp_rename<mp_list<double, int>, std::pair>
then A = mp_list<double, int>
    B = std::pair

---------
REMEMBER THNIS SPECIAIZATION TEQNICQE
--------
template<class L> struct mp_size_impl;
template<template<class...> class L, class... T> struct mp_size_impl<L<T...>>
{  using type = std::integral_constant<std::size_t, sizeof...(T)>;
template<class L> using mp_size = typename mp_size_impl<L>::type;


-------------------
template< bool B, class T, class F >
struct conditional;
(since C++11)
Provides member typedef type, which is defined as T if B is true at compile time, or as F if B is false.

template<class _Ux,
		enable_if_t<conjunction_v<conditional_t<is_array_v<_Ty>, _Can_array_delete<_Ux>, _Can_scalar_delete<_Ux>>,
			_SP_convertible<_Ux, _Ty>>, int> = 0>
		explicit shared_ptr(_Ux * _Px)

template<class... B>
struct conjunction;

	shared_ptr(const shared_ptr& _Other) noexcept
		{	// construct shared_ptr object that owns same resource as _Other
		this->_Copy_construct_from(_Other);
		}

	template<class _Ty2>
		void _Copy_construct_from(const shared_ptr<_Ty2>& _Other)
		{	// implement shared_ptr's (converting) copy ctor
		if (_Other._Rep)
			{

template<class _Ux>
		void _Setp(_Ux * _Px, false_type)
		{	// take ownership of _Px
		_TRY_BEGIN	// allocate control block and set
		_Set_ptr_rep_and_enable_shared(_Px, new _Ref_count<_Ux>(_Px));
		_CATCH_ALL	// allocation failed, delete resource
		delete _Px;
		_RERAISE;

	template<class _Ty0,
		class... _Types>
		friend shared_ptr<_Ty0> make_shared(_Types&&... _Args);


	// FUNCTION TEMPLATE allocate_shared
template<class _Ty,
	class _Alloc,
	class... _Types>
	_NODISCARD inline shared_ptr<_Ty> allocate_shared(const _Alloc& _Al_arg, _Types&&... _Args)
	{	// make a shared_ptr
	using _Refoa = _Ref_count_obj_alloc<_Ty, _Alloc>;
	using _Alref_alloc = _Rebind_alloc_t<_Alloc, _Refoa>;
	using _Alref_traits = allocator_traits<_Alref_alloc>;
	_Alref_alloc _Alref(_Al_arg);

	const auto _Rx = _Alref_traits::allocate(_Alref, 1);

	_TRY_BEGIN
		_Alref_traits::construct(_Alref, _Unfancy(_Rx), _Al_arg, _STD forward<_Types>(_Args)...);
	_CATCH_ALL
		_Alref_traits::deallocate(_Alref, _Rx, 1);
	_RERAISE;
	_CATCH_END

	shared_ptr<_Ty> _Ret;
	_Ret._Set_ptr_rep_and_enable_shared(_Rx->_Getptr(), _Unfancy(_Rx));
	return (_Ret);
	}

(1)
template<class _Fn>
	_INLINE_VAR constexpr bool _Pass_functor_by_value_v = sizeof(_Fn) <= sizeof(void *)
		&& conjunction_v<
			is_trivially_copy_constructible<_Fn>,
			is_trivially_destructible<_Fn>>;

template<class _Fn,
	enable_if_t<_Pass_functor_by_value_v<_Fn>, int> = 0>
	constexpr _Fn _Pass_fn(_Fn _Val)
	{	// pass functor by value
	return (_Val);
	}

template<class _Fn,
	enable_if_t<!_Pass_functor_by_value_v<_Fn>, int> = 0>
	constexpr _Ref_fn<_Fn> _Pass_fn(_Fn& _Val)
	{	// pass functor by "reference"
	return {_Val};
	}

// rolling the array!
template<size_t... _Indices,
	class... _LockN> inline
	void _Lock_from_locks(const int _Target, index_sequence<_Indices...>, _LockN&... _LkN)
	{	// lock _LkN[_Target]
	int _Ignored[] = { ((static_cast<int>(_Indices) == _Target
		? (void)_LkN.lock()
		: void()), 0)... };
	(void)_Ignored;
	}

	// can specialize on function pointer as well!
template <typename F>
struct make_tuple_of_params;
template <typename Ret, typename... Args>
struct make_tuple_of_params<Ret (Args...)>
{ using type = std::tuple<Args...>; };
   // if F is in the form void(double *, double *)
   // make_tuple_of_params is std::tuple<double *, double *>
   make_tuple_of_params_t<F> params;


	 //iterate over tuple! Better then recursion
	 template <typename F, typename Params, std::size_t... I>
auto dispatch_params(F f,
                     Params & params,
                     std::index_sequence<I...>)
{
    return f(std::get<I>(params)...);
}
template <typename F, typename Tuple, std::size_t... I>
void dispatch_to_c(F f, Tuple & t, std::index_sequence<I...>)
{  
	  f(&std::get<I>(t)...);
}


(2)
 template<class T,
         typename std::enable_if<
             !std::is_trivially_destructible<T>{} &&
             (std::is_class<T>{} || std::is_union<T>{}),
            int>::type = 0>
void destroy(T* t)

(3)

    template <typename C, 
        typename = void_t<>> 
        struct is_iterable 
        : std::false_type {}; 

        template <typename C> 
        struct is_iterable< 
        C, void_t<decltype(*begin(std::declval<C>())), 
        decltype(end(std::declval<C>()))>> 
        : std::true_type{};

(4). template <typename T, typename... Args>
std::unique_ptr<T> make_unique_helper(std::false_type, Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template <typename T, typename... Args>
std::unique_ptr<T> make_unique_helper(std::true_type, Args&&... args) {
   static_assert(std::extent<T>::value == 0,
       "make_unique<T[N]>() is forbidden, please use make_unique<T[]>().");

   typedef typename std::remove_extent<T>::type U;
   return std::unique_ptr<T>(new U[sizeof...(Args)]{std::forward<Args>(args)...});
}

template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
   return make_unique_helper<T>(std::is_array<T>(), std::forward<Args>(args)...);
}

(5). 

template<class... _Types> inline
	constexpr tuple<typename _Unrefwrap<_Types>::type...>
		make_tuple(_Types&&... _Args)
	{	// make tuple from elements
	typedef tuple<typename _Unrefwrap<_Types>::type...> _Ttype;
	return (_Ttype(_STD forward<_Types>(_Args)...));
	}



(6). 
template <class C, class F>
auto test(C c, F f) -> decltype((void)(c.*f)(), void())
{ std::cout << "Reference overload called\n"; }

template <class C, class F>
auto test(C c, F f) -> decltype((void)((c->*f)()), void())
{ std::cout << "Pointer overload called\n"; }

auto enableIfSINAFE(Container& v) -> decltype(enable_if_t<is_same<Iter, random_access_iterator_tag>::value>(), void())
auto enableIfSINAFE(Container& v) -> decltype(enable_if_t<is_same<Iter, bidirectional_iterator_tag>::value>(), void())

template <class _Ty1>
class = enable_if_t<_Is_swappable<_Ty1>::value && _Is_swappable<_Ty2>::value>>
void f()

(7). 
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

(8) Expression tempalate
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

9.  template <class Result, class... Args>
    auto memoFib(Result (*f)(Args...))

10. tuples don't have generic hash functions BUT do have operator <!
11. 
        template <class... InnerArgs>
        Result operator ()(InnerArgs&&... args)
        {
            const auto key = make_tuple(std::forward<InnerArgs>(args)...); #tuple does forwarding inside already!
             ..........
                auto r = _f(*this, std::forward<decay_t<InnerArgs>>(args)...); /but here we need forwarding!
12. function loop!
template <typename... Args>
 void f(Args... args)
{ {o(args)...};


13. this pointer is required!
template<class T>
struct B : A<T> {  int foo() {  return this->i }  };

14.       template<typename _Tp, typename... _Args>
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


15.   template<typename _Tp>
    struct __is_floating
    {
      enum { __value = 0 };
      typedef __false_type __type;
    };

  // three specializations (float, double and 'long double')
  template<>
    struct __is_floating<float>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };


16. 

    template <typename... T> struct _and;
    template <> struct _and<> : std::true_type {};
    template <typename T1> struct _and<T1> : T1 {};
    template <typename T1, typename T2> struct _and<T1, T2> : std::conditional<T1::value, T2, T1>::type  {};
    template <typename T1, typename T2, typename... Tn> struct _and<T1, T2, Tn...> : _and<T1, _and<T2, Tn...>> {};

17. 
  template<typename _From, typename _To,
           bool = __or_<is_void<_From>, is_function<_To>,
                        is_array<_To>>::value>
    struct __is_convertible_helper
    {
      typedef typename is_void<_To>::type type;
    };

  template<typename _From, typename _To>
    class __is_convertible_helper<_From, _To, false>
    {
      template<typename _To1>
    static void __test_aux(_To1) noexcept;

      template<typename _From1, typename _To1,
           typename = decltype(__test_aux<_To1>(std::declval<_From1>()))>
    static true_type
    __test(int);

      template<typename, typename>
    static false_type
    __test(...);

    public:
      typedef decltype(__test<_From, _To>(0)) type;
    };

18.   struct __nonesuchbase {};
  struct __nonesuch : private __nonesuchbase {
    ~__nonesuch() = delete;
    __nonesuch(__nonesuch const&) = delete;
    void operator=(__nonesuch const&) = delete;
  };

      pair&
      operator=(typename conditional<
        __and_<is_copy_assignable<_T1>,
               is_copy_assignable<_T2>>::value,
        const pair&, const __nonesuch&>::type __p)
      {
    first = __p.first;
    second = __p.second;
    return *this;
      }

19.  template <typename T,typename,typename... Args> struct mine_is_constructible : std::false_type {};
     template <typename T, typename... Args> struct mine_is_constructible<T, void_t<decltype(T(std::declval<Args>()...))>, Args...> : std::true_type {};
     template <typename T, typename... Args> using mine_is_constructible_v = mine_is_constructible<T, void_t<T>, Args...>;

20.   template<typename> struct __is_member_function_pointer_helper : public false_type { };
      template<typename _Tp, typename _Cp> struct __is_member_function_pointer_helper<_Tp _Cp::*> : public is_function<_Tp>::type { };

21.   template<typename> struct is_function : public false_type { };
      template<typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
    struct is_function<_Res(_ArgTypes...) _GLIBCXX_NOEXCEPT_QUAL>
    : public true_type { };


22. type complete check!
static_assert(sizeof(type), "type must be complete");

23. template <typename T, bool v = is...> struct S {}
    template <typename T> struct S<T, false> {}
     
24.   template<typename _MemberPointer>  struct _Mem_fn; // undefined
  template<typename _Res, typename _Class>
    struct _Mem_fn<_Res _Class::*>
    : _Mem_fn_base<_Res _Class::*>
    {
      using _Mem_fn_base<_Res _Class::*>::_Mem_fn_base;
    };

25. Make sure return types are comparable!

   template<typename _From, typename _To>
    using __check_func_return_type
      = __or_<is_void<_To>, is_same<_From, _To>, is_convertible<_From, _To>>;

      template<typename _Func,
           typename _Res2 = typename result_of<_Func&(_ArgTypes...)>::type>
    struct _Callable : __check_func_return_type<_Res2, _Res> { };

      // Used so the return type convertibility checks aren't done when
      // performing overload resolution for copy construction/assignment.
      template<typename _Tp>
    struct _Callable<function, _Tp> : false_type { }; //note that function here can be a short name for template!

26. template <char ..c>
  constexpr auto operator"" _c() {
//parses char and returns int!
}
1_c + 1_2

27. But since C++11 there is also expression SFINAE. expression SFINAE occurs on arbitrary expressions in the function signature

28.    template <typename... T>
 void p(const T&... args) 
 {   
   int a[] = {0, ((void)(cout << "," << args), 0)...}; (void)a;   
   cout << "\n";
 }

template <typename C> using ContainerType = decltype(*begin(declval<C>()));
template <typename C, typename = void_t<>> struct is_container : std::false_type {};
template <typename C> struct is_container<C, 
void_t<decltype(*begin(declval<C>())),
       decltype(*end(declval<C>())),
       typename C::iterator
>> : std::true_type {};

static_assert(is_container<vector<size_t>>::value, "not vec");

template <typename C, typename = enable_if_t<
std::conjunction<
  is_container<C>,
  std::negation<is_same<C, string>>
>::value
  >>
ostream& operator<<(ostream& out, const C& v)
{
  for (const auto& item : v)
    cout << item << ",";
  return out;
}

30. template <typename C> using ContainerType =
    typename iterator_traits<
        decltype(declval<C>().begin())
       >::value_type;

31.   FIFO(const FIFO& other) {  copyConstruct(other); }       
      template <typename O>  FIFO(const FIFO<O>& other) {  copyConstruct(other); }
      FIFO(FIF&& other) {  moveConstruct(other); }  

FIFO<int> q1;
FIFO<double> q2
q2 = move(q1);

    Even just having templated copy construct will cause q1 to be first converted into FIFO<double>
 and then moved into q2. So this is a bad idea!

32. template non-dependent symbol lookup just associats symbols with whatever it
 encounter so far! even it there is better definition later or after template instantiation. 
temlate <class T> struct B : D<T> { g() // global g() will be called, not one from D<T>! }; 
temlate <class T> struct B : D<T> { FromDT v; //won't compile because FromDT is defined in D<T>; D<T>::FromDt v will compile }; 

33. 

For a non-dependent name used in a template definition, unqualified name lookup takes place when the template definition is examined. The binding to the declarations made at that point is not affected by declarations visible at the point of instantiation. For a dependent name used in a template definition, the lookup is postponed until the template arguments are known, at which time ADL examines function declarations with external linkage (until C++11) that are visible from the template definition context as well as in the template instantiation context, while non-ADL lookup only examines function declarations with external linkage (until C++11) that are visible from the template definition context (in other words, adding a new function declaration after template definition does not make it visible except via ADL). The behavior is undefined if there is a better match with external linkage in the namespaces examined by the ADL lookup, declared in some other translation unit, or if the lookup would have been ambiguous if those translation units were examined. In any case, if a base class depends on a template parameter, its scope is not examined by unqualified name lookup (neither at the point of definition nor at the point of instantiation).

34. We now understand the difference between type deduction and type substitution. Type deduction can be ambiguous when different concrete types can be deduced from different arguments. If this happens, it means we have failed to deduce the argument types and cannot use this template function. Type substitution is never ambiguous—once we know what T is, we simply substitute that type every time we see T in the function definition. This process can also fail, but in a different way

35. namespace implementation {
      // Selected if C is a class type 
         template<typename C> static char test(int C::*);    // Selected otherwise  
         template<typename C> static int test(...);}t
          
          emplate <class T>struct is_class :   std::integral_constant<bool, sizeof
          (implementation::test<T>(0)) ==  sizeof(char)> {};

legacy like SFINAE


36. auto have_star_equal(const T& x, size_t n) -> decltype(T(x) *= n, yes())

37. how to call templated constructor from base class!
using __uniq_ptr_impl<_Tp, _Dp>::__uniq_ptr_impl;

38. template adaptor
template <typename T>
struct DeleteMyHeap : public DeleteHeap<T, MyHeap> {
    using DeleteHeap<T, MyHeap>::DeleteHeap;
};

template <class <typename> class C, class T>
ostream& operator<<(ostream& o, const C<T>& v)
{
}

operator<<(cout, DeleteMyHeap<int>()) // works!


39. std::type_identity can be used to establish non-deduced contexts in template argument deduction.

template<class T>
T foo(T a, T b) { return a + b; }
 
template<class T>
T bar(T a, std::type_identity_t<T> b) { return a + b; }
 
int main()
{
    // foo(4.2, 1); // error, deduced conflicting types for 'T'
    std::cout << bar(4.2, 1) << '\n';  // OK, calls bar<double>
}

In the following cases, the types, templates, and non-type values that are used to compose P do not participate in template argument deduction, but instead use the template arguments that were either deduced elsewhere or explicitly specified. If a template parameter is used only in non-deduced contexts and is not explicitly specified, template argument deduction fails.

1) The nested-name-specifier (everything to the left of the scope resolution operator ::) of a type that was specified using a qualified-id:


2) The expression of a decltype-specifier:
template<typename T>
void f(decltype(*std::declval<T>()) arg);

4) A template parameter used in the parameter type of a function parameter that has a default argument that is being used in the call for which argument deduction is being done:


....

40.

template specifalizing by another template are also possible!

template<class T>
class Foo;

template<class T>
class Foo<Bar<T>>
{
     /* Your implementation of this partial specialization. */
};
