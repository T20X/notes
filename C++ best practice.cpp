Inheritance or composition are the two choices for integrating the policy class into the primary class. In general, the composition should be preferred, unless there is a reason to use inheritance. We have already seen one such reason—the empty base class optimization. Inheritance is also the necessary choice if we want to affect the public interface of the class.
----------------------------------
there are no polymorphic types only the polymorphic use
------------------
shared structure breaks our ability to locally reason about code
---------------
don't call new directly for example, better use make_shared, make_unique
vector<shared_ptr<Item>> v;
v.emplace_back(new item); //if emplace_back throws due to size reallocation inside vector, then new Item would not be associated with a pointer and it would leak.
-----
class Builder {
Foo&& finish()    {  return std::move(obj);  } } // not good, as this would make object of Builder unusable!
better make it more explicit
class Builder { 
  Foo&& finish() && 
  { return std::move(obj); }
   // safer now caller is forced to call std::move! move(obj).finish();

--------------
when transferring ownership even trvially copyable types good to pass as rvalue refrences to functions as this would force caller to call std::move and it would be more clear code to read.
----------
use std::copy to copy as it would choose the most optimizied veriosn
------------
regular types match primitive type semantics
we call the set of axioms satisfaying data type and a set of operations on it a concept
---------------
f(object o);
f(object(12)) - copy of object can be elided - contstructs right in o!
---------
copy/move operators and destructos ideally should not have observable side-effects as if someone is 100% relied on them being called
------------------
objcet  {
  object(const object&)...
  object& operator=(const object&)...
  { object tmp(other); dat = move(tmp.dat);return *this; }

object f() { return 5; }
object o = f(); //only calls o ctor!
object o2 = 1;
o2 = f(); //calls ctor, ctor and copy
-------------
assignment operators ideally should provide strong exception safety gauarantee
---------------------
there is no such a polymorphic type, but the use is
-------------------
value is a correspondence between entities with common properties and a set of values
an object type is a pattern for storing a value of a corresponding value type inside the memory
an object is a representation of a concrete entity as a value in memory
we define operations in terms of the operations semantics. choosing the same syntax for the same semantics enables code reuse
(example: assingment is a procedure of taking two same types and making the first like second one without modifying the second one)

regular types which have operations defined on them using normal names are said to have "value semantics"
regualr types which are accessed indirectly through some shared reference are said to have "reference semantics"

partially formed type - it can be destructed, it can be assigned to, but cannnot be used in other way
-------------------
classes no need to define swap function anymore really from c++14 if move operators are present as std::swap can now work well with move operators!
------
objects shall be composable
sink object ideally pass by value
objects shall be easy to move
----------------------
Pointers can have a lot of different meanings in different situations, and they�re bad at communicating what they�re used for. When you see a function that returns a pointer, you have a few options:
It�s a factory function that returns an object you�ll become the owner of.
It�s a function that returns a pointer to an existing object you don�t own.
It�s a function that can fail, and it communicates the failure by returning the null pointer as the result.
------------------------------------------------------
This is the recommended approach to designing programs with state based on sum types: put the logic that deals with one state inside the object that defines that state, and put the logic that performs state transitions into the main program
-------------------------
It�s a bad idea to ever make std::pair or std::tuple to be a part of a public API
--------------------------------
no auto for non-moveable types! auto v = type() //type has to be moveable!
------------
void set_name(string s) noexcept //never declare it like that. even function is, but it may throw at the caller end!
{ _s = std::move(s); }

it is good to pass by values in constructors! because then we may not miss the capacity the source (string/vector) already have!

forwarding references are good in functions which forwards values and then the destination function will know what to do with it

----------------------
dont use rvalue refrence optimizations (&&) and forwarding refrences by default - just stick to const & and by value parameter passing and refer to the && as an optimization given the set of use cases
-----------------------
The most important insight is that the answer depends on the details of the base class�s contract. It is not enough to know that the public interfaces / method signatures are compatible; one also needs to know if the contracts / behaviors are compatible
-------------
The important part of the previous sentence are the words �contracts / behaviors.� That phrase goes well beyond the public interface = method signatures = method names and parameter types and constness. A method�s contract means its advertised behavior = advertised requirements and promises = advertised preconditions and postconditions. So if the base class has a method void insert(const Foo& x), the contract of that method includes the signature (meaning the name insert and the parameter const Foo&), but goes well beyond that to include the method�s advertised preconditions and postconditions
------------------------
in OO, a thing is defined by how it behaves, not by the label used to name it
The derived class must do what the base class promises, not what it actually does.
The key is that we�ve separated the advertised behavior (�specification�) from implemented behavior (�implementation�)
-------------
Next insight: inheritance means �is substitutable for.� It does not mean �is a� (since that is ill defined) and it does not mean �is a kind of� (also ill defined). Substitutability is well defined: to be substitutable, the derived class is allowed (not required) to add (not remove) public methods, and for each public method inherited from the base class, the derived class is allowed (not required) to weaken preconditions and/or strengthen postconditions (not the other way around). Further the derived class is allowed to have completely different constructors, static methods, and non-public methods
-------------
You should use inheritance (a) only if the derived class�s methods can abide by all the promises made in the base class, and (b) only if you don�t think you�ll confuse your users, and (c) only if there�s something to be gained by using the inheritance � some real, measurable improvement in time, money or risk

Bad inheritances always have a base class with an extra capability (often an extra member function or two; sometimes an extra promise made by one or a combination of member functions) that a derived class can�t satisfy. You�ve either got to make the base class weaker, make the derived class stronger, or eliminate the proposed inheritance relationship. I�ve seen lots and lots and lots of these bad inheritance proposals, and believe me, they all boil down to the Circle/Ellipse example

-------------
you can make anything inherit from anything provided the methods in the derived class abide by the promises made in the base class
------------
concept based polymorphizm
----------------------
Note: decltype(auto) is primarily useful for deducing the return type of forwarding functions and similar wrappers, as shown above, where you want the type to exactly �track� some expression you�re invoking. However, decltype(auto) is not intended to be a widely used feature beyond that. In particular, although it can be used to declare local variables, doing that is probably just an antipattern since a local variable�s reference-ness should not depend on the initialization expression. Also, it is sensitive to how you write the return statement. These two functions have different return types

------------------------
Don't delete the move members

*********************************
Assuming your compiler is completely C++11 conforming,
 then explicitly deleting the move constructor will also implicitly declare the following:

Foo(const Foo&) = delete;
Foo& operator=(const Foo&) = delete;
AND YOU CANNOT ENABLE THEM BACK!
-------------------------


---------
auto for range loops
---------

I personally like to use auto&& in generic code where I need to modify 
the elements of the sequence. If I don't, I'll just stick to auto const&
-----------
So what do I recommend? Use '\n', and std::flush if you really do
mean it. You may as well put the '\n' into the preceding string literal
while you are at it.
 std::cout << "foo\n";
 std::cout << "Some int: " << i << '\n';
 std::cout << "bar\n" << std::flush;

-----------------------
Do not use throw to indicate a coding error in usage of a function. Use assert or other mechanism to either send the process into a debugger or to crash the process and collect the crash dump for the developer to debug.
Do not use throw if you discover unexpected violation of an invariant of your component, use assert or other mechanism to terminate the program. Throwing an exception will not cure memory corruption and may lead to further corruption of important user data.
-----------------------
Good example of using exceptions 

void f()  // Using exceptions
{
  try {
    GResult gg = g();
    HResult hh = h();
    IResult ii = i();
    JResult jj = j();
    // ...
  }
  catch (FooError& e) {
    // ...code that handles "foo" errors...
  }
  catch (BarError& e) {
    // ...code that handles "bar" errors...
  }
}
IMAGINE NOW checking returns codes for each function instead???
---------------------

std::any issues
 - no control over memory allocation coz no way atm on how  to store type erased memory allocator in type erased class!
- if T move constructor throws is bad as it type is erased; any(any&&) noexcept...
---------------------------

constexpr containers downside
 - container is immutable
 - call site must know size of container
 --------------
  A typical mistake in implementing the Coercion by Member Template Idiom is failing to provide the non-template copy constructor or copy assignment operator when introducing the templated copy constructor and assignment operator. A compiler will automatically declare a copy constructor and a copy assignment operator if a class does not declare them, which can cause hidden and non-obvious faults when using this idiom

  -----------------
  We have examined a rather convoluted design pattern that combines both sides of C++—generic programming (templates) and object-oriented programming (inheritance). True to its name, the Curiously Recurring Template Pattern creates a circular loop, where the derived class inherits the interface and the implementation from the base class, while the base class has access to the interface of the derived class through the template parameters. CRTP has two main use modes—true static polymorphism, or static interface, where the object is primarily accessed as the base type, and expanding the interface, or delegation, where the derived class is accessed directly but the implementation uses CRTP to provide common functionality.
  -------------------------------
  A more accurate description of well-designed memory ownership takes more than one quoted sentence. Generally, the following are the attributes of good memory ownership practices:If a function or a class does not alter memory ownership in any way, this should be clear to every client of this function or class, as well as the implementerIf a function or a class takes exclusive ownership of some of the objects passed to it, this should be clear to the client (we assume that the implementer knows this already, since he/she has to write the code)If a function or a class shares ownership of an object passed to it, this should be clear to the client (or anyone who reads the client code, for that matter)For every object that is created, at every point that its used, it is clear whether this code is expected to delete it or not
  ---------------------------------
  bad design requires additional knowledge that cannot be deduced from the context
  ---------------------
  There is another, more fundamental reason, to avoid public virtual functions. A public method is a part of the class interface. Virtual function override is a customization of the implementation. A public virtual function inherently does both of these, at once. The same entity performs two very different functions that should not be coupled—declaring the public interface and providing an alternative implementation. Each of these functions have different constraints—the implementation can be altered in any way, as long as the hierarchy invariants hold. But the interface cannot be actually changed by the virtual function (except for returning covariant types, but even that does not really change the interface). All the public virtual function does is restate that yes, indeed, the public interface still looks like what the base class has declared. Such mixing of two very distinct roles calls for a better separation of concerns. The Template Method pattern is an answer to that design problem, and in C++, it takes the form of the Non-Virtual Interface (NVI).