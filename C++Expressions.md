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

# Expressionn

An expression is a sequence of operators and operands that specifies a computation.An expression can result in a value and can cause side effects The implementation can regroup operators according to the usual mathematical rules only where the operators really are associative or commutative

# Refenrece

references are not objects

a refenrece is a basically a const pointer that gets dereferenced every time it refered to. a reference yields lvalue expression

refenreces were made to make overloaded operators to work with class types . basically there is a need for something like a pointer but without explicitly taking its address

a reference to const T can bind to an expression x that's not an lvalue of type T.
...if there's a conversion from x's type to T


rvalue refenreces buy only to rvalues or to plain values types
rvalue refrences (type) bind only to rvalues

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
          std::construct_at(&a02, a1)2; // a now contains a ref to i2; not UB even rvalue rebounded, but note that  tmp which a02.i pointed to still has not his lifetime ended
        }

        //temp which points to a02.i get its lifetime ended
    }

## pointer to reference
does not exist!
//int &*v2 = &p1; cannot be complied 

## you can have reference to functions as well

void Foo() {}

int main()
{
    void(& func)() = Foo;
    func(); //::Foo();
}

## reference compatibility

Given types “cv1 T1” and “cv2 T2”, “cv1 T1” is reference-related to “cv2 T2” if T1 is similar ([conv.qual]) to T2, or T1 is a base class of T2. “cv1 T1” is reference-compatible with “cv2 T2” if a prvalue of type “pointer to cv2 T2” can be converted to the type “pointer to cv1 T1” via a standard conversion sequence ([conv]). In all cases where the reference-compatible relationship of two types is used to establish the validity of a reference binding and the standard conversion sequence would be ill-formed, a program that necessitates such a binding is ill-formed.


## referece initialization

A reference to type “cv1 T1” is initialized by an expression of type “cv2 T2” as follows:
  (5.1) If the reference is an lvalue reference and the initializer expression
  (5.1.1) is an lvalue (but is not a bit-field), and “cv1 T1” is reference-compatible with “cv2 T2”, or
  (5.1.2) has a class type (i.e., T2 is a class type), where T1 is not reference-related to T2, and can be converted to an lvalue of type “cv3 T3”, where “cv1 T1” is reference-compatible with “cv3 T3”85 (this conversion is selected by enumerating the applicable conversion functions ([over.match.ref]) and choosing the best one through overload resolution),
  then the reference binds to the initializer expression lvalue in the first case and to the lvalue result of the conversion in the second case (or, in either case, to the appropriate base class subobject of the object).
  [Note 2: The usual lvalue-to-rvalue, array-to-pointer, and function-to-pointer standard conversions are not needed, and therefore are suppressed, when such direct bindings to lvalues are done. — end note]
  [Example 3:
  double d = 2.0;
  double& rd = d;                 // rd refers to d
  const double& rcd = d;          // rcd refers to d

  struct A { };
  struct B : A { operator int&(); } b;
  A& ra = b;                      // ra refers to A subobject in b
  const A& rca = b;               // rca refers to A subobject in b
  int& ir = B();                  // ir refers to the result of B​::​operator int&
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
B&& rrb = x;                        // binds directly to the result of operator B
— end example]
(5.4)
Otherwise:
(5.4.1)
If T1 or T2 is a class type and T1 is not reference-related to T2, user-defined conversions are considered using the rules for copy-initialization of an object of type “cv1 T1” by user-defined conversion ([dcl.init], [over.match.copy], [over.match.conv]); the program is ill-formed if the corresponding non-reference copy-initialization would be ill-formed. The result of the call to the conversion function, as described for the non-reference copy-initialization, is then used to direct-initialize the reference. For this direct-initialization, user-defined conversions are not considered.
(5.4.2)
Otherwise, the initializer expression is implicitly converted to a prvalue of type “T1”. The temporary materialization conversion is applied, considering the type of the prvalue to be “cv1 T1”, and the reference is bound to the result.
If T1 is reference-related to T2:
(5.4.3)
cv1 shall be the same cv-qualification as, or greater cv-qualification than, cv2; and
(5.4.4)
if the reference is an rvalue reference, the initializer expression shall not be an lvalue.
[Note 3: This can be affected by whether the initializer expression is move-eligible ([expr.prim.id.unqual]). — end note]
[Example 6:
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
— end example]
In all cases except the last (i.e., implicitly converting the initializer expression to the referenced type), the reference is said to bind directly to the initializer expression.



# Value Category
      
Each C++ expression(an operator with its operands, a literal, a variable name, etc.) is characterized by two independent properties : a type and a value category.Each expression has some non-reference type, and each expression belongs to exactly one of the three primary value categories : prvalue, xvalue, and lvalue.

- A glvalue is an expression whose evaluation determines the identity of an object or function.
- A prvalue is an expression whose evaluation initializes an object or computes the value of an operand of an operator, as specified by the context in which it appears, or an expression that has type cv void.
- An xvalue is a glvalue that denotes an object whose resources can be reused (usually because it is near the end of its lifetime).
- An lvalue is a glvalue that is not an xvalue.
- An rvalue is a prvalue or an xvalue.



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


The result of a glvalue is the entity denoted by the expression. The result of a prvalue is the value that the expression stores into its context; a prvalue that has type cv void has no result. A prvalue whose result is the value V is sometimes said to have or name the value V. The result object of a prvalue is the object initialized by the prvalue; a non-discarded prvalue that is used to compute the value of an operand of a built-in operator or a prvalue that has type cv void has no result object.



Whenever a glvalue appears as an operand of an operator that requires a prvalue for that operand, the lvalue-to-rvalue ([conv.lval]), array-to-pointer ([conv.array]), or function-to-pointer ([conv.func]) standard conversions are applied to convert the expression to a prvalue


          Example 1:
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
  
  The result of the conversion is determined according to the following rules:
  (3.1) If T is cv std​::​nullptr_t, the result is a null pointer constant ([conv.ptr]).
  [Note 1: Since the conversion does not access the object to which the glvalue refers, there is no side effect even if T is volatile-qualified ([intro.execution]), and the glvalue can refer to an inactive member of a union ([class.union]). — end note]
  (3.2) Otherwise, if T has a class type, the conversion copy-initializes the result object from the glvalue.
  (3.3) Otherwise, if the object to which the glvalue refers contains an invalid pointer value ([basic.stc.dynamic.deallocation]), the behavior is implementation-defined.
  (3.4) Otherwise, the object indicated by the glvalue is read ([defns.access]), and the value contained in the object is the prvalue result.
  4
  
  [Note 2: See also [basic.lval]. — end note]
        45) For historical reasons, this conversion is called the “lvalue-to-rvalue” conversion, even though that name does not accurately reflect the taxonomy of expressions described in 

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

# Pointer ariphmetic 

For addition or subtraction, if the expressions P or Q have type “pointer to cv T”, where T and the array element type are not similar, the behavior is undefined.
[Note 2: In particular, a pointer to a base class cannot be used for pointer arithmetic when the array contains objects of a derived class type. — end note]
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
    assert(firstElement = firstElment2)
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

The materialization of a temporary object is generally delayed as long as possible in order to avoid creating unnecessary temporary objects. [ Note: Temporary objects are materialized

Even when the creation of the temporary object is unevaluated ([expr.context]), all the semantic restrictions shall be respected as if the temporary object had been created and later destroyed ----> EXCEPT <---------- if it used in decltype in that case semantic restrictions dont apply ----> 


Temporary objects are created
- when a prvalue is converted to an xvalue ([conv.rval]),
- when needed by the implementation to pass or return an object of trivially copyable type (see below), and
- when throwing an exception ([except.throw]).


A prvalue of type T can be converted to an xvalue of type T. This conversion initializes a temporary object ([class.temporary]) of type T from the prvalue by evaluating the prvalue with the temporary object as its result object, and produces an xvalue denoting the temporary object. T shall be a complete type.
[Note 1: If T is a class type (or array thereof), it must have an accessible and non-deleted destructor; see [class.dtor]. — end note]
[Example 1:
struct X {
  int n; };
int k = X().n;      // OK, X() prvalue is converted to xvalue


When an implementation introduces a temporary object of a class that has a non-trivial constructor ([class.default.ctor], [class.copy.ctor]), it shall ensure that a constructor is called for the temporary object. Similarly, the destructor shall be called for a temporary with a non-trivial destructor ([class.dtor]). Temporary objects are destroyed as the last step in evaluating the full-expression ([intro.execution]) that (lexically) contains the point where they were created. This is true even if that evaluation ends in throwing an exception. The value computations and side effects of destroying a temporary object are associated only with the full-expression, not with any specific subexpression

If a temporary object has a reference member initialized by another temporary object, lifetime extension applies recursively to such a member's initializer.
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



# Unevaluated expressions

The operands of the operators typeid, sizeof, noexcept, and decltype (since C++11) are expressions that are not evaluated (unless they are polymorphic glvalues and are the operands of typeid), since these operators only query the compile-time properties of their operands. Thus, std::size_t n = sizeof(std::cout << 42); does not perform console output


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


-----
initialization
--------

Important take aways:
 - since C++20 direct aggregate initialization is now possible, though it would allow narrowing conversions and won't extend the liftime of a refrence. Something like this is possible now
 struct A { int i; int j;  };  make_shared<A>(1, 2);

 - basically during value initialization if the object's defualt constructor is not user-provider and not deleted and also trivial than zero initialization during value initialization is possible, otherwise the object would be defualt initialized

 - Aggregate initialization would copy initializate explicitly initialized members and direct initializate members with designated initializer, the rest would be initialized as if they were with {}
 

 - expression e implicitly converts to T if copy initialization works -> T a = e;

+++++++++++++++++++++
default initialization
+++++++++++++++++++++
default -> T t;
default->new T;
+++++++++++++++++++++

Default initialization is performed in three situations:

1) when a variable with automatic, static, or thread-local storage duration is declared with no initializer;
2) when an object with dynamic storage duration is created by a new-expression with no initializer;
3) when a base class or a non-static data member is not mentioned in a constructor initializer list and that constructor is called.

The effects of default initialization are:

- if T is a (possibly cv-qualified) class type, the constructors are considered and subjected to overload resolution against the empty argument list. The constructor selected (which is one of the default constructors) is called to provide the initial value for the new object;
NOTE THAT FOR CLASS TYPE IF NO DEFAULT CONSTRUCTOR CAN BE FOUND THEN IT WOULD
    COMPILE ERROR !
- if T is an array type,
    every element of the array is default - initialized;
- else otherwise, no initialization is performed:
  * the objects with automatic / dynamic storage duration(and their subobjects)contain indeterminate values,
  * static and thread - local objects are zero initialized



----------------------- direct initialization  ----------------------

T object(arg);
T object(arg1, arg2, ...);

 T object { arg };  ----> initialization of an object of non-class type with a single brace-enclosed initializer (note: for class types and other uses of braced-init-list, see list-initialization) 
// ----> T object { arg, arga2,... }; - note that these refer to list initialization
 T(other) 
T(arg1, arg2, ...)

 static_cast<T>(other)(4) new T(args, ...)(5) Class::Class()
  : member(args, ...){...}(6)[arg](){...}

      () style direct initialization is more permissive as it allows narrowing
      conversations and using explicit constructors

while direct - initialization considers all constructors and all user -
    defined conversion functions

     The effets of direct initialization are : 
    - If T is an array type the array is initialized as in aggregate initialization, except that narrowing conversions are allowed and any elements without an
    initializer are value
     initialized.A a[2]{A(1)}; // OK: initializes a[0] with A(1) and a[1] with A()
 -

(----> PRETTY MUCH ALLOWS LIMITED AGGREGTE like INITIAZLIATOIN SINCE C++20)
if T is class Type otherwise, if the destination type is a (possibly cv-qualified) aggregate class, it is initialized as described in aggregate initialization except that narrowing conversions are permitted, designated initializers are not allowed, a temporary bound to a reference does not have its lifetime extended, there is no brace elision, and any elements without an initializer are value-initialized.
struct B
{
    int a;
    int&& r;
};
 
int f();
int n = 10;
 
B b1{1, f()};            // OK, lifetime is extended
B b2(1, f());            // well-formed, but dangling reference
B b3{1.0, 1};            // error: narrowing conversion
B b4(1.0, 1);            // well-formed, but dangling reference
B b5(1.0, std::move(n)); // OK

----------------- value initialization ------------------------

   T()
   new T()
   Class::Class(...)
      : member(){...}
   T object{};
(since C++ 11) - BE CAREFULL FOR AGGREGATES IT WOULD CALL AGGREGATE
        INITIALIZATION which has these rules so explicit defult constructors may
        not work !explicit constructors
    and conversion operators wont work in aggregate initialization

    ""
    ""
    "   Otherwise, if the element is not a reference, the element is "
    "copy-initialized from an empty initializer list.  "
    ""
    ""

    T{}
    new T{}(6)(since C++ 11)Class::Class(...)
    : member{} {...}(7)(since C++ 11)

if the empty pair of braces {} is used and T is an aggregate type, aggregate-initialization is performed instead of value-initialization.
Turns into list initialization: If T is a class type that has no default constructor but has a constructor taking std::initializer_list, list-initialization is performed.

Zero- and value-initialization also initialize pointers to their null values

(since C++11)
The effects of value initialization are:

1) if T is a class type with no default constructor  or with user-provided or deleted (since C++11) default constructor, the object is default-initialized;
2) if T is a class type with a default constructor (it can be non-trivial) that is neither user-provided nor deleted (since C++11) (that is, it may be a class with an implicitly-defined or defaulted default constructor), the object is zero-initialized and the semantic constraints for default-initialization are checked, and if T has a non-trivial default constructor, the object is default-initialized;
3) if T is an array type, each element of the array is value-initialized;
4) otherwise, the object is zero-initialized.


The standard specifies that zero-initialization is not performed when the class has a user-provided or deleted default constructor, which implies that whether said default constructor is selected by overload resolution is not considered. All known compilers performs additional zero-initialization if a non-deleted defaulted default constructor is selected


++++++++++++++++++
aggregate initialization
++++++++++++++++

T object = { arg1, arg2, ... };
(1) T object{arg1, arg2, ...};
(2)(since C++ 11) T object = {.des1 = arg1, .des2{arg2}...};
(3)(since C++ 20) T object{.des1 = arg1, .des2{arg2}...};	(4)	(since C++20)
(6) T array[N] = {other - sequence}; IMPORTANT -> array just just aggregate class!

all value computations and side effects associated with a given element are sequenced before those of any element that follows it in order of declaration

the way c++ works is that if object is aggregate , aggregate initialization is always been tried on for ither value initialization / list initialization


element is copy-initialized from the corresponding initializer clause of the initializer list:
- If the initializer clause is an expression, implicit conversions are allowed as per copy-initialization, except that narrowing conversions are prohibited (since C++11).
- If the initializer clause is a nested braced-init-list (which is not an expression), list-initialize the corresponding element from that clause, which will (since C++11) recursively apply the rule if the corresponding element is a subaggregate.
- if no value for a given memeber is given then it is copy-initializate with {}

If the aggregate is a union and the initializer list is empty, then If any variant member has a default member initializer, that member is initialized from its default member initializer.(since C++ 11)Otherwise, the first member of the union(if any) is copy  initialized from an empty initializer list.



For a non-union aggregate, each element that is not an explicitly initialized element is initialized as follows:

If the element has a default member initializer, the element is initialized from that initializer.
(since C++11)
Otherwise, if the element is not a reference, the element is copy-initialized from an empty initializer list.
Otherwise, the program is ill-formed.
struct S
{
    int a;
    const char* b;
    int c;
    int d = b[a];
};
 
// initializes ss.a with 1,
//             ss.b with "asdf",
//             ss.c with the value of an expression of the form int{} (that is, 0), <---------------------
//         and ss.d with the value of ss.b[ss.a] (that is, 's')
S ss = {1, "asdf"};


********************* copy initialization *****************************************

Syntax 
(1) T object = other;
(2) f(other)
(3) return other;
(4) throw object;
(5)catch (T object)

---
NARROWING CONVERSIONS ARE allowed
-------

The effects of copy initialization are:

- First, if T is a class type and the initializer is a prvalue expression whose cv-unqualified type is the same class as T, the initializer expression itself, rather than a temporary materialized from it, is used to initialize the destination object: see copy elision
(since C++17)

- If T is a class type and the cv-unqualified version of the type of other is T or a class derived from T, the non-explicit constructors of T are examined and the best match is selected by overload resolution. The constructor is then called to initialize the object.

- If T is a class type, and the cv-unqualified version of the type of other is not T or derived from T, or if T is non-class type, but the type of other is a class type, user-defined conversion sequences that can convert from the type of other to T (or to a type derived from T if T is a class type and a conversion function is available) are examined and the best one is selected through overload resolution. The result of the conversion, which is prvalue expression (since C++17) of the cv-unqualified version of T if a converting constructor was used, is then used to direct-initialize the object. The last step is usually optimized out and the result of the conversion is constructed directly in the memory allocated for the target object, but the appropriate constructor (move or copy) is required to be accessible even though its not used.

- Otherwise (if neither T nor the type of other are class types), standard conversions are used, if necessary, to convert the value of other to the cv-unqualified version of T.

In addition, the implicit conversion in copy-initialization must produce T directly from the initializer, while, e.g. direct-initialization expects an implicit conversion from the initializer to an argument of T's constructor.

struct S { S(std::string) {} }; // implicitly convertible from std::string
S s("abc");   // OK: conversion from const char[4] to std::string
S s = "abc";  // Error: no conversion from const char[4] to S
S s = "abc"s; // OK: conversion from std::string to S
If other is an rvalue expression, a move constructor will be selected by overload resolution and called during copy-initialization. This is still considered copy-initialization; there is no special term (e.g., move-initialization) for this case.

Implicit conversion is defined in terms of copy-initialization: if an object of type T can be copy-initialized with expression E, then E is implicitly convertible to T.

If other is an rvalue expression, a move constructor will be selected by overload resolution and called during copy-initialization. This is still considered copy-initialization; there is no special term (e.g., move-initialization) for this case.


----------------
list initialization
-------------

Direct-list-initialization

T object { arg1, arg2, ... };
(1) T{arg1, arg2, ...}
(2) new T{arg1, arg2, ...}
(3)Class {
  T member{arg1, arg2, ...};
};
(4) Class::Class() : member{arg1, arg2, ...} {
  ...
  (5) Copy - list - initialization

          T object = {arg1, arg2, ...};
  (6) function({arg1, arg2, ...})
  (7) return {arg1, arg2, ...};
  (8) object[{arg1, arg2, ...}]
  (9) object = {arg1, arg2, ...}
  (10)U({arg1, arg2, ...})
  (11) Class {
    T member = {arg1, arg2, ...};
  };	



The effects of list-initialization of an object of type T are:

- If T is an aggregate class and the braced-init-list has a single element of the same or derived type (possibly cv-qualified), the object is initialized from that element (by copy-initialization for copy-list-initialization, or by direct-initialization for direct-list-initialization).

- Otherwise, if T is an aggregate type, aggregate initialization is performed.

- Otherise, if the braced-init-list is empty and T is a class type with a default constructor, value-initialization is performedw.

- Otherwise, the constructors of T are considered, in two phases:
If the previous stage does not produce a match, all constructors of T participate in overload resolution against the set of arguments that consists of the elements of the braced-init-list, with the restriction that only non-narrowing conversions are allowed. If this stage produces an explicit constructor as the best match for a copy-list-initialization, compilation fails (note, in simple copy-initialization, explicit constructors are not considered at all).

- Otherwise (if T is not a class type), if the braced-init-list has only one element and either T is not a reference type or is a reference type whose referenced type is same as or is a base class of the type of the element, T is direct-initialized (in direct-list-initialization) or copy-initialized (in copy-list-initialization), except that narrowing conversions are not allowed.

a prvalue is generated. The prvalue initializes its result object by copy-list-initialization. The prvalue is then used to direct-initialize the reference (this fails if the reference is a non-const lvalue reference). The type of the temporary is the type referenced by T, unless T is “reference to array of unknown bound of U”, in which case the type of the temporary is the type of x in the declaration U x[] H, where H is the initializer list (since C++20).
(since C++17)

-