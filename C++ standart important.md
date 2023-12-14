
# Undefined behavior

behavior for which this document imposes no requirements
[Note 1: Undefined behavior may be expected when this document omits any explicit definition of behavior or when a program uses an erroneous construct or erroneous data. Permissible undefined behavior ranges from ignoring the situation completely with unpredictable results, to behaving during translation or program execution in a documented manner characteristic of the environment (with or without the issuance of a diagnostic message ([defns.diagnostic])), to terminating a translation or execution (with the issuance of a diagnostic message). Many erroneous program constructs do not engender undefined behavior; they are required to be diagnosed. Evaluation of a constant expression ([expr.const]) never exhibits behavior explicitly specified as undefined in [intro] through [cpp]. — end note]




# [defns.unspecified]unspecified behavior

behavior, for a well-formed program ([defns.well.formed]) construct and correct data, that depends on the implementation
[Note 1: The implementation is not required to document which behavior occurs. The range of possible behaviors is usually delineated by this document. — end note]


-----------------------


------------------------
Cannot re-create const in const!
-------------------------
Creating a new object within the storage that a const complete object with static, thread, or automatic storage duration occupies, or within the storage that such a const object used to occupy before its lifetime ended, results in undefined behavior. [?Example:
struct B {
  B();
  ~B();
};

const B b;

void h() {
  b.~B();
  new (const_cast<B*>(&b)) const/*const in const is not allowed*/ B;     // undefined behavior

---------------------------------


struct V {
  virtual void f();
};

struct A : virtual V { };

struct B : virtual V {
  B(V*, A*);
};

struct D : A, B {
  D() : B((A*)this, this) { }
};

B::B(V* v, A* a) {
  typeid(*this);        // type_�info for B
  typeid(*v);           // well-defined: *v has type V, a base of B yields type_�info for B
  typeid(*a);           //*a IS ***STILL UNDER CONSTRUCTION BECAUSE IT WAS PASSAED FROM D!*** undefined behavior: type A not a base of B
  dynamic_cast<B*>(v);  // well-defined: v of type V*, V base of B results in B*
  dynamic_cast<B*>(a);  // undefined behavior, a has type A*, A not a base of B

------------------
The storage duration is the property of an object that defines the minimum potential lifetime of the storage containing the object. The storage duration is determined by the construct used to create the object and is one of the following:
- static storage duration
- thread storage duration
- automatic storage duration
- dynamic storage duration

When storage for an object with automatic or dynamic storage duration is obtained, the object has an indeterminate value, and if no initialization is performed for the object, that object retains an indeterminate value until that value is replaced ([expr.ass]). [?Note: Objects with static or thread storage duration are zero-initialized, see

If a variable with automatic storage duration has initialization or a destructor with side effects, an implementation shall not destroy it before the end of its block nor eliminate it as an optimization, even if it appears to be unused, except that a class object or its copy/move may be eliminated as specified i

--------------
Every value computation and side effect associated with a full-expression is sequenced before every value computation and side effect associated with the next full-expression to be evaluate

When calling a function (whether or not the function is inline), every value computation and side effect associated with any argument expression, or with the postfix expression designating the called function, is sequenced before execution of every expression or statement in the body of the called function

A discarded-value expression is an expression that is used for its side-effects only. The value calculated from such expression is discarded. Such expressions include the full expression of any expression statement, the left-hand argument of the built-in comma operator, or the argument of a cast-expression that casts to the type void.

----------
Operator overloading shall not modify the rules for the built-in operators, that is, for operators applied to types for which they are defined by this Standard
------------------
