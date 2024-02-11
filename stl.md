# iterators

‘input_iterator‘ maynotbecopyable

std::iterator used to define your own iterator has been deprecated now!

A value-initialized LegacyForwardIterator behaves like the past-the-end iterator of some unspecified empty container. Value-initialised iterators can only be compared to other value-initialised iterators.


The solution is to recognize the validity of null iterators by allowing value-initialized forward iterators to be compared, and ensuring that all value-initialized iterators for a particular container type compare equal. The result of comparing a value-initialized iterator to an iterator with a non-singular value is undefined

this is undefined behaviour btw:

```
std::vector<int> x{1,2,3};
std::vector<int>::iterator it{};
bool result = x.begin() != it;
```

this is fine!

```
it == it //True!
```

C++20 changed iterators a bit by adding new iterator concepts.
Changes:
- Unlike the LegacyInputIterator requirements, the input_iterator concept does not require equality_comparable, since input iterators are typically compared with sentinels.
- ForwardIterators are not comparable and neither default construcbale!
- Unlike the LegacyForwardIterator requirements, the forward_iterator concept does not require dereference to return a reference.
- Unlike the LegacyBidirectionalIterator requirements, the bidirectional_iterator concept does not require dereference to return an lvalue.

Although currently stl containers use both iterator constainrs: legacy iterators and new C++20 iterator concepts
For example here is iterator definitoin in vector: LegacyRandomAccessIterator, contiguous_iterator, and ConstexprIterator to value_type


# storing nont moveable / noncopyable types in containers

std::vector<std::mutex> v; // WON'T WORK!

std::deque / std::list - would work!

# containers

## unordered_map

After container move construction (overload (4)), references, pointers, and iterators (other than the end iterator) to other remain valid, but refer to elements that are now in *this

### emplace / try_emplace

template <class... Args>
pair<iterator, bool> try_emplace(const key_type& k, Args&&... args);
Unlike insert or emplace, these functions do not move from rvalue arguments
 IF the insertion does not happen, which makes it easy to manipulate maps
whose values are move-only types as emplace() before actual insertion or checking if element already exists does create a node first!
like this 
_Auto_node __z(*this, std::forward<_Args>(__args)...);
this is fine for keys which are not moveable / copyable if they are created from implicit constructors and not created by copy/move!
although for try_emplace to work it requires KEY object to already exist so that it would later copy / move it. emplace on the other hand does not require this , but it does first creates a full node without checking if the key exists!

so in summary emplace is still good in two cases:
 - keys are not copyable/moveable
 - cost to move/copy key is high compared to creating key object by implicit constructor

using structured binding
```
 const  auto& [my_level_it,new_level_added ] = my_ladder.levels.try_emplace(price,  typename MyLadder::OrderList{Order{id, quantity}});
```

### operator[]

**only works for non const objects!**

### erase

References and iterators to the erased elements are invalidated. Other iterators and references are not invalidated.
The iterator pos must be valid and dereferenceable. Thus the end() iterator (which is valid, but is not dereferenceable) cannot be used as a value for pos.

## std::priority_queue

if you use std::greater<>, note the the container type has to support operator>!

# std::less

Function object for performing comparisons. Unless specialized, invokes operator< on type T.

Implementation-defined strict total order over pointers
A specialization of std::less for any pointer type yields the implementation-defined strict total order, even if the built-in < operator does not.

also note that operator function of less can auto-deduce its parameters like this


(1) vector::push_back
"If T's move constructor is not noexcept and T is not CopyInsertable into *this, 
vector will use the throwing move constructor. If it throws, 
the guarantee is waived and the effects are unspecified."

move_if_noexcept - can still move even move throwing move constructor,
 if copy constructor is not available! That is were no-throw guarenatt is broken


If T's move constructor is not noexcept and T is not CopyInsertable into *this, vector will use the throwing move constructor. If it throws, the guarantee is waived and the effects are unspecified

(2)
If the new size() is greater than capacity() then all 
iterators and references (including the past-the-end iterator)
 are invalidated. Otherwise only the past-the-end iterator is invalidated

(3) unordered_map
If rehashing occurs due to the insertion, all iterators are invalidated.
 Otherwise iterators are not affected. References are not invalidated. 
Rehashing occurs only if the new number of elements is greater than max_load_factor()*bucket_count().

(4)
template <class M>
pair<iterator, bool> insert_or_assign(const key_type& k, M&& obj); 
If the insertion is successful, pointers and references to the element obtained while it is held 
in the node handle are invalidated, and pointers and references obtained
 to that element before it was extracted become valid.

insert_or_assign returns more information than operator[] and does not require default-constructibility of the mapped type

(5)


(6)
node_type extract( const_iterator position );
node_type extract( const key_type& x );

 Unlinks the node that contains the element pointed to by position and
 returns a node handle that owns i

Extracting a node invalidates only the iterators to the extracted element, 
and preserves the relative order of the elements that are not erased.
 Pointers and references to the extracted element remain valid, 
but cannot be used while element is owned by a node handle:
 they become usable if the element is inserted into a container.

That being said, the handle itself has member functions value()/key()/mapped() 
that may be of value (!) to you.

(7) deque

The complexity (efficiency) of common operations on deques is as follows:

Random access - constant O(1)
Insertion or removal of elements at the end or beginning - constant O(1)
Insertion or removal of elements - linear O(n)

erase in the end only invalidates the erase iterator and reference as well as past the end iterator
erase in the front only invalidates the erase iterator and reference, but not the rest including past the end iterator
insert in the end invalidates all the iterators including past the end. No references are invalidated
insert in the front invalidates all the iterators including past the end. No references are invalidated

insert/emplace - Time complexity: Constant plus linear in the lesser of the distances between pos and either of the ends of the container.
insert in the middle of the deque invalidates all iterators and references!



Invalidation notes
When inserting at either end of the deque, references are not invalidated by insert and emplace.
push_front, push_back, emplace_front and emplace_back do not invalidate any references to elements of the deque.
When erasing at either end of the deque, references to non-erased elements are not invalidated by erase, pop_front and pop_back.
A call to resize with a smaller size does not invalidate any references to non-erased elements.
A call to resize with a bigger size does not invalidate any references to elements of the deque.

(8) emplaces moves from even it can fail! But not try_emplace!

(9) unordered_map::insert
If rehashing occurs due to the insertion, all iterators are invalidated. Otherwise iterators are not affected. References are not invalidated. Rehashing occurs only if the new number of elements is greater than max_load_factor()*bucket_count(). If the insertion is successful, pointers and references to the element obtained while it is held in the node handle are invalidated, and pointers and references obtained to that element before it was extracted become valid. (since C++17)

(10) std::unordered_map::erase (worst case performance is N)
References and iterators to the erased elements are invalidated. Other iterators and references are not invalidated
The order of the elements that are not erased is preserved. (This makes it possible to erase individual elements while iterating through the container.)

(11) std::basic_string_view::data
Unlike std::basic_string::data() and string literals, data() may return a pointer to a buffer that is not null-terminated. Therefore it is typically a mistake to pass data() to a routine that takes just a const CharT* and expects a null-terminated strin

(12) Writing to the character array accessed through c_str() is undefined behavior

(13)        vector<int> v = { 1, 2, 3, 4,5, 9 };
            int SIZE = v.end() - v.begin();  WORKS!!!

(14) template< class InputIt >
typename std::iterator_traits<InputIt>::difference_type 
    distance( InputIt first, InputIt last );

(15)             list<int> v = { 1, 2, 3, 4,5, 9 };
            int n = std::distance(v.begin(), v.end()); 
distance first arugment MUST be before the second one in the container order apart from std::vector! 

(16) Like all other functions from <cctype>, the behavior of std::tolower is undefined if the argument's value is neither representable as unsigned char nor equal to EOF. To use these functions safely with plain chars (or signed chars), the argument should first be converted to unsigned char:

char my_tolower(char ch)
{
    return static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
}

(17) things custom iterator shall have
	using iterator_category = random_access_iterator_tag;
	using value_type = typename _Myvec::value_type;
	using difference_type = typename _Myvec::difference_type;
	using pointer = typename _Myvec::const_pointer;
	using reference = const value_type&;
	using _Tptr = typename _Myvec::pointer;

(18) The cost of the atomics, while not zero, is not overwhelming. The cost is mitigated by the use of move construction and move assignment which do not need to use atomic operations. Such operations are commonly used in vector<shared_ptr<T>> erase and insert

(19) be carefull with hashtables! if hash functions maps two items to diffrent slots even though they are equal, both will get inserted!!!!!

(20) Do keep in mind that all iterator range based functions assume closed;open interval, like [front;back). while back is the iterator PAST the last one processed!

(21) std::iterator_traits<InputIt>::difference_type  is there for a reason. Because it may potentially count objects which are not memory addressable - say number of planets/stars in the universe! Even though in practice it is std::ptrdiff_t (aka int64)

(22) One of the most useful features of pairs and tuples is that when you create product types with them, you get lexicographical comparison operators for free

(23) It�s a bad idea to ever make them a part of a public API (even if some parts of the STL do so)

(24) bool operator<(const person_t& left, const person_t& right)
{
    return std::tie(left.m_surname, left.m_name) <
            std::tie(right.m_surname, right.m_name);
}
]
(25) std::distance - returns the number of increments needed to go from first to last. The value may be negative if random-access iterators are used and first is reachable from last (since C++11)

(26) An LegacyInputIterator is an LegacyIterator that can read from the pointed-to element. LegacyInputIterators only guarantee validity for single pass algorithms: once an LegacyInputIterator i has been incremented, all copies of its previous value may be invalidated.

(27)  std::list(list&& other). After container move construction (overload (6)), references, pointers, and iterators (other than the end iterator) to other remain valid, but refer to elements that are now in *this

list(list&&) = defualt - is noexcept in gcc implementation!

(28) "The C++ Standard forbids containers of const elements
      because allocator<const/volatile T> is ill-formed

(29)  Allocator properties

      /// How the allocator is propagated on copy assignment
      using propagate_on_container_copy_assignment = false_type;

      /// How the allocator is propagated on move assignment
      using propagate_on_container_move_assignment = true_type;

      /// How the allocator is propagated on swap
      using propagate_on_container_swap = false_type;

      /// Whether all instances of the allocator type compare equal. That means other allocator
      /// can be moved in!
      using is_always_equal = true_type;

(30) All standard containers (std::vector, std::list, etc.) value-initialize their elements when constructed with a single size_type argument or when grown by a call to resize()


(31)
list& operator=( list&& other )
Move assignment operator. Replaces the contents with those of other using move semantics (i.e. the data in other is moved from other into this container). other is in a valid but unspecified state afterwards. If std::allocator_traits<allocator_type>::propagate_on_container_move_assignment::value is true, the target allocator is replaced by a copy of the source allocator. If it is false and the source and the target allocators do not compare equal, the target cannot take ownership of the source memory and must move-assign each element individually, allocating additional memory using its own allocator as needed. In any case, all elements originally present in *this are either destroyed or replaced by elementwise move-assignment

(32) unordered_set/unordered_map containers have the following important requirenments which make open addressing implementation not possible and chaining is used!

- iterators are only forward ones and it must be O(1) complexity to move to the next iterator!
- insertation of a new item shall not invalidate refrences to the items already stored in the container.

(33) list past the end iterator is never invalidates, because list creates it right away!

    [2]<->[past the end]<->[1]<->[2]->[past the end]...

(34)  with rbegin() we must add one before converting to a forward iterator to get an iterator that's valid 
    -- but with rend() we must not add one before converting to get a valid iterator.

(35) 
A value-initialized ForwardIterator behaves like the past-the-end iterator of some unspecified empty container: it compares equal to all value-initialized ForwardIterators of the same type

(36)
The elements of an unordered associative container are organized into buckets. Keys with the same hash code appear in the same bucket. The number of buckets is automatically increased as elements are added to an unordered associative container, so that the average number of elements per bucket is kept below a bound. Rehashing invalidates iterators, changes ordering between elements, and changes which buckets elements appear in, but does not invalidate pointers or references to elements. For unordered_multiset and unordered_multimap, rehashing preserves the relative ordering of equivalent elements


# std::unique

```
template<class ForwardIt>
ForwardIt unique(ForwardIt first, ForwardIt last)
{
    if (first == last)
        return last;
 
    ForwardIt result = first;
    while (++first != last)
        //result would get stuck at unique one until
        // first advances all unique ones in a group and hits a distinct element
        if (!(*result == *first) && ++result != first) 
            *result = std::move(*first);
 
    return ++result;
}
```
# iterators

since a default-constructed container iterator is not associated with any particular container, there is no good value it could take. Therefore it is just an uninitialized variable and the only legal operation to do with it is to assign a valid iterator to it.


# Don’t blindly prefer emplace_back to push_back

In one of my recent training courses, a student informed me that both clang-tidy and PVS-Studio were complaining about some code of the form

std::vector<Widget> widgets;

widgets.push_back(Widget(foo, bar, baz));
Both tools flagged this line as “bad style.” clang-tidy even offered a (SARCASM ALERT) helpful fixit:

warning: use emplace_back instead of push_back [modernize-use-emplace]
    widgets.push_back(Widget(foo, bar, baz));
            ^~~~~~~~~~~~~~~~~             ~
            emplace_back(
The student dutifully changed the line, and both tools reported their satisfaction with the replacement:

widgets.emplace_back(Widget(foo, bar, baz));
The original line materializes a temporary Widget object on the stack; takes an rvalue reference to it; and passes that reference to vector<Widget>::push_back(Widget&&), which move-constructs a Widget into the vector. Then we destroy the temporary.

The student’s replacement materializes a temporary Widget object on the stack; takes an rvalue reference to it; and passes that reference to vector<Widget>::emplace_back<Widget>(Widget&&), which move-constructs a Widget into the vector. Then we destroy the temporary.

Absolutely no difference.

The change clang-tidy meant to suggest — and in fact did suggest, if you pay very close attention to the underlining in the fixit — was actually this:

widgets.emplace_back(foo, bar, baz);
This version does not materialize any Widget temporaries. It simply passes foo, bar, baz to vector<Widget>::emplace_back<Foo&, Bar&, Baz&>(Foo&, Bar&, Baz&), which constructs a Widget into the vector using whatever constructor of Widget best matches that bunch of arguments.

emplace_back is not magic C++11 pixie dust
Even a decade after C++11 was released, I still sometimes see programmers assume that emplace_back is somehow related to move semantics. (In the same way that some programmers assume lambdas are somehow the same thing as std::function, you know?) For example, they’ll rightly observe that this code makes an unnecessary copy:

void example() {
    auto w = Widget(1,2,3);
    widgets.push_back(w);  // Copy-constructor alert!
}
So they’ll change it to this:

void example() {
    auto w = Widget(1,2,3);
    widgets.emplace_back(w);  // Fixed? Nope!
}
The original line constructs a Widget object into w, then passes w by reference to vector<Widget>::push_back(const Widget&), which copy-constructs a Widget into the vector.

The replacement constructs a Widget object into w, then passes w by reference to vector<Widget>::emplace_back<Widget&>(Widget&), which copy-constructs a Widget into the vector.

Absolutely no difference.

What the student should have done is ask the compiler to make an rvalue reference to w, by saying either

widgets.push_back(std::move(w));
or

widgets.emplace_back(std::move(w));
It doesn’t matter which verb you use; what matters is the value category of w. You must explicitly mention std::move, so that the language (and the human reader) understand that you’re done using w and it’s okay for widgets to pilfer its guts.

emplace_back was added to the language at the same time as std::move — just like lambdas were added at the same time as std::function — but that doesn’t make them the same thing. emplace_back may “look more C++11-ish,” but it’s not magic move-enabling pixie dust and it will never insert a move in a place you don’t explicitly request one.

When all else is equal, prefer push_back to emplace_back
So, given that these two lines do the same thing and are equally efficient at runtime, which should I prefer, stylistically?

widgets.push_back(std::move(w));
widgets.emplace_back(std::move(w));
I recommend sticking with push_back for day-to-day use. You should definitely use emplace_back when you need its particular set of skills — for example, emplace_back is your only option when dealing with a deque<mutex> or other non-movable type — but push_back is the appropriate default.

One reason is that emplace_back is more work for the compiler. push_back is an overload set of two non-template member functions. emplace_back is a single variadic template.

# some good STL implementations

## std::less<>::operator()

member type is_transparent indicates to the caller that this function object is a transparent function object: it accepts arguments of arbitrary types and uses perfect forwarding, which avoids unnecessary copying and conversion when the function object is used in heterogeneous context, or with rvalue arguments. In particular, template functions such as std::set::find and std::set::lower_bound make use of this member type on their Compare types

```
std::less<>::operator()
template< class T, class U >
constexpr auto operator()( T&& lhs, U&& rhs ) const
    -> decltype(std::forward<T>(lhs) < std::forward<U>(rhs));
```
