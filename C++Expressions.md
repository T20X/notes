# Side effects

Reading an object designated by a volatile glvalue ([basic.lval]), modifying an object, calling a library I/O function, or calling a function that does any of those operations are all side effects, which are changes in the state of the execution environment. Evaluation of an expression (or a subexpression) in general includes both value computations (including determining the identity of an object for glvalue evaluation and fetching a value previously assigned to an object for prvalue evaluation) and initiation of side effects. When a call to a library I/O function returns or an access through a volatile glvalue is evaluated the side effect is considered complete, even though some external actions implied by the call (such as the I/O itself) or by the volatile access may not have completed yet.

#  as-if rule

According to the as-if rule, C++ allows any and all code transformations that do not change the observable behavior of


The semantic descriptions in this International Standard define a parameterized nondeterministic abstract machine. This International Standard places no requirement on the structure of conforming implementations. In particular, they need not copy or emulate the structure of the abstract machine. Rather, conforming implementations are required to emulate (only) the observable behavior of the abstract machine as explained below


This provision is sometimes called the “as-if” rule, because an implementation is free to disregard any requirement of this International Standard as long as the result is as if the requirement had been obeyed, as far as can be determined from the observable behavior of the program. For instance, an actual implementation need not evaluate part of an expression if it can deduce that its value is not used and that no side effects affecting the observable behavior of the program are produced


The least requirements on a conforming implementation are:
  - Accesses through volatile glvalues are evaluated strictly according to the rules of the abstract machine.
  - At program termination, all data written into files shall be identical to one of the possible results that execution of the program according to the abstract semantics would have produced.
  - The input and output dynamics of interactive devices shall take place in such a fashion that prompting output is actually delivered before a program waits for input. What constitutes an interactive device is implementation-defined.
These collectively are referred to as the observable behavior of the program.
[Note 2: More stringent correspondences between abstract and actual semantics can be defined by each implementation. — end note]
6) This provision is sometimes called the “as-if” rule, because an implementation is free to disregard any requirement of this document as long as the result is as if the requirement had been obeyed, as far as can be determined from the observable behavior of the program. For instance, an actual implementation need not evaluate part of an expression if it can deduce that its value is not used and that no side effects affecting the observable behavior of the program are produced.

# declarations

Declarations are how names are introduced (or re-introduced) into the C++ program. Not all declarations actually declare anything, and each kind of entity is declared differently. Definitions are declarations that are sufficient to use the entity identified by the name.

https://en.cppreference.com/w/cpp/language/declarations


When a block-declaration appears inside a block, and an identifier introduced by a declaration was previously declared in an outer block, the outer declaration is hidden for the remainder of the block

simple declarations -> attr decl-specifier-seq (pretty much type with const /volatile/const/ constexpr etc) ) [decorator](#decorator) list

An instance of each object with automatic storage duration is associated with each entry into its block. Such an object exists and retains its last-stored value during the execution of the block and while the block is suspended (by a call of a function, suspension of a coroutine ([expr.await]), or receipt of a signal).

## decorator

Each declarator introduces exactly **one** object, reference, function, or (for typedef declarations) type alias, whose type is provided by decl-specifier-seq and optionally modified by operators such as & (reference to) or [] (array of) or () (function returning) in the declarator. These operators can be applied recursively, as shown below. Note that apart from variable name it can contain refrences and pointers as well initiazlizator


# Expression theory

expression evaluation -> ( side-effects & value) 

An expression is a sequence of operators and operands that specifies a computation. An expression can result in a value and can cause side effects The implementation can regroup operators according to the usual mathematical rules only where the operators really are associative or commutative.
Expression evaluation may produce a result (e.g., evaluation of 2 + 2 produces the result 4) and may generate side-effects (e.g. evaluation of std::printf("%d", 4) prints the character '4' on the standard output).

Each C++ expression is characterized by two independent properties: A type and a value category

The operands of any operator may be other expressions or  [primary expression](#primary-expression)  (e.g. in 1 + 2 * 3, the operands of operator+ are the subexpression 2 * 3 and the primary expression 1).


## expression statement

An expression statement is an expression followed by a semicolon.

attr ﻿(optional) expression ﻿(optional) ;		
attr	-	(since C++11) optional sequence of any number of attributes
expression	-	an expression
Most statements in a typical C++ program are expression statements, such as assignments or function cal

## primary expression

A primary-expression is not necessarily atomic, evaluated first, top-level, more important than other expressions , or anything like that . And all expressions are "building blocks" because any expression can be added to to form a larger expression

It refers to the way that operands are grouped with operators, not the order in which subexpressions are executed. In the case of f() + ([]{int n, t1 = 0, t2 = 1, nextTerm = 0; cout << "Enter the number of terms: ";.... etc. etc. , the f() may or may not be called before the lambda is called. The parentheses around the lambda do not cause it to be evaluated first


Primary expressions are any of the following:

* this
* literals (e.g. 2 or "Hello, world")
* id-expressions, including
* suitably declared unqualified identifiers (e.g. n or cout),
* suitably declared qualified identifiers (e.g. std::string::npos), and
* identifiers to be declared in declarators
* lambda-expressions
* fold-expressions
* requires-expressions
* Any expression in parentheses is also classified as a primary expression: this guarantees that the parentheses have higher precedence than any operator. Parentheses preserve value, type, and value category

## subexpression

The immediate subexpressions of an expression E are
(3.1) the constituent expressions of E's operands ([expr.prop]) (say function paramaters)
(3.2) any function call that E implicitly invokes,
(3.3) if E is a lambda-expression ([expr.prim.lambda]), the initialization of the entities captured by copy and the constituent expressions of the initializer of the init-captures,
(3.4) if E is a function call or implicitly invokes a function, the constituent expressions of each default argument ([dcl.fct.default]) used in the call, or
(3.5) if E creates an aggregate object ([dcl.init.aggr]), the constituent expressions of each default member initializer ([class.mem]) used in the initialization.

**void func(){
  Test t{} // implicitly invoke the defautl constructor of `Test`
  // would implicitly  invoke the destructor of `Test`
}
the expression func() does not implicitly invoke anything. Functions that are invoked in the body of func() don't count. The point about implicit invocations includes such things as the destructors of temporaries at the end of a full-expression, constructors called to materialize temporary objects, constructors and conversion operators invoked by implicit conversions required by an expression, and so on**


(3.5) if E creates an aggregate object ([dcl.init.aggr]), the constituent expressions of each default member initializer ([class.mem]) used in the initialization.


A subexpression of an expression E is an immediate subexpression of E or a subexpression of an immediate subexpression of E. Note that expressions appearing in the 'function body' of lambda expressions are not subexpressions of the lambda expression



## full expression

A full-expression is
(5.1) an unevaluated operand,
(5.2) a constant-expression ([expr.const]),
(5.3) an immediate invocation ([expr.const]),
(5.4) **an init-declarator ([dcl.decl]) or a mem-initializer ([class.base.init]), including the constituent expressions of the initializer**
(5.5) an invocation of a destructor generated at the end of the lifetime of an object other than a temporary object ([class.temporary]) **whose lifetime has not been extended**(***this means that if lifetime of temporary not extend, it is part of full epxression in which temporary was created***), or
(5.6) an expression that is not a subexpression of another expression and that is not otherwise part of a full-expression.

If a language construct is defined to produce an implicit call of a function, a use of the language construct is considered to be an expression for the purposes of this definition. Conversions applied to the result of an expression in order to satisfy the requirements of the language construct in which the expression appears are also considered to be part of the full-expression. For an initializer, performing the initialization of the entity (including evaluating default member initializers of an aggregate) is also considered part of the full-expression.
[Example 2: 

```
struct S {
  S(int i): I(i) { }            // full-expression is initialization of I
  int& v() { return I; }
  ~S() noexcept(false) { }
private:
  int I;
};

S s1(1);                        // full-expression comprises call of S​::​S(int)
void f() {
  S s2 = 2;                     // full-expression comprises call of S​::​S(int)
  if (S(3).v())                 // full-expression includes lvalue-to-rvalue and int to bool conversions,
                                // performed before temporary is deleted at end of full-expression
  { }
  bool b = noexcept(S(4));      // exception specification of destructor of S considered for noexcept

  // full-expression is destruction of s2 at end of block
}
```

## value computations

 value computations (including determining the identity of an object for glvalue evaluation and fetching a value previously assigned to an object for prvalue evaluation)

## "evaluation"

[https://eel.is/c++draft/basic#intro.execution-7]
Evaluation of an expression (or a subexpression) in general includes both value computations (including determining the identity of an object for glvalue evaluation and fetching a value previously assigned to an object for prvalue evaluation) and initiation of side effects

# Sequence points

The C++11 and C++14 versions of the standard do not formally contain 'sequence points'; operations are 'sequenced before' or 'unsequenced' or 'indeterminately sequenced' instead. The net effect is essentially the same, but the terminology is different.


# Sequenced before

Sequenced before is an asymmetric, transitive, pair-wise relation between evaluations executed by a single thread ([intro.multithread]), which induces a partial order among those evaluations. Given any two evaluations A and B, if A is sequenced before B (or, equivalently, B is sequenced after A), then the execution of A shall precede the execution of B. If A is not sequenced before B and B is not sequenced before A, then A and B are unsequenced.
[Note 3: The execution of unsequenced evaluations can overlap. — end note]
Evaluations A and B are indeterminately sequenced when either A is sequenced before B or B is sequenced before A, but it is unspecified which.
[Note 4: Indeterminately sequenced evaluations cannot overlap, but either can be executed first. — end note]

An expression X is said to be sequenced before an expression Y if every value computation and every side effect associated with the expression X is sequenced before every value computation and every side effect associated with the expression Y.


Every value computation and side effect associated with a full-expression is sequenced before every value computation and side effect associated with the next full-expression to be evaluated.37

Except where noted, evaluations of operands of individual operators and of subexpressions of individual expressions are unsequenced.
[Note 5: In an expression that is evaluated more than once during the execution of a program, unsequenced and indeterminately sequenced evaluations of its subexpressions need not be performed consistently in different evaluations. — end note]
The value computations of the operands of an operator are sequenced before the value computation of the result of the operator. If a side effect on a memory location ([intro.memory]) is unsequenced relative to either another side effect on the same memory location or a value computation using the value of any object in the same memory location, and they are not potentially concurrent ([intro.multithread]), the behavior is undefined.
[Note 6: The next subclause imposes similar, but more complex restrictions on potentially concurrent computations. — end note]
[Example 3: 
void g(int i) {
  i = 7, i++, i++;              // i becomes 9

  i = i++ + 1;                  // the value of i is incremented
  i = i++ + i;                  // undefined behavior
  i = i + 1;                    // the value of i is incremented
}
— end example]


When invoking a function (whether or not the function is inline), every argument expression and the postfix expression designating the called function are sequenced before every expression or statement in the body of the called function. For each function invocation or evaluation of an await-expression F, each evaluation that does not occur within F but is evaluated on the same thread and as part of the same signal handler (if any) is either sequenced before all evaluations that occur within F or sequenced after all evaluations that occur within F;38 if F invokes or resumes a coroutine ([expr.await]), only evaluations subsequent to the previous suspension (if any) and prior to the next suspension (if any) are considered to occur within F.
Several contexts in C++ cause evaluation of a function call, even though no corresponding function call syntax appears in the translation unit.
[Example 4: Evaluation of a new-expression invokes one or more allocation and constructor functions; see [expr.new]. For another example, invocation of a conversion function ([class.conv.fct]) can arise in contexts in which no function call syntax appears. — end example]
The sequencing constraints on the execution of the called function (as described above) are features of the function calls as evaluated, regardless of the syntax of the expression that calls the function.



Once an rvalue has been bound to a name, it's an lvalue again, whether it's a wrapping type as std::tuple or rvalue references or a plain rvalue reference.


# function call

A function call is a postfix expression followed by parentheses containing a possibly empty, comma-separated list of initializer-clauses which constitute the arguments to the function

Calling a function through an expression whose function type E is different from the function type F of the called function's definition results in undefined behavior unless the type “pointer to F” can be converted to the type “pointer to E” via a function pointer conversion

A function call is an lvalue if the result type is an lvalue reference type or an rvalue reference to function type, an xvalue if the result type is an rvalue reference to object type, and a prvalue otherwise


It is implementation-defined whether the lifetime of a parameter ends when the function in which it is defined returns or at the end of the enclosing full-expression. The initialization and destruction of each parameter occurs within the context of the calling function

function parameters are really expressions

# Refenrece

references are not objects

a refenrece is a basically a const pointer that gets dereferenced every time it refered to. a reference yields **lvalue expression**

refenreces were made to make overloaded operators to work with class types . basically there is a need for something like a pointer but without explicitly taking its address

a reference to const T can bind to an expression x that's not an lvalue of type T.
...if there's a conversion from x's type to T


rvalue refenreces bind only to rvalues or to plain values types
rvalue refrences (type) bind only to rvalues

```
struct A { int& i; };
struct A2 { int&& i; };
inline int f2(int i) {return 2 * i;}
int main() {
    float x = 3.14f;
    float&& expr = x * x + x * x; // expr binds to the result of an expression
    expr = x; // this isn't a rebind, just an assignment to an rvalue reference

    float& y = x;
    // no way to rebind y. It's an lvalue reference
    // and it cannot be changed to refer to another lvalue.

    // However, lvalue references that are inside a class can
    // be changed since an object of the class can be identified
    int i1{ 1 };
    int i2{ 2 };

    // lvalue reference in a class
    A a0{ i1 };
    A a1{ i2 };
    std::construct_at(&a0, a1); // a now contains a ref to i2;

    {
        A2 a02{ 4 };
        {
          A2 a12{ 9 };
          std::construct_at(&a02, a12); // a now contains a ref to i2; not UB even rvalue rebounded, but note that  tmp which a02.i pointed to still has not his lifetime ended
        }

        //temp which points to a02.i get its lifetime ended
    }

```

## refrence coallasing

typedef int&  lref;
typedef int&& rref;
int n;
 
lref&  r1 = n; // type of r1 is int&
lref&& r2 = n; // type of r2 is int&
rref&  r3 = n; // type of r3 is int&
rref&& r4 = 1; // type of r4 is int&&



## pointer to reference
does not exist!
//int &*v2 = &p1; cannot be complied 

## you can have reference to functions as well

```
void Foo() {}

int main()
{
    void(& func)() = Foo;
    func(); //::Foo();
}
```

## reference compatibility

Given  types “cv1 T1” and “cv2 T2”, “cv1 T1” is reference-related to “cv2 T2” if T1 is similar ([conv.qual]) to T2, or T1 is a base class of T2. “cv1 T1” is **reference-compatible** with “cv2 T2” if a prvalue of type “pointer to cv2 T2” can be converted to the type “pointer to cv1 T1” via a standard conversion sequence ([conv]). In all cases where the **reference-compatible** relationship of two types is used to establish the validity of a reference binding and the standard conversion sequence would be ill-formed, a program that necessitates such a binding is ill-formed.


## referece initialization

A reference to type “cv1 T1” is initialized by an expression of type “cv2 T2” as follows:
  (5.1) If the reference is an lvalue reference and the initializer expression
  (5.1.1) is an lvalue (but is not a bit-field), and “cv1 T1” is reference-compatible with “cv2 T2”, or
  (5.1.2) has a class type (i.e., T2 is a class type), where T1 is not reference-related to T2, and can be converted to an lvalue of type “cv3 T3”, where “cv1 T1” is reference-compatible with “cv3 T3”85 (this conversion is selected by enumerating the applicable conversion functions ([over.match.ref]) and choosing the best one through overload resolution),
  then the reference binds to the initializer expression lvalue in the first case and to the lvalue result of the conversion in the second case (or, in either case, to the appropriate base class subobject of the object).
  [Note 2: The usual lvalue-to-rvalue, array-to-pointer, and function-to-pointer standard conversions are not needed, and therefore are suppressed, when such direct bindings to lvalues are done. — end note]
  [Example 3:
  ```
  double d = 2.0;
  double& rd = d;                 // rd refers to d
  const double& rcd = d;          // rcd refers to d

  struct A { };
  struct B : A { operator int&(); } b;
  A& ra = b;                      // ra refers to A subobject in b, A& and B& are refrence-related
  const A& rca = b;               // rca refers to A subobject in b
  int& ir = B();                  // ir refers to the result of B​::​operator int&
  ```

  — end example]
(5.2)
Otherwise, if the reference is an lvalue reference to a type that is not const-qualified or is volatile-qualified, the program is ill-formed.
[Example 4:
double& rd2 = 2.0;              // error: not an lvalue and reference not const
int  i = 2;
double& rd3 = i;                // error: type mismatch and reference not const
— end example]
(5.3)
Otherwise, if the initializer expression
(5.3.1)
is an rvalue (but not a bit-field) or function lvalue and “cv1 T1” is reference-compatible with “cv2 T2”, or
(5.3.2)
has a class type (i.e., T2 is a class type), where T1 is not reference-related to T2, and can be converted to an rvalue or function lvalue of type “cv3 T3”, where “cv1 T1” is reference-compatible with “cv3 T3” (see [over.match.ref]),
then the initializer expression in the first case and the converted expression in the second case is called the converted initializer. If the converted initializer is a prvalue, its type T4 is adjusted to type “cv1 T4” ([conv.qual]) and the temporary materialization conversion ([conv.rval]) is applied. In any case, the reference binds to the resulting glvalue (or to an appropriate base class subobject).
[Example 5:
```
struct A { };
struct B : A { } b;
extern B f();
const A& rca2 = f();                // binds to the A subobject of the B rvalue.
A&& rra = f();                      // same as above
struct X {
  operator B();
  operator int&();
} x;
const A& r = x;                     // binds to the A subobject of the result of the conversion
int i2 = 42;
int&& rri = static_cast<int&&>(i2); // binds directly to i2
B&& rrb = x;                        // binds directly to 
```
the result of operator B
— end example]
(5.4)
Otherwise:
(5.4.1)
If T1 or T2 is a class type and T1 is not reference-related to T2, user-defined conversions are considered using the rules for copy-initialization of an object of type “cv1 T1” by user-defined conversion ([dcl.init], [over.match.copy], [over.match.conv]); the program is ill-formed if the corresponding non-reference copy-initialization would be ill-formed. The result of the call to the conversion function, as described for the non-reference copy-initialization, is then used to direct-initialize the reference. For this direct-initialization, user-defined conversions are not considered.
(5.4.2)
Otherwise, the initializer expression is implicitly converted to a prvalue of type “T1”. The temporary materialization conversion is applied, considering the type of the prvalue to be “cv1 T1”, and the reference is bound to the result.
**If T1 is reference-related to T2:
(5.4.3)
cv1 shall be the same cv-qualification as, or greater cv-qualification than, cv2; and**
(5.4.4)
**if the reference is an rvalue reference, the initializer expression shall not be an lvalue.**
[Note 3: This can be affected by whether the initializer expression is move-eligible ([expr.prim.id.unqual]). — end note]
[Example 6:
```
struct Banana { };
struct Enigma { operator const Banana(); };
struct Alaska { operator Banana&(); };
void enigmatic() {
  typedef const Banana ConstBanana;
  Banana &&banana1 = ConstBanana(); // error
  Banana &&banana2 = Enigma();      // error
  Banana &&banana3 = Alaska();      // error
}

const double& rcd2 = 2;             // rcd2 refers to temporary with value 2.0
double&& rrd = 2;                   // rrd refers to temporary with value 2.0
const volatile int cvi = 1;
const int& r2 = cvi;                // error: cv-qualifier dropped
struct A { operator volatile int&(); } a;
const int& r3 = a;                  // error: cv-qualifier dropped
                                    // from result of conversion function
double d2 = 1.0;
double&& rrd2 = d2;                 // error: initializer is lvalue of related type
struct X { operator int&(); };
int&& rri2 = X();                   // error: result of conversion function is lvalue of related type
int i3 = 2;
double&& rrd3 = i3;                 // rrd3 refers to temporary with value 2.0
```

— end example]
In all cases except the last (i.e., implicitly converting the initializer expression to the referenced type), the reference is said to bind directly to the initializer expression.


## forwarding refrence

template<typename T> decltype(auto)
f(T&& t)
{
  return g(std::forward<T>(t));
}
A function parameter such as T&& t is known as a forwarding reference. It matches arguments of any value category, making t an lvalue reference if the supplied argument was an lvalue or an rvalue reference if the supplied argument was an rvalue. If U is t’s underlying non-reference type (namely std::remove_reference_t<decltype(t)>), then T will be inferred as U& for an lvalue argument and U for an rvalue. (Through reference collapsing, if T is U&, then T&& is also U&.) Regardless of t’s variable decltype, its expression decltype is always an lvalue reference; that’s why you always need to provide an explicit template argument to std::forward.

Note that in the example, f actually demonstrates an appropriate use of decltype(auto) return type to preserve the value category of g’s result (including prvalue). Note also that except for initializer lists, auto bindings use the same type deduction rules as function templates. Hence, “auto &&x = f()” is another form of forwarding reference.


# Value Category

The most important thing to remember is that value categories are a taxonomy of expressions. They are not categories of objects or variables or types
value category is just synonymous with the reference qualification on expression decltype
      
Each C++ expression(an operator with its operands, a literal, a variable name, etc.) is characterized by two independent properties : a type and a value category.Each expression has some non-reference type, and each expression belongs to exactly one of the three primary value categories : prvalue, xvalue, and lvalue.


const T& can be initialized from any value category, but overload resolution will prefer T&& over const T& for rvalues if there are functions accepting both. (This is why copy constructors can fill the role of a missing move constructor.)


## glvalue

Formal Definition: A glvalue is an expression whose evaluation determines the identity of an object or function.

Informal Definition: A glvalue is an actual object in your program, constructed with a constructor call if its type is not trivially constructible. (The constructor doesn’t have to have returned yet.) The specification says a glvalue’s “evaluation determines the identity of an object, bit field, or function.” This means you can generally take a glvalue’s address (except bitfields). You can also assign to a non-const glvalue unless it is a function or a user-defined class with a deleted or inaccessible operator=.

## prvalue

Formal Definition: A prvalue is an expression whose evaluation initializes an object or computes the value of an operand of an operator, as specified by the context in which it appears, or an expression that has type cv void.


Informal Definition: As of C++17, a prvalue (“pure rvalue”) of type T is an abstract recipe for initializing an object of type T (unless T is void). A prvalue does not correspond to an actual object of type T in your program. Nor does it require constructor invocation. Literal constants such as 5, true, nullptr, and enum tags are prvalues because they initialize objects and operands. For example, the prvalue 5 initializes x in “int x = 5”, initializes the right-hand operand of + in “y + 5”, and initializes the function argument in “std::to_string(5)”. You cannot modify a prvalue (true = 1 [wrong]) or take its address (&this [wrong]).

When you write “auto s = std::string("hello world");”, the cast expression std::string("hello world") is a prvalue. Evaluating the prvalue does not create a string object or invoke the std::string(const char*) constructor. If it did, s would need to be move-constructed from the already-constructed prvalue. Instead, s is constructed directly from the argument "hello world". The object that is ultimately initialized by a prvalue (in this case s) is known as the prvalue’s result object, and the value used to initialize the result object is the prvalue’s result.5

Compilers try to defer “materializing” prvalues as long as possible to avoid unnecessary moves and copies, particularly when handling function return values. A prvalue must eventually be materialized even if its value is discarded, however, so deferring materialization can elide only copy and move constructors, not other constructors.

Also non-class rvalues are not meant to occupy the stroage and hence have address
const int MAX = 100 //complier will avoid generating storage even for const objects, unless its address is erquired somehwere!
&MAX//oh now itis address is required so now MAX needs storage


### new in C++17

C++17 takes a different approach. Instead of guaranteeing that copies will be elided in these cases, it changes the rules such that the copies were never there in the first place. This is achieved through redefining when temporaries are created.

As noted in the value category descriptions earlier, prvalues exist for purposes of initialization. C++11 creates temporaries eagerly, eventually using them in an initialization and cleaning up copies after the fact. In C++17, the materialization of temporaries is deferred until the initialization is performed.

That’s a better name for this feature. Not guaranteed copy elision. Deferred temporary materialization.

Temporary materialization creates a temporary object from a prvalue, resulting in an xvalue. The most common places it occurs are when binding a reference to or performing member access on a prvalue. If a reference is bound to the prvalue, the materialized temporary’s lifetime is extended to that of the reference (this is unchanged from C++11, but worth repeating). If a prvalue initializes a class type of the same type as the prvalue, then the destination object is initialized directly; no temporary required

In C++17, however, std::string{"a pony"} is the Platonic ideal of “a pony”. It’s not a real object in C++’s object model, it’s some elusive, amorphous idea which can be passed around your program, only being given form when initializing some result object, or materializing a temporary. C++17’s prvalues are purer prvalues


A more complex example:

```
std::string a() {
    return "a pony";
}

std::string b() {
    return a();
}

int main() {
    auto x = b();
}
```

In the C++11 model, return "a pony"; initializes the temporary return object of a(), which move-constructs the temporary return object of b(), which move-constructs x. All the moves are likely elided by the compiler.

In the C++17 model, return "a pony"; initializes the result object of a(), which is the result object of b(), which is x.

In essence, rather than an initializer creating a series of temporaries which in theory move-construct a chain of return objects, the initializer is teleported to the eventual result object.


The “guaranteed copy elision” rules do not guarantee copy elision; instead they purify prvalues such that the copy doesn’t exist in the first place. Next time you hear or read about “guaranteed copy elision”, think instead about deferred temporary materialization

## xvalue

An xvalue is a glvalue that denotes an object whose resources can be reused (usually because it is near the end of its lifetime).

An xvalue (“expiring glvalue”) is a glvalue whose value will soon not matter, for instance because it is a temporary object about to be destroyed at the end of the current full-expression. Xvalues are what make move construction possible: if you no longer care about the contents of an expiring object, you can often move its contents into another object much more efficiently than if you needed to preserve the expiring object’s value. As you might expect, std::move transforms its argument into an xvalue.

## lvalue

 An lvalue is a glvalue that is not an xvalue. derefrencing pointer or taking address generates lvalue!

## rvalue
 An rvalue is a prvalue or an xvalue.



An expression is an xvalue if it is:
  (4.1) a move-eligible id-expression ([expr.prim.id.unqual]),
  (4.2) the result of calling a function, whether implicitly or explicitly, whose return type is an rvalue reference to object type ([expr.call]),
  (4.3) a cast to an rvalue reference to object type ([expr.type.conv], [expr.dynamic.cast], [expr.static.cast], [ expr.reinterpret.cast], [expr.const.cast], [expr.cast]),
  (4.4) a subscripting operation with an xvalue array operand ([expr.sub]),
  (4.5) a class member access expression designating a non-static data member of non-reference type in which the object expression is an xvalue ([expr.ref]), or
  (4.6) a .* pointer-to-member expression in which the first operand is an xvalue and the second operand is a pointer to data member ([expr.mptr.oper]).
  In general, the effect of this rule is that named rvalue references are treated as lvalues and unnamed rvalue references to objects are treated as xvalues; rvalue references to functions are treated as lvalues whether named or not. — end note]
  [Example 1:
  struct A {
  int m;
  };
  A&& operator+(A, A);
  A&& f();

  A a;
  A&& ar = static_cast<A&&>(a);
  The expressions f(), f().m, static_cast<A&&>(a), and a + a are xvalues. The expression ar is an lvalue. — end example]


***The result of a glvalue is the entity denoted by the expression.***

 ***The result of a prvalue is the value that the expression stores into its context***
 
 ***a prvalue that has type cv void has no result A prvalue whose result is the value V is sometimes said to have or name the value V***
 
 ***The result object of a prvalue is the object initialized by the prvalue; a non-discarded prvalue that is used to compute the value of an operand of a built-in operator or a prvalue that has type cv void has no result object***



Whenever a glvalue appears as an operand of an operator that requires a prvalue for that operand, the lvalue-to-rvalue ([conv.lval]), array-to-pointer ([conv.array]), or function-to-pointer ([conv.func]) standard conversions are applied to convert the expression to a prvalue

Example 1:
          
  ```
  struct S { int n; };
  auto f() {
  S x { 1 };
  constexpr S y { 2 };
  return [&](bool b) { return (b ? y : x).n; };
  }
  auto g = f();
  int m = g(false);   // undefined behavior: access of x.n outside its lifetime
  int n = g(true);    // OK, does not access y.n
  — end example]
  3
  ```

  The result of the conversion is determined according to the following rules:
  (3.1) If T is cv std​::​nullptr_t, the result is a null pointer constant ([conv.ptr]).
  [Note 1: Since the conversion does not access the object to which the glvalue refers, there is no side effect even if T is volatile-qualified ([intro.execution]), and the glvalue can refer to an inactive member of a union ([class.union]). — end note]
  (3.2) Otherwise, if T has a class type, the conversion copy-initializes the result object from the glvalue.
  (3.3) Otherwise, if the object to which the glvalue refers contains an invalid pointer value ([basic.stc.dynamic.deallocation]), the behavior is implementation-defined.
  (3.4) Otherwise, the object indicated by the glvalue is read ([defns.access]), and the value contained in the object is the prvalue result.
  4
  
  [Note 2: See also [basic.lval]. — end note]
        1)  For historical reasons, this conversion is called the “lvalue-to-rvalue” conversion, even though that name does not accurately reflect the taxonomy of expressions described in 

  Whenever a prvalue appears as an operand of an operator that expects a glvalue for that operand, the temporary materialization conversion is applied to convert the expression to an xvalue

"informal" definition : lvalue - is an expression which refers to an object

int n;

n = 1; //that is an lvalue expression consisting of 2 subexpresions:
  n  -> lvalue
  1 -> prvalue

Rationale behind having lvalues and rvalues:
    1) compiler got freedom to optimize because prvalues for example don't have associated storage
    2) complier forbids assigning to prvalues


note that complier assumes that for non-class types, rvalues don't occupy storage, but for class rvalues they do occupy
S {int x} a;
int i = S().a; //S() is used as rvalue expression, we don't say that S() is rvalue
for S().a to work, S() would need to have a base offset and that means it has to be stored somewhere


**If a function argument binds to an rvalue reference parameter, the implementation may assume that this parameter is a unique reference to this argument**


int&& xref = 5;
... creates a temporary, initialized with 5, whose lifetime is extended to the end of the block.
The assignment
xref = 10;
changes the value of the still living temporary.


## lvalue-to-rvalue conversion
- value indicated by lvalue is really rvalue result!
- in the context where rvalue is expected and lvalue given, than lvalue to rvalue conversion takes place
- on the left hand side, assignment expects rvalue 
    int i = 5; //i expects to be initialized by ralue, otherwise the code on this line will not make any sense
- expression e can be implicitly converted to type P. result is lvalue is e is lvalue and rvalue otherwise

## value category does not bind to lifetime

Guideline: Do not assume that “rvalues are short-lived,” nor that “everything sufficiently long-lived must be an lvalue.” Vice versa, do not assume that “lvalues are long-lived,” nor that “everything sufficiently short-lived must appear as an rvalue.”

Value category is not lifetime.

## forwarding reference

```
    const vector<string> verticies;    
    for (auto&& v : verticies)        
      static_assert(std::is_same_v<decltype(v), const string&>);
```    
   
```
    vector<string> verticies;    
    for (auto&& v : verticies)        
      static_assert(std::is_same_v<decltype(v), string&>);
```

## const can be dropped


A const T is a object of type T whose value cannot be modified. However, when T is a reference type, the const modifier is superfluous since references cannot be changed once initialized - they always refer to the same object. Thus, a const T when T=int& (it is kind of to say const (int&) - you see it looks as int& is const now , not int...)is just a T(which in this case is int&). 


The main point there is that the const in the original template belongs to T (because you say so in template <typename T> void test(const T& x)). When you instantiate with test<int&>, then T is int&. That turns the function parameter into const (int&) &. This const (int&) is the same as (int&) const, which is int & const, and there, const applies to & and is hence dropped



```
template <class T>
void f(const T v) {  //better mental mode is const (T) v
}

f<int&>(a) // T = int&, v type = int& const is dropped as refrences are const by definition! 

  int const &i10 = 3;
  const int *const ptr = &i10; // ptr itself is const, that is what refrences are actually!
  const int &const i11 = i10;// wont even compile because ref is const by definition


```

- IMPORTANT: There is one way to make const T& refrence like though!

```
std::add_lvalue_reference_t<std::add_const_t<std::remove_reference_t<in_type>>>
```

- Similarly by value T ommits const as well...

```
template <typename T> decltype(auto) any_value(T t) { return t; }
static_assert(std::is_same_v<int, decltype(any_value<const int>(i))>);
```

## Conversions

An expression’s value category determines what references the expression may initialize. Specifically, if T is a non-reference type, then:

T& can be initialized only from lvalues
T&& can be initialized only from rvalues
const T& can be initialized from any value category, but overload resolution will prefer T&& over const T& for rvalues if there are functions accepting both. (This is why copy constructors can fill the role of a missing move constructor.)


- xvalue can bind to CONST lvalue ref!
  
```  
f(const int &v)
f(std::move(r)) #this works!
```

Note that when binding a prvalue to a reference, it must be materialized into a temporary object. Generally, a temporary object is destroyed at the end of the full expression, which would leave a dangling reference. To avoid this, C++ extends the lifetime of temporary objects that are bound to references, so that they survive until the reference goes out of scope

## Other things

 - Return value optimization won't kick in and the value would be moved in the best case

```
auto f() {
  if (..) {
    return A();
  } else {
    return A();
  }
}
```

- no move from const
  const A v;
  std::move(v) #forbidden


- no auto move from lvalue's member!

```
struct A { int b; };
auto f() {
  return a.b; //no move here or value optimization, it would be copied!
  return std::move(a).b; //this would work!
}
```

- temporaries may dangle easy in generic code

```
template <class T>
decltype(auto) f(T&& v) {
  return v;
}

const A& v = f(A());// ***A() will be destroyed right after calling f, leaving v a dangling reference***
```

## tricky examples

struct S {
  static int static_member;
  int data_member = 0;
  int &lref = static_member;
  int &&rref = std::move(static_member);
};

S f();




By contrast, f().lref and f().rref both have int& exprtype because the ints they reference won’t be destroyed when the S object returned by f is destroyed. To push further on the file system analogy, destroying a class or array is like recursively deleting a directory—only the regular files in the directory will expire, not the files named by symbolic links in that directory.

## implementaitons 

note that foward forces you to provide Tp type as it won't be auto-deduced!

According to the rvalue reference proposal, a named rvalue is no different from an lvalue, except for decltype. That is any named parameter of a function cannot be implicitly casted or used to initialize another rvalue reference; it only copies to lvalue references; but static_cast can explicitly cast the valueness of the reference. So when you pass a named (in contrast to unnamed temporary) object/variable to a function, it can only be captured by lvlaues. To cast back a function argument to rvalue a static_cast is necessary, but it is too verbose about valueness of its output type. Any function such as std::forward has no way of deducing the decltype of its named operand. So the type must be verbosly passed as a template type argument; Correct deduction of return type of forward relies on its type argument. And the two overloads of this special function are necessary: the lvalue overload captures named rvalues and the rvalue overload captures unnamed lvalues who cannot directly bind to lvalue references. The deduction of underlying type of the input reference is prevented by using template type alises and stressed by nesting them in a trait type; either std::type_identity or std::remove_reference or else. This keeps programmers away from the pitfall of unintentionally passing wrong valueness while using std::forward

```
template<typename _Tp>
    _GLIBCXX_NODISCARD
    constexpr _Tp&&
    forward(typename std::remove_reference<_Tp>::type& __t) noexcept
    { return static_cast<_Tp&&>(__t); }

  template<typename _Tp>
    _GLIBCXX_NODISCARD
    constexpr _Tp&&
    forward(typename std::remove_reference<_Tp>::type&& __t) noexcept
    {
      static_assert(!std::is_lvalue_reference<_Tp>::value,
	  "std::forward must not be used to convert an rvalue to an lvalue");
      return static_cast<_Tp&&>(__t);
    }
  
  template<typename _Tp>
    _GLIBCXX_NODISCARD
    constexpr typename std::remove_reference<_Tp>::type&&
    move(_Tp&& __t) noexcept
    { return static_cast<typename std::remove_reference<_Tp>::type&&>(__t); }

```

Also note at this trick from unique_ptr. It tries to get rvalue / lvalue based on the Deleter type! get_deleter always returns a reference. However _ep could be reference / value itself . If it reference you'd just want to pass it along but if it is value, than it make sense to move it from!

```
      template<typename _Up, typename _Ep, typename = _Require<
               __safe_conversion_up<_Up, _Ep>,
	       typename conditional<is_reference<_Dp>::value,``
				    is_same<_Ep, _Dp>,
				    is_convertible<_Ep, _Dp>>::type>>
	unique_ptr(unique_ptr<_Up, _Ep>&& __u) noexcept
	: _M_t(__u.release(), std::forward<_Ep>(__u.get_deleter()))
	{ }
```
# std::initializer_list

an initializer list behaves like an array of const T – and since the elements are const, we cannot move them out of the array.

so be careful as this would cause vector to create 3 temporaries and copy them!
vector<string> v{"adsf", "asdf", "adsf"};


# std::move

explicitly move()-ing something that is already an prvalue won’t do anything good. (In fact, Clang with -Wall will warn about a pessemistic move in this case, which is rather nice.)



# Pointer ariphmetic 

***For addition or subtraction, if the expressions P or Q have type “pointer to cv T”, where T and the array element type are not similar, the behavior is undefined***
*[Note 2: In particular, a pointer to a base class cannot be used for pointer arithmetic when the array contains objects of a derived class type. — end note]*
80)
As specified in [basic.compound], an object that is not an array element is considered to belong to a single-element array for this purpose and a pointer past the last element of an array of n elements is considered to be equivalent to a pointer to a hypothetical array element n for this purpose. ⮥


 you cannot create an array from allocating two objects adjacently to each othe



** (AND ONLY AND ONLY ) For purposes of pointer arithmetic (7.6.6) and comparison (7.6.9, 7.6.10) **, a pointer past the end of the
last element of an array x of n elements is considered to be equivalent to a pointer to a hypothetical array
element n of x and an object of type T that is not an array element is considered to belong to an array
with one element of type T. The value representation of pointer types is implementation-defined. Pointers to
layout-compatible types shall have the same value representation and alignment requirements (6.7.6).
[Note 3 : Pointers to over-aligned types (6.7.6) have no special representation, but their range of valid values is
restricted by the extended alignment requirement. —end note]
//This is valid code!
Derived d;    
Derived* d4 = &(&d)[0];

conversions to void* and the other way around preserve pointer values

pointer to array, again not the same as pointer to the first element:
```
    int a[5];
    int (*ptr2)[] = &a;
    int* firstElement = a;
    int* firstElement2 = &a[0];
    assert(firstElement = firstElment2
    
    
    )
```

Because the comittee wants to make clear that an array is a low level concept an not a first class object: you cannot return an array nor assign to it for example. Pointer-interconvertibility is meant to be a concept between objects of same level: only standard layout classes or unions


# Accessing data

If a program attempts to access the stored value of an object through a glvalue whose type is not similar to one of the following types the behavior is undefined:55
(11.1) the dynamic type of the object,
(11.2) a type that is the signed or unsigned type corresponding to the dynamic type of the object, or
(11.3) a char, unsigned char, or std​::​byte type.


For example:

```
int main() {
    union U { int i; char c; };    
    U u{.i=1};

//although this would work in all compliers this is not correct by standard!
    char c1 = (char&)u;
    char c2 = (char&)u.i;
    char c3 = u.c; //reading  inactive member! bad
    return c1 + c2 + c3;
}
```

c1, c2, and c3 are obviously equivalent her
Note that this uses the "object representation" and aliasing special-cases for "char";
unfortunately, there are some specification holes in that area.


It is a known issue that the current wording of the standard does not technically permit a user to implement their own `memcpy`, because there is no provision in the language to be able to read a byte of an object representation through a char glvalue; see P1839 for discussion. The intent is to fix this wording gap eventually, but it's not so easy.

It is assumed that we will find some kind of solution to this problem, in which the naive strategy of `reinterpret_cast`ing to `char*` and then reading through that pointer will actually be well-defined and yield the object representation, and whatever the resolution is, it will be considered as a DR.

If that's the case, then you can use a `char` glvalue to read the object representation of a `U` object, notwithstanding the fact that the active member is something other than a `char`. In the meantime, you can rely on this not being UB for all practical purposes, even though the wording to make it well-defined has not been figured out yet.

![](images/issue_with_viewing_object_representation.PNG)

# Strinct Aliasing Rules

If a program attempts to access the stored value of an object through a glvalue of other than one of the following types the behavior is undefined:

(11.1) the dynamic type of the object,
(11.2) a cv-qualified version of the dynamic type of the object,
(11.3) a type similar (as defined in 7.5) to the dynamic type of the object,
(11.4) a type that is the signed or unsigned type corresponding to the dynamic type of the object,
(11.5) a type that is the signed or unsigned type corresponding to a cv-qualified version of the dynamic type of the object,
(11.6) an aggregate or union type that includes one of the aforementioned types among its elements or non-static data members (including, recursively, an element or non-static data member of a subaggregate or contained union),
(11.7) a type that is a (possibly cv-qualified) base class type of the dynamic type of the object,
(11.8) a char, unsigned char, or std::byte type.

# Binding

binding rules occur in the following events:
 return from function
 assignment / initialization 
 pass to function by value



# Temporary 


Even when the creation of the temporary object is unevaluated ([expr.context]), all the semantic restrictions shall be respected as if the temporary object had been created and later destroyed ----> EXCEPT <---------- if it used in decltype in that case semantic restrictions dont apply ----> 


A prvalue of type T can be converted to an xvalue of type T. This conversion initializes a temporary object ([class.temporary]) of type T from the prvalue by evaluating the prvalue with the temporary object as its result object, and produces an xvalue denoting the temporary object. T shall be a complete type.
[Note 1: If T is a class type (or array thereof), it must have an accessible and non-deleted destructor; see [class.dtor]. — end note]
[Example 1:
struct X {
  int n; };
int k = X().n;      // OK, X() prvalue is converted to xvalue


When an implementation introduces a temporary object of a class that has a non-trivial constructor ([class.default.ctor], [class.copy.ctor]), it shall ensure that a constructor is called for the temporary object. Similarly, the destructor shall be called for a temporary with a non-trivial destructor ([class.dtor]). Temporary objects are destroyed as the last step in evaluating the full-expression ([intro.execution]) that (lexically) contains the point where they were created. This is true even if that evaluation ends in throwing an exception. The value computations and side effects of destroying a temporary object are associated only with the full-expression, not with any specific subexpression

If a temporary object has a reference member initialized by another temporary object, lifetime extension applies only **ONCE** to such a member's initializer.
[Example 4:
struct S {
  const int &m;
};
const S& s = S{1};     //this is goodpotentially-overlapping subobject

except this! ->>>> 
- A temporary object bound to a reference parameter in a function call ([expr.call]) persists until the completion of the full-expression containing the call.
- A temporary object bound to a reference element of an aggregate of class type initialized from a parenthesized expression-list ([dcl.init]) persists until the completion of the full-expression containing the expression-list.
struct S {
  const int &m;
  S(const int &m) : m(m1) {}
};
const S& s = S(1);     //this is bad, dangling reference!



A temporary bound to a reference in a new-initializer ([expr.new]) persists until the completion of the full-expression containing the new-initializer.
[Note 7: This might introduce a dangling reference. — end note]
[Example 5:
struct S {
  int mi;
  const std::pair<int, int> &mp; };
S a { 1, {2,3} }; //ok
S* p = new S{ 1, {2,3} }; // dangling reference

the reason why temporary does not extend its lifetime in new experssion is the fact 

If the expression is a non-void prvalue (after any lvalue-to-rvalue conversion that might have taken place), temporary materialization DOES occurs.
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

# Lifetime extension

“Lifetime extension” is a weird little quirk of C++98 that is still alive and kicking in C++17. The basic idea is that you can write C++ code like this:

int main() {
    const int& three = 1 + 2;
    return three;
}
and it will just magically work. Sure, it looks like you’re taking a reference to a temporary, there; but actually behind the scenes the compiler will extend the lifetime of that temporary to match the scope of the reference variable to which it’s bound (namely, three).

This magic happens only when the compiler can see that you’re dealing with a temporary, a prvalue expression, of type exactly T. The magic will not happen if you are dealing with an expression of type T&&, or const T&, or anything else that might conceivably be an xvalue. Thus:

int foo(const int& x) { return x; }
int a        = foo(42);  // OK
const int& b = foo(42);  // OK

const int& bar(const int& y) { return y; }
int c        = bar(42);  // OK




# consteval 

specifies that a function is an immediate function, that is, every call to the function must produce a compile-time constant


# implicit conversions
expression e implictily converted to type T if Ts copy initialization is valid. Implicit converions only works at one level

struct A{ 
  A(int){}
  std::string operator() { return "asd";
}
}
;
struct A1 {
  A1(std::string) {}
}

A1 a(3) // will not work as 2 level conversion


# initialization

## Important take aways:

 - since C++20 direct aggregate initialization is now possible, though it would allow narrowing conversions and won't extend the liftime of a refrence. Something like this is possible now
 struct A { int i; int j;  };  make_shared<A>(1, 2);

 - in aggregate initialization if a suboject is not initialized, then its default value is taken and if that is not provided than it is value initialized

 - basically during value initialization if the object's defualt constructor is not user-provider and not deleted and also trivial than zero initialization during value initialization is possible, otherwise the object would be defualt initialized

 - Aggregate initialization would copy initializate explicitly initialized members and direct initializate members with designated initializer, the rest would be initialized as if they were with {}
 

 - expression e implicitly converts to T if copy initialization works -> T a = e;

   T object{};
(since C++ 11) - BE CAREFULL FOR AGGREGATES IT WOULD CALL AGGREGATE
        INITIALIZATION which has these rules so explicit defult constructors may
        not work !explicit constructors
    and conversion operators wont work in aggregate initialization
    Also note that arrays are aggregates!


- An object whose initialization has completed is deemed to be constructed, even if the object is of non-class type or no constructor of the object's class is invoked for the initialization. [Note 9: Such an object might have been value-initialized or initialized by aggregate initialization ([dcl.init.aggr]) or by an inherited constructor ([class.inhctor.init]). — end note]

IMPORTANT !!! **If the initializer is a parenthesized expression-list, the expressions are evaluated in the order specified for function calls ([expr.call])**

IMPORTANT !!!  **Within the initializer-list of a braced-init-list, the initializer-clauses, including any that result from pack expansions ([temp.variadic]), are evaluated in the order in which they appear.**


- Destroying an object of class type invokes the destructor of the class. Destroying a scalar type has no effect other than ending the lifetime of the object ([basic.life]). Destroying an array destroys each element in reverse subscript order.


## zero-initialize 

To zero-initialize an object or reference of type T means:
(6.1) if T is a scalar type ([basic.types.general]), the object is initialized to the value obtained by converting the integer literal 0 (zero) to T;82
(6.2) if T is a (possibly cv-qualified) non-union class type, its padding bits ([basic.types.general]) are initialized to zero bits and each non-static data member, each non-virtual base class subobject, and, if the object is not a base class subobject, each virtual base class subobject is zero-initialized;
(6.3) if T is a (possibly cv-qualified) union type, its padding bits ([basic.types.general]) are initialized to zero bits and the object's first non-static named data member is zero-initialized;
(6.4) if T is an array type, each element is zero-initialized;
(6.5) if T is a reference type, no initialization is performed.

zero initialization does not mean litterly memset(0)! it could be in fact set to all bits 1! For example pointers to member objects normally set to 0xffffff...



## default-initialize 

To default-initialize an object of type T means:
(7.1) If T is a (possibly cv-qualified) class type ([class]), constructors are considered. The applicable constructors are enumerated ([over.match.ctor]), and the best one for the initializer () is chosen through overload resolution ([over.match]). The constructor thus selected is called, with an empty argument list, to initialize the object.
(7.2) If T is an array type, the semantic constraints of default-initializing a hypothetical element shall be met and each element is default-initialized.
(7.3) Otherwise, no initialization is performed.

A class type T is const-default-constructible if default-initialization of T would invoke a user-provided constructor of T (not inherited from a base class) or if
(8.1) each direct non-variant non-static data member M of T has a default member initializer or, if M is of class type X (or array thereof), X is const-default-constructible,
(8.2) if T is a union with at least one non-static data member, exactly one variant member has a default member initializer,
(8.3) if T is not a union, for each anonymous union member with at least one non-static data member (if any), exactly one non-static data member has a default member initializer, and
(8.4) each potentially constructed base class of T is const-default-constructible.
If a program calls for the default-initialization of an object of a const-qualified type T, T shall be a const-default-constructible class type or array thereof.


## value-initialize

To value-initialize an object of type T means:
(9.1) If T is a (possibly cv-qualified) class type ([class]), then
(9.1.1) if T has either no default constructor (which pretty much means default-initialization would fail) ([class.default.ctor]) or a default constructor that is user-provided or deleted, then the object is default-initialized;
(9.1.2) otherwise, the object is zero-initialized and the semantic constraints for default-initialization are checked, and if T has a non-trivial default constructor, the object is default-initialized.
(9.2) If T is an array type, the semantic constraints of value-initializing a hypothetical element shall be met and each element is value-initialized.
(9.3) Otherwise, the object is zero-initialized.


## other

- A program that calls for default-initialization or value-initialization of an entity of reference type is ill-formed.
- [Note 4: For every object of static storage duration, static initialization ([basic.start.static]) is performed at program startup before any other initialization takes place. In some cases, additional initialization is done later. — end note]
- if no initializer is specified for an object, the object is default-initialized.
- If the entity being initialized does not have class or array type, the expression-list in a parenthesized initializer shall be a single expression.

## copy-initialization

 The initialization that occurs in the = form of a brace-or-equal-initializer or condition ([stmt.select]), as well as in argument passing, function return, throwing an exception ([except.throw]), handling an exception ([except.handle]), and aggregate member initialization other than by a designated-initializer-clause ([dcl.init.aggr]), is called copy-initialization.
[Note 5: Copy-initialization can invoke a move ([class.copy.ctor]). — end note]

## direct-initialization

 The initialization that occurs
(15.1) for an initializer that is a parenthesized expression-list or a braced-init-list (aka {1,2, 3, 4}),
(15.2) for a new-initializer ([expr.new]),
(15.3) in a static_cast expression ([expr.static.cast]),
(15.4) in a functional notation type conversion ([expr.type.conv]), and
(15.5) in the braced-init-list form of a condition
is called **direct-initialization**.

## list-initialization

List-initialization is initialization of an object or reference from a braced-init-list. Such an initializer is called an initializer list, and the comma-separated initializer-clauses of the initializer-list or designated-initializer-clauses of the designated-initializer-list are called the elements of the initializer list. An initializer list may be empty. List-initialization can occur in direct-initialization or copy-initialization contexts; list-initialization in a direct-initialization context is called direct-list-initialization and list-initialization in a copy-initialization context is called copy-list-initialization. Direct-initialization that is not list-initialization is called direct-non-list-initialization.
[Note 1: List-initialization can be used
(1.1) as the initializer in a variable definition ([dcl.init])
(1.2) as the initializer in a new-expression ([expr.new])
(1.3) in a return statement ([stmt.return])
(1.4) as a for-range-initializer ([stmt.iter])
(1.5) as a function argument ([expr.call])
(1.6) as a template argument ([temp.arg.nontype])
(1.7) as a subscript ([expr.sub])
(1.8) as an argument to a constructor invocation ([dcl.init], [expr.type.conv])
(1.9) as an initializer for a non-static data member ([class.mem])
(1.10) in a mem-initializer ([class.base.init])
(1.11) on the right-hand side of an assignment ([expr.ass])
[Example 1: 
```
int a = {1};
std::complex<double> z{1,2};
new std::vector<std::string>{"once", "upon", "a", "time"};  // 4 string elements
f( {"Nicholas","Annemarie"} );  // pass list of two elements
return { "Norah" };             // return list of one element
int* e {};                      // initialization to zero / null pointer
x = double{1};                  // explicitly construct a double
std::map<std::string,int> anim = { {"bear",4}, {"cassowary",2}, {"tiger",7} };
— end example]
— end note]
```

A constructor is an initializer-list constructor if its first parameter is of type std​::​initializer_list<E> or reference to cv std​::​initializer_list<E> for some type E, and either there are no other parameters or else all other parameters have default arguments ([dcl.fct.default]).
[Note 2: Initializer-list constructors are favored over other constructors in list-initialization ([over.match.list]). Passing an initializer list as the argument to the constructor template template<class T> C(T) of a class C does not create an initializer-list constructor, because an initializer list argument causes the corresponding parameter to be a non-deduced context ([temp.deduct.call]). — end note]
The template std​::​initializer_list is not predefined; if a standard library declaration ([initializer.list.syn], [std.modules]) of std​::​initializer_list is not reachable from ([module.reach]) a use of std​::​initializer_list — even an implicit use in which the type is not named ([dcl.spec.auto]) — the program is ill-formed.

List-initialization of an object or reference of type T is defined as follows:
(3.1) If the braced-init-list contains a designated-initializer-list and T is not a reference type, T shall be an aggregate class. The ordered identifiers in the designators of the designated-initializer-list shall form a subsequence of the ordered identifiers in the direct non-static data members of T. Aggregate initialization is performed ([dcl.init.aggr]).
[Example 2: 
```
struct A { int x; int y; int z; };
A a{.y = 2, .x = 1};                // error: designator order does not match declaration order
A b{.x = 1, .z = 2};                // OK, b.y initialized to 0
```
— end example]
(3.2) If T is an aggregate class and the initializer list has a single element of type cv U, where U is T or a class derived from T, the object is initialized from that element (by copy-initialization for copy-list-initialization, or by direct-initialization for direct-list-initialization).
(3.3) Otherwise, if T is a character array and the initializer list has a single element that is an appropriately-typed string-literal ([dcl.init.string]), initialization is performed as described in that subclause.
(3.4) Otherwise, if T is an aggregate, aggregate initialization is performed ([dcl.init.aggr]).
[Example 3: 
double ad[] = { 1, 2.0 };           // OK
int ai[] = { 1, 2.0 };              // error: narrowing

struct S2 {
  int m1;
  double m2, m3;
};
S2 s21 = { 1, 2, 3.0 };             // OK
S2 s22 { 1.0, 2, 3 };               // error: narrowing
S2 s23 { };                         // OK, default to 0,0,0
— end example]
(3.5) Otherwise, if the initializer list has no elements and T is a class type with a default constructor, the object is value-initialized.
(3.6) Otherwise, if T is a specialization of std​::​initializer_list<E>, the object is constructed as described below.
(3.7) Otherwise, if T is a class type, constructors are considered. The applicable constructors are enumerated and the best one is chosen through overload resolution ([over.match], [over.match.list]). If a narrowing conversion (see below) is required to convert any of the arguments, the program is ill-formed.
[Example 4: 
```
struct S {
  S(std::initializer_list<double>); // #1
  S(std::initializer_list<int>);    // #2
  S();                              // #3
  // ...
};
S s1 = { 1.0, 2.0, 3.0 };           // invoke #1
S s2 = { 1, 2, 3 };                 // invoke #2
S s3 = { };                         // invoke #3
— end example]
[Example 5: 
struct Map {
  Map(std::initializer_list<std::pair<std::string,int>>);
};
Map ship = {{"Sophie",14}, {"Surprise",28}};
— end example]
[Example 6: 
struct S {
  // no initializer-list constructors
  S(int, double, double);           // #1
  S();                              // #2
  // ...
};
S s1 = { 1, 2, 3.0 };               // OK, invoke #1
S s2 { 1.0, 2, 3 };                 // error: narrowing
S s3 { };                           // OK, invoke #2
```
— end example]
(3.8) Otherwise, if T is an enumeration with a fixed underlying type ([dcl.enum]) U, the initializer-list has a single element v of scalar type, v can be implicitly converted to U, and the initialization is direct-list-initialization, the object is initialized with the value T(v) ([expr.type.conv]); if a narrowing conversion is required to convert v to U, the program is ill-formed.
[Example 7: 

```
enum byte : unsigned char { };
byte b { 42 };                      // OK
byte c = { 42 };                    // error
byte d = byte{ 42 };                // OK; same value as b
byte e { -1 };                      // error

struct A { byte b; };
A a1 = { { 42 } };                  // error
A a2 = { byte{ 42 } };              // OK

void f(byte);
f({ 42 });                          // error

enum class Handle : uint32_t { Invalid = 0 };
Handle h { 42 };                    // OK
```

— end example]
(3.9) Otherwise, if the initializer list is not a designated-initializer-list and has a single element of type E and either T is not a reference type or its referenced type is reference-related to E, the object or reference is initialized from that element (by copy-initialization for copy-list-initialization, or by direct-initialization for direct-list-initialization); if a narrowing conversion (see below) is required to convert the element to T, the program is ill-formed.
[Example 8: 
int x1 {2};                         // OK
int x2 {2.0};                       // error: narrowing
— end example]
(3.10) Otherwise, if T is a reference type, a prvalue is generated. The prvalue initializes its result object by copy-list-initialization from the initializer list. The prvalue is then used to direct-initialize the reference. The type of the prvalue is the type referenced by T, unless T is “reference to array of unknown bound of U”, in which case the type of the prvalue is the type of x in the declaration U x[] H, where H is the initializer list.
[Note 3: As usual, the binding will fail and the program is ill-formed if the reference type is an lvalue reference to a non-const type. — end note]
[Example 9: 
```
struct S {
  S(std::initializer_list<double>); // #1
  S(const std::string&);            // #2
  // ...
};
const S& r1 = { 1, 2, 3.0 };        // OK, invoke #1
const S& r2 { "Spinach" };          // OK, invoke #2
S& r3 = { 1, 2, 3 };                // error: initializer is not an lvalue
const int& i1 = { 1 };              // OK
const int& i2 = { 1.1 };            // error: narrowing
const int (&iar)[2] = { 1, 2 };     // OK, iar is bound to temporary array

struct A { } a;
struct B { explicit B(const A&); };
const B& b2{a};                     // error: cannot copy-list-initialize B temporary from A

struct C { int x; };
C&& c = { .x = 1 };                 // OK
```

— end example]
(3.11) Otherwise, if the initializer list has no elements, the object is value-initialized.
[Example 10: 
int** pp {};                        // initialized to null pointer
— end example]
(3.12) Otherwise, the program is ill-formed.
```
[Example 11: 
struct A { int i; int j; };
A a1 { 1, 2 };                      // aggregate initialization
A a2 { 1.2 };                       // error: narrowing
struct B {
  B(std::initializer_list<int>);
};
B b1 { 1, 2 };                      // creates initializer_list<int> and calls constructor
B b2 { 1, 2.0 };                    // error: narrowing
struct C {
  C(int i, double j);
};
C c1 = { 1, 2.2 };                  // calls constructor with arguments (1, 2.2)
C c2 = { 1.1, 2 };                  // error: narrowing

int j { 1 };                        // initialize to 1
int k { };                          // initialize to 0
```
— end example]

Within the initializer-list of a braced-init-list, the initializer-clauses, including any that result from pack expansions ([temp.variadic]), are evaluated in the order in which they appear. That is, every value computation and side effect associated with a given initializer-clause is sequenced before every value computation and side effect associated with any initializer-clause that follows it in the comma-separated list of the initializer-list.
[Note 4: This evaluation ordering holds regardless of the semantics of the initialization; for example, it applies when the elements of the initializer-list are interpreted as arguments of a constructor call, even though ordinarily there are no sequencing constraints on the arguments of a call. — end note]

An object of type std​::​initializer_list<E> is constructed from an initializer list as if the implementation generated and materialized ([conv.rval]) a prvalue of type “array of N const E”, where N is the number of elements in the initializer list; this is called the initializer list's backing array. Each element of the backing array is copy-initialized with the corresponding element of the initializer list, and the std​::​initializer_list<E> object is constructed to refer to that array.
[Note 5: A constructor or conversion function selected for the copy is required to be accessible ([class.access]) in the context of the initializer list. — end note]
If a narrowing conversion is required to initialize any of the elements, the program is ill-formed.
[Note 6: Backing arrays are potentially non-unique objects ([intro.object]). — end note]

The backing array has the same lifetime as any other temporary object ([class.temporary]), except that initializing an initializer_list object from the array extends the lifetime of the array exactly like binding a reference to a temporary

# aggregate initialization

An **aggregate** is an array or a class ([class]) with
(1.1) no user-declared or inherited constructors ([class.ctor]),
(1.2) no private or protected direct non-static data members ([class.access]),
(1.3) no private or protected direct base classes ([class.access.base]), and
(1.4) no virtual functions ([class.virtual]) or virtual base classes ([class.mi]).
[Note 1: Aggregate initialization does not allow accessing protected and private base class' members or constructors. — end note]

The elements of an aggregate are:
(2.1) for an array, the array elements in increasing subscript order, or
(2.2) for a class, the direct base classes in declaration order, followed by the direct non-static data members ([class.mem]) that are not members of an anonymous union, in declaration order.

When an aggregate is initialized by an initializer list as specified in [dcl.init.list], the elements of the initializer list are taken as initializers for the elements of the aggregate. The explicitly initialized elements of the aggregate are determined as follows:
(3.1) If the initializer list is a brace-enclosed designated-initializer-list, the aggregate shall be of class type, the identifier in each designator shall name a direct non-static data member of the class, and the explicitly initialized elements of the aggregate are the elements that are, or contain, those members.
(3.2) If the initializer list is a brace-enclosed initializer-list, the explicitly initialized elements of the aggregate are the first n elements of the aggregate, where n is the number of elements in the initializer list.
(3.3) Otherwise, the initializer list must be {}, and there are no explicitly initialized elements.

For each explicitly initialized element:
(4.1) If the element is an anonymous union member and the initializer list is a brace-enclosed designated-initializer-list, the element is initialized by the braced-init-list { D }, where D is the designated-initializer-clause naming a member of the anonymous union member. There shall be only one such designated-initializer-clause.
[Example 1: 
```
struct C {
  union {
    int a;
    const char* p;
  };
  int x;
} c = { .a = 1, .x = 3 };
```
initializes c.a with 1 and c.x with 3. — end example]
(4.2) Otherwise, the element is copy-initialized from the corresponding initializer-clause or is initialized with the brace-or-equal-initializer of the corresponding designated-initializer-clause. If that initializer is of the form assignment-expression or = assignment-expression and a narrowing conversion ([dcl.init.list]) is required to convert the expression, the program is ill-formed.
[Note 2: If the initialization is by designated-initializer-clause, its form determines whether copy-initialization or direct-initialization is performed. — end note]
[Note 3: If an initializer is itself an initializer list, the element is list-initialized, which will result in a recursive application of the rules in this subclause if the element is an aggregate. — end note]
[Example 2: 
```
struct A {
  int x;
  struct B {
    int i;
    int j;
  } b;
} a = { 1, { 2, 3 } };
initializes a.x with 1, a.b.i with 2, a.b.j with 3.
struct base1 { int b1, b2 = 42; };
struct base2 {
  base2() {
    b3 = 42;
  }
  int b3;
};
struct derived : base1, base2 {
  int d;
};

derived d1{{1, 2}, {}, 4};
derived d2{{}, {}, 4};
```
initializes d1.b1 with 1, d1.b2 with 2, d1.b3 with 42, d1.d with 4, and d2.b1 with 0, d2.b2 with 42, d2.b3 with 42, d2.d with 4. — end example]


For a non-union aggregate, each element that is not an explicitly initialized element is initialized as follows:
(5.1) If the element has a default member initializer ([class.mem]), the element is initialized from that initializer.
(5.2) Otherwise, if the element is not a reference, the element is copy-initialized from an empty initializer list ([dcl.init.list]).
(5.3) Otherwise, the program is ill-formed.
If the aggregate is a union and the initializer list is empty, then
(5.4) if any variant member has a default member initializer, that member is initialized from its default member initializer;
(5.5) otherwise, the first member of the union (if any) is copy-initialized from an empty initializer list.

[Example 3: 
```
struct S { int a; const char* b; int c; int d = b[a]; };
S ss = { 1, "asdf" };
initializes ss.a with 1, ss.b with "asdf", ss.c with the value of an expression of the form int{} (that is, 0), and ss.d with the value of ss.b[ss.a] (that is, 's'), and in
struct X { int i, j, k = 42; };
X a[] = { 1, 2, 3, 4, 5, 6 };
X b[2] = { { 1, 2, 3 }, { 4, 5, 6 } };
a and b have the same value
struct A {
  string a;
  int b = 42;
  int c = -1;
};
```
A{.c=21} has the following steps:
(6.1) Initialize a with {}
(6.2) Initialize b with = 42
(6.3) Initialize c with = 21
— end example]

The initializations of the elements of the aggregate are evaluated in the element order. That is, all value computations and side effects associated with a given element are sequenced before those of any element that follows it in order.

An aggregate that is a class can also be initialized with a single expression not enclosed in braces, as described in [dcl.init].

The destructor for each element of class type other than an anonymous union member is potentially invoked ([class.dtor]) from the context where the aggregate initialization occurs.
[Note 4: This provision ensures that destructors can be called for fully-constructed subobjects in case an exception is thrown ([except.ctor]). — end note]

An array of unknown bound initialized with a brace-enclosed initializer-list containing n initializer-clauses is defined as having n elements ([dcl.array]).
[Example 4: 
int x[] = { 1, 3, 5 };
declares and initializes x as a one-dimensional array that has three elements since no size was specified and there are three initializers. — end example]
An array of unknown bound shall not be initialized with an empty braced-init-list {}.83
[Note 5: A default member initializer does not determine the bound for a member array of unknown bound. Since the default member initializer is ignored if a suitable mem-initializer is present ([class.base.init]), the default member initializer is not considered to initialize the array of unknown bound.
[Example 5: 
struct S {
  int y[] = { 0 };          // error: non-static data member of incomplete type
};
— end example]
— end note]

[Note 6: Static data members, non-static data members of anonymous union members, and unnamed bit-fields are not considered elements of the aggregate.
[Example 6: 
```
struct A {
  int i;
  static int s;
  int j;
  int :17;
  int k;
} a = { 1, 2, 3 };
```
Here, the second initializer 2 initializes a.j and not the static data member A​::​s, and the third initializer 3 initializes a.k and not the unnamed bit-field before it. — end example]
— end note]


When initializing a multidimensional array, the initializer-clauses initialize the elements with the last (rightmost) index of the array varying the fastest ([dcl.array]).
[Example 10: 
int x[2][2] = { 3, 1, 4, 2 };
initializes x[0][0] to 3, x[0][1] to 1, x[1][0] to 4, and x[1][1] to 2. On the other hand,
float y[4][3] = {
  { 1 }, { 2 }, { 3 }, { 4 }
};
initializes the first column of y (regarded as a two-dimensional array) and leaves the rest zero. — end example]

All implicit type conversions ([conv]) are considered when initializing the element with an assignment-expression. If the assignment-expression can initialize an element, the element is initialized. Otherwise, if the element is itself a subaggregate, brace elision is assumed and the assignment-expression is considered for the initialization of the first element of the subaggregate.
[Note 7: As specified above, brace elision cannot apply to subaggregates with no elements; an initializer-clause for the entire subobject is required. — end note]
[Example 12: 
```
struct A {
  int i;
  operator int();
};
struct B {
  A a1, a2;
  int z;
};
A a;
B b = { 4, a, a };
```
Braces are elided around the initializer-clause for b.a1.i. b.a1.i is initialized with 4, b.a2 is initialized with a, b.z is initialized with whatever a.operator int() returns. — end example]

[Note 8: An aggregate array or an aggregate class can contain elements of a class type with a user-declared constructor ([class.ctor]). Initialization of these aggregate objects is described in [class.expl.init]. — end note]

[Note 9: Whether the initialization of aggregates with static storage duration is static or dynamic is specified in [basic.start.static], [basic.start.dynamic], and [stmt.dcl]. — end note]

When a union is initialized with an initializer list, there shall not be more than one explicitly initialized element.
[Example 13: 
```
union u { int a; const char* b; };
u a = { 1 };
u b = a;
u c = 1;                        // error
u d = { 0, "asdf" };            // error
u e = { "asdf" };               // error
u f = { .b = "asdf" };
u g = { .a = 1, .b = "asdf" };  // error
```
[Note 10: As described above, the braces around the initializer-clause for a union member can be omitted if the union is a member of another aggregate. — end note]

# initializer algorithm

The semantics of initializers are as follows. The destination type is the type of the object or reference being initialized and the source type is the type of the initializer expression. If the initializer is not a single (possibly parenthesized) expression, the source type is not defined.
(16.1) If the initializer is a (non-parenthesized) braced-init-list or is = braced-init-list, the object or reference is **list-initialized** ([dcl.init.list]).
(16.2) If the destination type is a reference type, see [dcl.init.ref].
(16.3) If the destination type is an array of characters, an array of char8_t, an array of char16_t, an array of char32_t, or an array of wchar_t, and the initializer is a string-literal, see [dcl.init.string].
(16.4) If the initializer is (), the object is value-initialized.
[Note 6: Since () is not permitted by the syntax for initializer,
X a();
is not the declaration of an object of class X, but the declaration of a function taking no arguments and returning an X. The form () is permitted in certain other initialization contexts ([expr.new], [expr.type.conv], [class.base.init]). — end note]
(16.5) Otherwise, if the destination type is an array, the object is initialized as follows. Let 
 be the elements of the expression-list. If the destination type is an array of unknown bound, it is defined as having k elements. Let n denote the array size after this potential adjustment. If k is greater than n, the program is ill-formed. Otherwise, the ith array element is copy-initialized with xi for each 1  ≤ i  ≤ k, and value-initialized for each k<i≤n. For each 1≤i<j≤n, every value computation and side effect associated with the initialization of the ith element of the array is sequenced before those associated with the initialization of the jth element.
(16.6) Otherwise, if the destination type is a (possibly cv-qualified) class type:
(16.6.1) If the initializer expression is a prvalue and the cv-unqualified version of the source type is the same class as the class of the destination, the initializer expression is used to initialize the destination object.
[Example 2: T x = T(T(T())); value-initializes x. — end example]
(16.6.2) Otherwise, if the initialization is direct-initialization, or if it is copy-initialization where the cv-unqualified version of the source type is the same class as, or a derived class of, the class of the destination, constructors are considered. The applicable constructors are enumerated ([over.match.ctor]), and the best one is chosen through overload resolution ([over.match]). Then:
(16.6.2.1) If overload resolution is successful, the selected constructor is called to initialize the object, with the initializer expression or expression-list as its argument(s).
(16.6.2.2) Otherwise, if no constructor is viable, the destination type is an aggregate class, and the initializer is a parenthesized expression-list, the object is initialized as follows. Let e1, …, en be the elements of the aggregate ([dcl.init.aggr]). Let x1, …, xk be the elements of the xpression-list. If k is greater than n, the program is ill-formed. The element ei is copy-initialized with xi for 1  ≤ i  ≤ k. The remaining elements are initialized with their default member initializers, if any, and otherwise are value-initialized. For each 1≤i<j≤n, every value computation and side effect associated with the initialization of ei is sequenced before those associated with the initialization of ej.
[Note 7: By contrast with direct-list-initialization, narrowing conversions ([dcl.init.list]) are permitted, designators are not permitted, a temporary object bound to a reference does not have its lifetime extended ([class.temporary]), and there is no brace elision.
[Example 3: 

```
struct A {
  int a;
  int&& r;
};

int f();
int n = 10;

A a1{1, f()};                   // OK, lifetime is extended
A a2(1, f());                   // well-formed, but dangling reference
A a3{1.0, 1};                   // error: narrowing conversion
A a4(1.0, 1);                   // well-formed, but dangling reference
A a5(1.0, std::move(n));        // OK
```

(16.6.2.3) Otherwise, the initialization is ill-formed.
(16.6.3) Otherwise (i.e., for the remaining copy-initialization cases), user-defined conversions that can convert from the source type to the destination type or (when a conversion function is used) to a derived class thereof are enumerated as described in [over.match.copy], and the best one is chosen through overload resolution ([over.match]). If the conversion cannot be done or is ambiguous, the initialization is ill-formed. The function selected is called with the initializer expression as its argument; if the function is a constructor, the call is a prvalue of the cv-unqualified version of the destination type whose result object is initialized by the constructor. The call is used to direct-initialize, according to the rules above, the object that is the destination of the copy-initialization.
(16.7) Otherwise, if the source type is a (possibly cv-qualified) class type, conversion functions are considered. The applicable conversion functions are enumerated ([over.match.conv]), and the best one is chosen through overload resolution ([over.match]). The user-defined conversion so selected is called to convert the initializer expression into the object being initialized. If the conversion cannot be done or is ambiguous, the initialization is ill-formed.
(16.8) Otherwise, if the initialization is direct-initialization, the source type is std​::​nullptr_t, and the destination type is bool, the initial value of the object being initialized is false.
(16.9) Otherwise, the initial value of the object being initialized is the (possibly converted) value of the initializer expression. A standard conversion sequence ([conv]) is used to convert the initializer expression to a prvalue of the cv-unqualified version of the destination type; no user-defined conversions are considered. If the conversion cannot be done, the initialization is ill-formed. When initializing a bit-field with a value that it cannot represent, the resulting value of the bit-field is implementation-defined.
[Note 8: An expression of type “cv1 T” can initialize an object of type “cv2 T” independently of the cv-qualifiers cv1 and cv2.

# conditional operator

 Conditional operator[expr.cond]
conditional-expression:
	logical-or-expression
	logical-or-expression ? expression : assignment-expression

 The first expression is contextually converted to bool. It is evaluated and if it is true, the result of the conditional expression is the value of the second expression, otherwise that of the third expression. Only one of the second and third expressions is evaluated. The first expression is sequenced before the second or third expression

 
## decltype

- Value category is just synonymous with the reference qualification on expression decltype-

- no deduction inside decltype!
  template<typename T> void f(decltype(*std::declval<T>()) arg);
  not GONNA WORK!

IMPORTANT: **Not working on s.method, s.*method_ptr, p->method, p->*method_ptr)

decltype-specifier:
	decltype ( expression )

For an expression E, the type denoted by decltype(E) is defined as follows:
(1.1) if E is an unparenthesized id-expression naming a structured binding ([dcl.struct.bind]), decltype(E) is the referenced type as given in the specification of the structured binding declaration;
(1.2) otherwise, if E is an unparenthesized id-expression naming a non-type template-parameter, decltype(E) is the type of the template-parameter after performing any necessary type deduction ([dcl.spec.auto], [dcl.type.class.deduct]);
(1.3) otherwise, if E is an unparenthesized id-expression or an unparenthesized class member access ([expr.ref]), decltype(E) is the type of the entity named by E. If there is no such entity, or if E names a set of overloaded functions, the program is ill-formed;
(1.4) otherwise, if E is an xvalue, decltype(E) is T&&, where T is the type of E;
(1.5) otherwise, if E is an lvalue, decltype(E) is T&, where T is the type of E;
(1.6) otherwise, decltype(E) is the type of E.
The operand of the decltype specifier is an unevaluated operand.
[ Example:
```
const int&& foo();
int i;
struct A { double x; };
const A* a = new A();
decltype(foo()) x1 = 17;        // type is const int&&
decltype(i) x2;                 // type is int
decltype(a->x) x3;              // type is double
decltype((a->x)) x4 = x3;       // type is const double&
```
— end example ] [ Note: The rules for determining types involving decltype(auto) are specified in [dcl.spec.auto]. — end note ]

  

### variable decltype

If E is an unparenthesized id-expression (e.g., x, s.field, S::field), then decltype(E) returns the exact type with which the variable, field, or non-type template parameter was declared, including an lvalue or rvalue reference if and only if the variable or field was declared as one. This is a bit like the lstat(2) system call, which is one of the few ways to differentiate between files and symbolic links in the file system.

Let’s call this first calculation variable decltype, since it gives us the type with which a variable (or field) was declared.


### expression decltype

expression decltypes of built-in operators are analogous to the return types of overloaded operators

If E is anything else, including a parenthesized id-expression (e.g., (x), (s.field)), then C++ makes any reference in E’s type completely transparent and undetectable (think stat(2), not lstat). So decltype(E) takes the underlying, non-reference type T of E and decides whether to make it a reference as follows: If E is a prvalue, then decltype(E) is just T; if E is an lvalue, then decltype(E) is T&; and if E is an xvalue, then decltype(E) is T&&.

Let’s call this second calculation expression decltype, or, to coin a clunky abbreviation, exprtype. Later on, I’ll provide an equivalent formulation that does not depend on value categories, in which case we can run the above rule backwards and say an expression E is a prvalue if decltype((E)) is a non-reference type T, an lvalue if decltype((E)) is T&, and an xvalue if decltype((E)) is T&&.

Expression decltype -> decltype((E))
E  expression of type T, and
a) if the value category of expression is xvalue, then decltype yields T&&;
b) if the value category of expression is lvalue, then decltype yields T&;
c) if the value category of expression is prvalue, then decltype yields T.

- Function calls, including overloaded operators, have exprtype identical to the function’s return type

- Unlike other types, string literals, functions, and references to function always have an exprtype of lvalue reference.

  E ->	decltype((E))
  "hello" ->	const char(&)[6]
  getpid ->	int(&)()
  static_cast<int(&)()>(getpid)	-> int(&)()
  std::move(getpid)	-> int(&)()

### decltype(auto)

decltype(auto) - is good for forwarding functions as if we want to deduce the return type for free and at the same time pass EXACT values.

becuase forwarding refrences have the name they must be used with std::forward otherwise theyll be always treated like lvalues!
To put it another way, the compiler can figure out which forward to call based on whether the input is an lvalue or rvalue, but that is unrelated to whether you want the desired output to be an lvalue or rvalue. The compiler knows the type of the input, but you need to explicitly tell it the type of the output, because they are not the same! Like complier could figure it out, BUT what if you want first to use that forwarding reference as lvalue first and then move from it in the end!
like


# built-int operators

freiend is a way to extended the public interface without breaking encapsulation

**if you make friend functions private they would be found through ADL and won't polute class inteface!**

As described by Herb Sutter and Scott Meyers, prefer non-friend non-member functions to member functions, to help increase encapsulation

## operator+()
take paramters by lvalue / rvalue.  **always returns prvalue**!

## >>, << >> operators

better define as friend function inside class scope so that it is nicely composable. 

## mod

in C++ expression a % b returns remainder of division of a by b (if they are positive. For negative numbers sign of result is implementation defined


