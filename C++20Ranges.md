# ranges

object which contains begin() and end(). They don't replace iterators,but enchance them!

normally ranges return std::ranges::in_out_result which got two members "in" (input iterator - the begin of it) and "out" (begin of output iterator)

The library creates and manipulates range views, lightweight objects that indirectly represent iterable sequences (ranges). Ranges are an abstraction on top of

[begin, end) – iterator pairs, e.g. ranges made by implicit conversion from containers. All algorithms that take iterator pairs now have overloads that accept ranges (e.g. ranges::sort)
begin + [​0​, size) – counted sequences, e.g. range returned by views::counted
[begin, predicate) – conditionally-terminated sequences, e.g. range returned by views::take_while
[begin, ..) – unbounded sequences, e.g. range returned by views::iota
The ranges library includes range algorithms, which are applied to ranges eagerly, and range adaptors, which are applied to views lazily. Adaptors can be composed into pipelines, so that their actions take place as the view is iterated.

## current issues and proposals

https://brevzin.github.io/cpp_proposals/2011_pipeline/d2011r2.html


## IMPORTAT caveates

- Ranges must be passed by value or by forwarding reference!!!!! Otherwise you may get UB - just because some ranges modify values and some other ranges do some internal caching

- IMPORTANT ! In general, it is always valid to modify the elements of any particular view except few exceptions like fitler_view

## how range view piping works!

So looking at this example
auto last = v | std::views::drop(1);

that "std::views::drop" is basically this 

```
 struct _Drop : __adaptor::_RangeAdaptor<_Drop>
    {
      template<viewable_range _Range, typename _Tp>
	requires __detail::__can_drop_view<_Range, _Tp>
	constexpr auto
	operator()(_Range&& __r, _Tp&& __n) const
	{
	 (4) return drop_view(std::forward<_Range>(__r), std::forward<_Tp>(__n));
	}
```

now **__adaptor::_RangeAdaptor** got the following operator function primarely which gets invoked first

```
  template<typename _Derived>
    struct _RangeAdaptor
    {
      template<typename... _Args>
	requires __adaptor_partial_app_viable<_Derived, _Args...>
	constexpr auto
	operator()(_Args&&... __args) const
	{
	(1)  return _Partial<_Derived, decay_t<_Args>...>{std::forward<_Args>(__args)...};
	}
    };
```
**_Partial** is defined as 

```
  template<typename _Adaptor, typename _Arg>
    requires __adaptor_has_simple_extra_args<_Adaptor, _Arg>
      && is_trivially_copyable_v<_Arg>
    struct _Partial<_Adaptor, _Arg> : _RangeAdaptorClosure
    {
      _Arg _M_arg;

      constexpr
      _Partial(_Arg __arg)
	: _M_arg(std::move(__arg))
      { }

      template<typename _Range>
	requires __adaptor_invocable<_Adaptor, _Range, const _Arg&>
	constexpr auto
	operator()(_Range&& __r) const
	(3) { return _Adaptor{}(std::forward<_Range>(__r), _M_arg); } //_Adaptor is (Drop_)

      static constexpr bool _S_has_simple_call_op = true;
    };
```
**_RangeAdaptorClosure** defined as 

```
  struct _RangeAdaptorClosure**
  {
    // range | adaptor is equivalent to adaptor(range).
    template<typename _Self, typename _Range>
      requires derived_from<remove_cvref_t<_Self>, _RangeAdaptorClosure>
	&& __adaptor_invocable<_Self, _Range>
      friend constexpr auto
      operator|(_Range&& __r, _Self&& __self) (--->v (Range) | Partial_<vies::filter::drop>(Self)  <----)
   (2)   { return std::forward<_Self>(__self)(std::forward<_Range>(__r)); }

    // Compose the adaptors __lhs and __rhs into a pipeline, returning
    // another range adaptor closure object.
    template<typename _Lhs, typename _Rhs>
      requires derived_from<_Lhs, _RangeAdaptorClosure>
	&& derived_from<_Rhs, _RangeAdaptorClosure>
      friend constexpr auto
      operator|(_Lhs __lhs, _Rhs __rhs)
      { return _Pipe<_Lhs, _Rhs>{std::move(__lhs), std::move(__rhs)}; }
  };
```

in the end **drop_view** is created which would contain the Range adapted with ranges::all view adapter. In our case it would convert std::vecgor into ranges::ref_view

     drop_view(_Vp __base, range_difference_t<_Vp> __count)
	: _M_count(__count), _M_base(std::move(__base))
      { __glibcxx_assert(__count >= 0); }


## now all range view algorthms support pipes!

operator| is only provided for those algorithms for which the first argument is a range and the result is a view.

transform, filter, take, etc., are all algorithms that take a range and return a view. More specifically, they take a viewable_range and return a view.

But for all the other algorithms - max, find_if, any_of, etc. - that don't return a range, there is no | alternative to them. So this:

```
    original
        | std::ranges::views::transform([](char c){return std::toupper(c);})
        | std::ranges::copy(std::back_inserter(upper));
```

isn't even intended to work, because copy is not one of the algorithms that returns a view, so it doesn't get | support


## rvalues to algorithms

Forbidding all uses of rvalue containers would make the above a compile error, forcing you to create a variable to hold the result of get_vector() before passing that to copy(). This seems overly heavy-handed – after all, the problem is not with the use of rvalue ranges per se, but with returning iterators into such ranges that people might later try to use.

The solution in C++20 is to have a special type, std::ranges::dangling, which algorithms use in their return types instead of potentially-dangling iterators when passed an rvalue range. So for example

auto vec = get_vector();
auto it = std::ranges::find(vec, 99);
is fine – it will be a std::vector::iterator. But if you were to write

auto it = std::ranges::find(get_vector(), 99);
then the code will still compile, but it is now of type std::ranges::dangling

IMPORTANT ! But for algorithms which don't return iterators, taking rvalues is completely fine!

## borrowed ranges


This is not the case for all ranges though. A std::string_view’s iterators for example actually point into some other character array – so we can safely use a string_view::iterator after the string_view itself has been destroyed (provided the underlying array is still around, of course):

std::string str = "Hello world";
// Weird, but okay:
auto iter = std::string_view{str}.begin();
*iter = 'h';
Types like std::string_view are called borrowed ranges in the new C++20 terminology. A borrowed range is one whose iterators can safely outlive their parent object. So an rvalue std::vector is not a borrowed range, but an rvalue std::span is.

Since we don’t need to worry about dangling iterators when we use borrowed ranges, we don’t need to apply the “dangling protection” described above when we pass them as rvalues to algorithms; we can just return iterators as normal:

auto iter = std::ranges::min_element(get_span());
// iter is an iterator, *not* ranges::dangling

auto vec = get_vector();


### Code example

```
auto v1 = vec | views::transform(func);
// Okay: vec is an lvalue
auto v2 = get_span() | views::transform(func);
// Okay: span is borrowed
auto v3 = subrange(vec.begin(), vec.end()) | views::transform(func)
// Okay: subrange is borrowed (and a view)
auto v4 = get_vector() | views::transform(func);
// ERROR: get_vector() returns an rvalue vector, which is neither
// a view nor a borrowed range

```

## sentinels

end() in a range is not necesserialy must be of the same iterator type as begin! it could be any type that got a comparison operator defined

unreachable_sentinel_t is used for optimization to ensure that any comparions would return false so that one can you use range algo which WOULD NEVER REACH the END and "iterator" with end() comparison could be optimized out!

std::ranges::find(s, std::unreachable_sentinel, CharT{})


## std::ranges::views::iota

 A range factory that generates a sequence of elements by repeatedly incrementing an initial value. Can be either bounded or unbounded (infinite)

```
  for (int i : std::ranges::iota_view{1, 10})
        std::cout << i << ' ';
    std::cout << '\n';
 
    for (int i : std::views::iota(1, 10))
        std::cout << i << ' ';
    std::cout << '\n';
```


## common ranges

you can make container out of them!

```
std::vector v(std::ranges::begin(r), std::ranges::end(r));
```
There are the STL containers, for which begin() and end() both return iterators of the same type. We call such a range a “common range”. If r is a common range, the above code will work.

## std::ranges::ref_view

    const std::string s{"cosmos"}; 
    const std::ranges::take_view tv{s, 3};
    const std::ranges::ref_view rv{tv};p

## member functions

```
auto results = inputs | ranges::view::transform(&X::getValue); // compiles OK
```

## convert range to standard container

* you can make container out of them!

```
std::vector v(std::ranges::begin(r), std::ranges::end(r));
```
There are the STL containers, for which begin() and end() both return iterators of the same type. We call such a range a “common range”. If r is a common range, the above code will work.



 * However, for all the new C++20 views, begin() returns an iterator and end() returns a sentinel. In general, those are different types. If you give the constructor of std::vector an iterator pair where the types don’t match, it will be all confused and fail to compile.

At first, it might look like we can fix this. There is a view in C++20 that lets you convert a non-common range into a common range. It’s called the common view. The adapter std::views::common returns the common view of a range, if it is not a common range already, and the all view of the range otherwise:

auto common = std::views::common(r);
auto v = std::vector(std::ranges::begin(common), std::ranges::end(common));
Unfortunately, this still doesn’t work for all ranges. Ranges with non-copyable iterators, such as std::ranges::basic_istream_view, cannot be converted to a common range (this is a constraint of common_view). In this case, this approach doesn’t work at all.



* Last resort which would finally work

So let’s try another approach. We can default-initialise the container first (which means you won’t get to benefit from CTAD), and then you explicitly copy the elements of the range across:

```
std::vector<my_type> v;
std::ranges::copy(r, std::back_inserter(v));
```


* Another summary of this issue which would be formally addressed in C++23


## projections

projection is a unary callable that is accepted by the most algorithms. projections modify the view of the data that projection sees. They are invocable really so you can pass pointers to memebers and pointers to member functions!

IMPORTANT: sometimes passing lambdas may be faster than passing pointers to memebers / functions, just because compliers more effectively inline lambads!

## views

IMPORTANT ! In general, it is always valid to modify the elements of any particular view 

enable to create ranges using lazy evaluation and don't create exessive copying / moves!


template<class T>
concept view = ranges::range<T> && std::movable<T> && ranges::enable_view<T>;


1) T models view only if:
move construction of T has constant time complexity, and if N copies and/or moves are made from a T object holding 
M elements, then these N objects have O(N+M) destruction (which implies that a moved-from view object has O(1) destruction), and
either std::copy_constructible<T> is false, or copy construction of T has constant time complexity, and
either std::copyable<T> is false, or copy assignment of T has no more time complexity than destruction followed by copy construction.

Examples of view types are:
-----------------------------
A range type that wraps a pair of iterators, e.g., std::ranges::subrange<I>.
A range type that holds its elements by std::shared_ptr and shares ownership with all its copies.
A range type that generates its elements on demand, e.g., std::ranges::iota_view.
A copyable container such as std::vector<std::string> generally does not meet the semantic requirements of view since copying the container copies all of the elements, which cannot be done in constant time.

While views were originally described as cheaply copyable and non-owning ranges, a type is not required to be copyable or non-owning for it to model view. However, it must still be cheap to copy (if it is copyable), move, assign, and destroy, so that range adaptors will not have unexpected complexity.

By default, a type modeling movable and range is considered a view if it is publicly and unambiguously derived from view_base, or exactly one specialization of std::ranges::view_interface.

### std::views::all

A RangeAdaptorObject (also a RangeAdaptorClosureObject) that returns a view that includes all elements of its range argument.
 - Given an expression e of type R, the expression views::all(e) is expression-equivalent to:
   Implicitly converting e to a std::decay_t<R> prvalue, if std::decay_t<R> models view.
- Otherwise, std::ranges::ref_view{e} if that expression is well-formed.
- Otherwise, std::ranges::owning_view{e}.

### iconic example

```
auto square = [](auto i) { return i* i;};
auto is_even = [](auto i) { return i % 2 == 0;};

auto view = vec | view::filter(is_even)
                | view::transform(square);

reanges::copy(view, ostream_iterator<int>{cout});
```


### filter_view DETAILS

Modification of the element a filter_­view​::​iterator denotes is permitted, but results in undefined behavior if the resulting value does not satisfy the filter predicate!

Also it caches the first begin() so that you must be carefull to re-run the view again!

drop_view, drop_while_view, split_view, reverse_view, and C++23's chunk_by_view.

## range adaptors

they create views from ranges or combine mutliple views into one


## running vies IN-PLACE - SUPPER IMPORTANT & EFFICIENT!

```
   std::vector<int> v{1, 1, 2, 4};
   std::vector<int> v2;
   auto square = [](auto i) { return i * i; };
   auto is_even = [](auto i) { return i % 2 == 0; };
   auto view = v | std::views::filter(is_even) | std::views::transform(square);
   auto r = std::ranges::transform(view, v.begin(), std::identity{}, {});
```

## examples

```
  std::ranges::for_each(v, [](auto v) {}, &Employe::id);
```


```
std::ranges::sort(employes, {}/*ranges_less by default*/, &Empliyed::id)
std::ranges::sort(paycheck, {}/*ranges_less by default*/, &Paycheck::id)
std::ranges::equalt_to(emplolyes, paycheck, {}/*ranges_equal by default*/, &Empliyed::id, &Paycheck::id)
```