An expression is a sequence of operators and operands that specifies a computation. An expression can result in a value and can cause side effects


The implementation can regroup operators according to the usual mathematical rules only where the operators really are associative or commutative

-----------
Value category
-------------

Each C++ expression (an operator with its operands, a literal, a variable name, etc.) is characterized by two independent properties: a type and a value category. Each expression has some non-reference type, and each expression belongs to exactly one of the three primary value categories: prvalue, xvalue, and lvalue.

a glvalue (“generalized” lvalue) is an expression whose evaluation determines the identity of an object or function;
a prvalue (“pure” rvalue) is an expression whose evaluation
computes the value of an operand of a built-in operator (such prvalue has no result object), or
initializes an object (such prvalue is said to have a result object).
The result object may be a variable, an object created by new-expression, a temporary created by temporary materialization, or a member thereof. Note that non-void discarded expressions have a result object (the materialized temporary). Also, every class and array prvalue has a result object except when it is the operand of decltype;
an xvalue (an “eXpiring” value) is a glvalue that denotes an object whose resources can be reused;

------------
temporary
------------

The materialization of a temporary object is generally delayed as long as possible in order to avoid creating unnecessary temporary objects. [ Note: Temporary objects are materialized

Even when the creation of the temporary object is unevaluated ([expr.context]), all the semantic restrictions shall be respected as if the temporary object had been created and later destroyed ----> EXCEPT <---------- if it used in decltype in that case semantic restrictions dont apply ----> 


Temporary objects are created
- when a prvalue is converted to an xvalue ([conv.rval]),
- when needed by the implementation to pass or return an object of trivially copyable type (see below), and
- when throwing an exception ([except.throw]).


A prvalue of type T can be converted to an xvalue of type T. This conversion initializes a temporary object ([class.temporary]) of type T from the prvalue by evaluating the prvalue with the temporary object as its result object, and produces an xvalue denoting the temporary object. T shall be a complete type.
[Note 1: If T is a class type (or array thereof), it must have an accessible and non-deleted destructor; see [class.dtor]. — end note]
[Example 1:
struct X { int n; };
int k = X().n;      // OK, X() prvalue is converted to xvalue


When an implementation introduces a temporary object of a class that has a non-trivial constructor ([class.default.ctor], [class.copy.ctor]), it shall ensure that a constructor is called for the temporary object. Similarly, the destructor shall be called for a temporary with a non-trivial destructor ([class.dtor]). Temporary objects are destroyed as the last step in evaluating the full-expression ([intro.execution]) that (lexically) contains the point where they were created. This is true even if that evaluation ends in throwing an exception. The value computations and side effects of destroying a temporary object are associated only with the full-expression, not with any specific subexpression

If a temporary object has a reference member initialized by another temporary object, lifetime extension applies recursively to such a member's initializer.
[Example 4:
struct S {
  const int& m;
};
const S& s = S{1};     //this is goodpotentially-overlapping subobject

except this! ->>>> 
- A temporary object bound to a reference parameter in a function call ([expr.call]) persists until the completion of the full-expression containing the call.
- A temporary object bound to a reference element of an aggregate of class type initialized from a parenthesized expression-list ([dcl.init]) persists until the completion of the full-expression containing the expression-list.
struct S {
  const int& m;
  S(const int& m):m(m1){}
};
const S& s = S{1};     //this is bad, dangling reference!



A temporary bound to a reference in a new-initializer ([expr.new]) persists until the completion of the full-expression containing the new-initializer.
[Note 7: This might introduce a dangling reference. — end note]
[Example 5:
struct S { int mi; const std::pair<int,int>& mp; };
S a { 1, {2,3} };
S* p = new S{ 1, {2,3} }; // dangling reference

the reason why temporary does not extend its lifetime in new experssion is the fact 


The materialization of a temporary object is generally delayed as long as possible in order to avoid creating unnecessary temporary objects.
[Note 3: Temporary objects are materialized:
- when binding a reference to a prvalue ([dcl.init.ref], [expr.type.conv], [ expr.dynamic.cast], [expr.static.cast], [expr.const.cast], [expr.cast]),
- when performing member access on a class prvalue ([expr.ref], [expr.mptr.oper]),
- when performing an array-to-pointer conversion or subscripting on an array prvalue ([conv.array], [expr.sub]),
- when initializing an object of type std​::​initializer_­list<T> from a braced-init-list ([dcl.init.list]),
- for certain unevaluated operands ([expr.typeid], [expr.sizeof]), and
- when a prvalue that has type other than cv void appears as a discarded-value expression


------------------------
TO ALLOW CPU REGISTER OPTIMIZATIONS
----------------------
When an object of class type X is passed to or returned from a function, if X has at least one eligible copy or move constructor ([special]), each such constructor is trivial, and the destructor of X is either trivial or deleted, implementations are permitted to create a temporary object to hold the function parameter or result object. The temporary object is constructed from the function argument or return value, respectively, and the function's parameter or return object is initialized as if by using the eligible trivial constructor to copy the temporary (even if that constructor is inaccessible or would not be selected by overload resolution to perform a copy or move of the object).

--------------------------
Unevaluated expressions
--------------------------
The operands of the operators typeid, sizeof, noexcept, and decltype (since C++11) are expressions that are not evaluated (unless they are polymorphic glvalues and are the operands of typeid), since these operators only query the compile-time properties of their operands. Thus, std::size_t n = sizeof(std::cout << 42); does not perform console output



