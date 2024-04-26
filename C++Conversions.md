# implicit conversions

Implicit conversions are performed whenever an expression of some type T1 is used in context that does not accept that type, but accepts some other type T2; in particular:

when the expression is used as the argument when calling a function that is declared with T2 as parameter;
when the expression is used as an operand with an operator that expects T2;
when initializing a new object of type T2, including return statement in a function returning T2;
when the expression is used in a switch statement (T2 is integral type);
when the expression is used in an if statement or a loop (T2 is bool).
The program is well-formed (compiles) only if there exists one unambiguous implicit conversion sequence from T1 to T2.

If there are multiple overloads of the function or operator being called, after the implicit conversion sequence is built from T1 to each available T2, overload resolution rules decide which overload is compiled.

Note: in [ink](#ariphmetic-converions), the destination type for the implicit conversions on the operands to binary operators is determined by a separate set of rules: usual arithmetic conversions.

## Order of the conversions

Implicit conversion sequence consists of the following, in this order:

1) zero or one standard conversion sequence;
2) zero or one user-defined conversion;
3) zero or one standard conversion sequence (only if a user-defined conversion is used).
When considering the argument to a constructor or to a user-defined conversion function, only one standard conversion sequence is allowed (otherwise user-defined conversions could be effectively chained). When converting from one non-class type to another non-class type, only a standard conversion sequence is allowed.

A standard conversion sequence consists of the following, in this order:

1) zero or one conversion from the following set:
lvalue-to-rvalue conversion,
array-to-pointer conversion, and
function-to-pointer conversion;
2) zero or one numeric promotion or numeric conversion;
3) zero or one function pointer conversion;
(since C++17)
4) zero or one qualification conversion.
A user-defined conversion consists of zero or one non-explicit single-argument converting constructor or non-explicit conversion function call.

An expression e is said to be implicitly convertible to T2 if and only if T2 can be copy-initialized from e, that is the declaration T2 t = e; is well-formed (can be compiled), for some invented temporary t. Note that this is different from direct initialization (T2 t(e)), where explicit constructors and conversion functions would additionally be consider


## boolean conversions

Boolean conversions
A prvalue of integral, floating-point, unscoped enumeration, pointer, and pointer-to-member types can be converted to a prvalue of type bool.

The value zero (for integral, floating-point, and unscoped enumeration) and the null pointer and the null pointer-to-member values become false. All other values become true.

In the context of a direct-initialization, a bool object may be initialized from a prvalue of type std::nullptr_t, including nullptr. The resulting value is 
false. However, this is not considered to be an implicit conversion.

A prvalue of type bool can be converted to a prvalue of type int, with false becoming ​0​ and true becoming 1


In the following contexts, the type bool is expected and the implicit conversion is performed if the declaration bool t(e); is well-formed (that is, an explicit conversion function such as explicit T::operator bool() const; is considered). Such expression e is said to be contextually converted to bool.

- the controlling expression of if, while, for;
- the operands of the built-in logical operators !, && and ||;
- the first operand of the conditional operator ?:;
- the predicate in a static_assert declaration;
- the expression in a noexcept specifier;
- the expression in an explicit specifier; (since C++20)
- void*

One early solution for this can be seen in std::basic_ios, which initially defines operator void*, so that the code such as if (std::cin) {...} compiles because void* is convertible to bool, but int n = std::cout; does not compile because void* is not convertible to int. This still allows nonsense code such as delete std::cout; to compile.

Many pre-C++11 third party libraries were designed with a more elaborate solution, known as the Safe Bool idiom. std::basic_ios also allowed this idiom via LWG issue 468, and operator void* was replaced (see notes).


## integral promotions

C++ promotions are "value-preserving." That is, the value after the promotion is guaranteed to be the same as the value before the promotion. In value-preserving promotions, objects of shorter integral types (such as bit fields or objects of type char) are promoted to type int if int can represent the full range of the original type. If int cannot represent the full range of values, then the object is promoted to type unsigned int. Although this strategy is the same as that used by ANSI C, value-preserving conversions do not preserve the "signedness" of the object.

Value-preserving promotions and promotions that preserve signedness normally produce the same results. However, they can produce different results if the promoted object is one of the following:

 - An operand of /, %, /=, %=, <, <=, >, or >=

    These operators rely on sign for determining the result. Therefore, value-preserving and sign-preserving promotions produce different results when applied to these operands.

- The left operand of >> or >>=

    These operators treat signed and unsigned quantities differently when performing a shift operation. For signed quantities, shifting a quantity right causes the sign bit to be propagated into the vacated bit positions. For unsigned quantities, the vacated bit positions are zero-filled.

### signed to unsgined 

Objects of signed integral types can be converted to corresponding unsigned types. When these conversions occur, the actual bit pattern does not change; however, the interpretation of the data changes


### floating types

An object of a floating type can be safely converted to a more precise floating type — that is, the conversion causes no loss of significance. For example, conversions from float to double or from double to long double are safe, and the value is unchanged.

An object of a floating type can also be converted to a less precise type, if it is in a range representable by that type. (See Floating Limits for the ranges of floating types.) If the original value cannot be represented precisely, it can be converted to either the next higher or the next lower representable value. If no such value exists, the result is undefined

### integer to float

Certain expressions can cause objects of floating type to be converted to integral types, or vice versa. When an object of integral type is converted to a floating type and the original value cannot be represented exactly, the result is either the next higher or the next lower representable value.


### float to integer


When an object of floating type is converted to an integral type, the fractional part is **truncated**. No rounding takes place in the conversion process. Truncation means that a number like 1.3 is converted to 1, and –1.3 is converted to –1

### ariphmetic converions

int is the least type possible for arithmetic operations
IMPORTANT: ***(uint8_t)3 + (uint8_t)3 -> still result type is int ***


Conditions Met	                                        | Conversion
Either operand is of type long double.	                | Other operand is converted to type long double.
Preceding condition not met and either operand is of    | Other operand is converted to type double.
type double.	                                         
Preceding conditions not met and either                 | Other operand is converted to type float.     
operand is of type float

Preceding conditions not met                            | Integral promotions are performed on the operands as follows:
(none of the operands are of floating types).	

- If either operand is of type unsigned long, the other operand is converted to type unsigned long.
- If preceding condition not met, and if either operand is of type long and the other of type unsigned int, both operands are converted to type unsigned long.
- If the preceding two conditions are not met, and if either operand is of type long, the other operand is converted to type long.
- If the preceding three conditions are not met, and if either operand is of type unsigned int, the other operand is converted to type unsigned int.
- If none of the preceding conditions are met, both operands are converted to type int.
 






# Numeric conversions

Unlike the promotions, numeric conversions may change the values, with potential loss of precision

# value transformations 

Value transformations are conversions that change the value category of an expression. They take place whenever an expression appears as an operand of an operator that expects an expression of a different value category:

Whenever a glvalue appears as an operand of an operator that requires a prvalue for that operand, the lvalue-to-rvalue, array-to-pointer, or function-to-pointer standard conversions are applied to convert the expression to a prvalue.
Whenever a prvalue appears as an operand of an operator that expects a glvalue for that operand, the temporary materialization conversion is applied to convert the expression to an xvalue.

# primitive types

# nullptr

Additive operators [expr.add]

7 If the value 0 is added to or subtracted from a pointer value, the result compares equal to the original pointer value. If two pointers point to the same object or both point one past the end of the same array or both are null, and the two pointers are subtracted, the result compares equal to the value 0 converted to the type std::ptrdiff_t

nullptr - nullptr == 0 !

# void*

A prvalue of type “pointer to cv T”, where T is an object type, can be converted to a prvalue of type “pointer to cv void”. The pointer value ([basic.compound]) is unchanged by this conversion.

## integral promotion 

For each of the standard signed integer types, there exists a corresponding (but different) standard unsigned integer type: “unsigned char”, “unsigned short int”, “unsigned int”, “unsigned long int”, and “unsigned long long int”.

ype char is a distinct type that has an implementation-defined choice of “signed char” or “unsigned char” as its underlying type. The three types char, signed char, and unsigned char are collectively called ordinary character types. The ordinary character types and char8_t are collectively called narrow character types. For narrow character types, each possible bit pattern of the object representation represents a distinct value.

Every integer type has an integer conversion rank defined as follows:
(1.1) No two signed integer types other than char and signed char (if char is signed) have the same rank, even if they have the same representation.
(1.2) The rank of a signed integer type is greater than the rank of any signed integer type with a smaller width.
(1.3) The rank of long long int is greater than the rank of long int, which is greater than the rank of int, which is greater than the rank of short int, which is greater than the rank of signed char.
(1.4) The rank of any unsigned integer type equals the rank of the corresponding signed integer type.
(1.5) The rank of any standard integer type is greater than the rank of any extended integer type with the same width.
(1.6) The rank of char equals the rank of signed char and unsigned char.
(1.7) The rank of bool is less than the rank of all standard integer types.
(1.8) The ranks of char8_t, char16_t, char32_t, and wchar_t equal the ranks of their underlying types ([basic.fundamental]).
(1.9) The rank of any extended signed integer type relative to another extended signed integer type with the same width is implementation-defined, but still subject to the other rules for determining the integer conversion rank.
(1.10) For all integer types T1, T2, and T3, if T1 has greater rank than T2 and T2 has greater rank than T3, then T1 has greater rank than T3.

A prvalue that is not a converted bit-field and has an integer type other than bool, char8_t, char16_t, char32_t, or wchar_t whose integer conversion rank ([conv.rank]) is less than the rank of int can be converted to a prvalue of type int if int can represent all the values of the source type; otherwise, the source prvalue can be converted to a prvalue of type unsigned int.




# Pointer interconvertible

Two objects a and b are pointer-interconvertible if:
- they are the same object
- one is a union object and the other is a non-static data member of that object ([class.union]), or
- one is a standard-layout classobject and the other is the first non-static data member of that object, or, if the object has no non-static data members, any base class subobject of that object ([class.mem]), or
- there exists an object c such that a and c are pointer-interconvertible, and c and b are pointer-interconvertible.
If two objects are pointer-interconvertible, then they have the same address, and it is possible to obtain a pointer to one from a pointer to the other via a reinterpret_­cast. [ Note: ***************** ->  An array object and its first element are not pointer-interconvertible, even though they have the same address. — end note <- ********* ]

Practically, the standard tries to place as little restriction as possible on implementations. Pointer-interconvertibility is the condition that pointers may be reinterpret_cast and yield the correct result.


If a standard-layout class object has any non-static data members, its address is the same as the address of its first non-static data member if that member is not a bit-field. Its address is also the same as the address of each of its base class subobjects.
[Note 11: There can therefore be unnamed padding within a standard-layout struct object inserted by an implementation, but not at its beginning, as necessary to achieve appropriate alignment. — end note]


# reinterpret_cast


     IMPORTANT POINTS: 
      - 0,NULL CANNOT be converted to the null pointer of the target type . For that static_cast / implicit convertions must be used!
      - you can convert nullptr to integer type, but nothing can be converted to std::nullptr_t
      - The null pointer constant NULL or integer zero is not guaranteed to yield the null pointer value of the target type; static_cast or implicit conversion should be used for this purpose.
      - pointer-interconvertible rules avoid Strict Aliasing rules been applied,so that this can be done safely
      struct A{
  int i;
} A a;
int *p = reinterpret_cast<int *>(&a);
*p = 100; <--- access is done through DynamicType int* in fact!, because of pointer-interconvertible rules

  If two objects are pointer  interconvertible, then they have the same address, and it is possible to obtain a pointer to one from a pointer to the other via a reinterpret_­cast([expr.reinterpret.cast]) .Practically, the standard tries to place as little restriction as possible on implementations.Pointer - interconvertibility is the condition that pointers may be reinterpret_cast and yield the correct result .

////INFORMAL////
reinterpret_cast may change pointer vaues - for pointer-interconvertabgle types and also on some implementations
say for cray

char * and int * have different representations on Cray T90, so:

int *i = <some int pointer value>;
char *c = reinterpret_cast<char *>(i);
Here, i and c will have differing representations on Cray T90 (and doing this conversion definitely uses CPU instructions)



The result of the expression reinterpret_cast<T>(v) is the result of converting the expression v to type T. If T is an lvalue reference type or an rvalue reference to function type, the result is an lvalue; if T is an rvalue reference to object type, the result is an xvalue; otherwise, the result is a prvalue and the lvalue-to-rvalue, array-to-pointer, and function-to-pointer standard conversions are performed on the expression v. Conversions that can be performed explicitly using reinterpret_cast are listed below. No other conversion can be performed explicitly using reinterpret_cast.




# static_cast 

MAJOR:
 - **allows explicit constructors to be called!**

struct D : B {};

struct M {
  explicit M(B &d) { std::cout << "explicit constructor M()\n"; }
};


    M m1 = static_cast<M>(d); // explicit constructor is fine for static cast

- Conversion of any pointer to pointer to void and back to pointer to the original(or more cv - qualified) type preserves its original value. The oposite may not yield the same result, for example if you take interger value, convert it to a pointer and than from pointer back to the integer value

  this is totally fine Test t;
void *p = t;
  Test* ptr = static_cast<Test*>(p)

downcast -> from Base to Derived!

Only the following conversions can be done with static_cast, except when such conversions would cast away constness or volatility.
1) If new-type is a reference to some class D and expression is an lvalue of its non-virtual base B, or new-type is a pointer to some complete class D and expression is a prvalue pointer to its non-virtual base B, static_cast performs a downcast. (This downcast is ill-formed if B is ambiguous, inaccessible, or virtual base (or a base of a virtual base) of D.)
Such a downcast makes no runtime checks to ensure that the objects runtime type is actually D, and may only be used safely if this precondition is guaranteed by other means, such as when implementing static polymorphism. Safe downcast may be done with dynamic_cast. If the object expression refers or points to is actually a base class subobject of an object of type D, the result refers to the enclosing object of type D. Otherwise, the behavior is undefined:

struct B {};
  struct D : B {
    B b;
  };

  D d;
  B &br1 = d;
  B &br2 = d.b;

  static_cast<D &>(br1); // OK: lvalue denoting the original d object
  static_cast<D &>(br2); // UB: the b subobject is not a base class subobject

2) If new-type is an rvalue reference type, static_cast converts the value of any lvalue (since C++17) expression to xvalue referring to the same object as the expression, or to its base sub-object (depending on new-type). If the target type is an inaccessible or ambiguous base of the type of the expression, the program is ill-formed. If the expression is a bit-field lvalue, it is first converted to prvalue of the underlying type. This type of static_cast is used to implement move semantics in std::move.
   D &&d1 = static_cast<D &&>(b3);


3) If there is an implicit conversion sequence from expression to new-type, or if overload resolution for a direct initialization of an object or reference of type new-type from expression would find at least one viable function, then static_cast<new-type>(expression) returns the imaginary variable Temp initialized as if by new-type Temp(expression);, which may involve implicit conversions, a call to the constructor of new-type or a call to a user-defined conversion operator. For non-reference new-type, the result object of the static_cast prvalue expression is what's direct-initialized.

struct D : B {};

struct M {
  explicit M(B &d) { std::cout << "explicit constructor M()\n"; }
};


    {
      // creates temporary!
      M m1 = static_cast<M>(d); // explicit constructor is fine for static cast
      M *m4 = reinterpret_cast<M *>(&d); //  you can convert it but cannot use
                                         //  it due to strict aliasing rules!
      M m2 = static_cast<M>(d1); // explicit constructor is fine for static cast
      M m3 = static_cast<M>(b3); // explicit constructor is fine for static cast
      // M m2 = d; ERROR explicit constructo
    }


4) If new-type is the type void (possibly cv-qualified), static_cast discards the value of expression after evaluating it.

  - you can do the same for interconvertible-pointers  --->
  A prvalue of type pointer to void (possibly cv-qualified) can be converted to pointer to any object type. If the original pointer value represents an address of a byte in memory that does not satisfy the alignment requirement of the target type, then the resulting pointer value is unspecified. Otherwise, if the original pointer value points to an object a, and there is an object b of the target type (ignoring cv-qualification) that is pointer-interconvertible (as defined below) with a, the result is a pointer to b. Otherwise the pointer value is unchanged. Conversion of any pointer to pointer to void and back to pointer to the original (or more cv-qualified) type preserves its original value.


5)  A prvalue of type “pointer to cv1 void” can be converted to a prvalue of type “pointer to cv2 T”, where T is an object type and cv2 is the same cv-qualification as, or greater cv-qualification than, cv1. If the original pointer value represents the address A of a byte in memory and A does not satisfy the alignment requirement of T, then the resulting pointer value is unspecified. Otherwise, if the original pointer value points to an object a, and there is an object b of type similar to T that is pointer-interconvertible with a, the result is a pointer to b. Otherwise, the pointer value is unchanged by the conversion.
[Example 3:
T* p1 = new T;
const T* p2 = static_cast<const T*>(static_cast<void*>(p1));
bool b = p1 == p2;  // b will have the value true.
— end example]

Another example for this is 

alingas(A) std::byte storage[sizeof(A)];
new (storage) T;

static_cast<T*>(   ... <---------------for this to work std::launder is needed ------------->........ void* )


# C-style casts

When the C-style cast expression is encountered, the compiler attempts to interpret it as the following cast expressions, in this order:
a) const_cast<new-type>(expression);
b) static_cast<new-type>(expression), with extensions: pointer or reference to a derived class is additionally allowed to be cast to pointer or reference to unambiguous base class (and vice versa) even if the base class is inaccessible (that is, this cast ignores the private inheritance specifier). Same applies to casting pointer to member to pointer to member of unambiguous non-virtual base;
c) static_cast (with extensions) followed by const_cast;
d) reinterpret_cast<new-type>(expression);
e) reinterpret_cast followed by const_cast.
 The first choice that satisfies the requirements of the respective cast operator is selected, even if it cannot be compiled (see example). If the cast can be interpreted in more than one way as static_cast followed by a const_cast, it cannot be compiled.
 In addition, C-style cast notation is allowed to cast from, to, and between pointers to incomplete class type. If both expression and new-type are pointers to incomplete class types, it is unspecified whether static_cast or reinterpret_cast gets selected

 they are really desirable for these cases for example

```
const volatile T* __p = ...
 ::new ((void *)__p) _Up(std::forward<_Args>(__args)...); //this works!, but static_cast / reinterpret_cast won't be able to work!

 ```

