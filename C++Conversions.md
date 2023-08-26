
# primitive types

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

-------------------
Strinct Aliasing Rules
--------------------
If a program attempts to access the stored value of an object through a glvalue of other than one of the following types the behavior is undefined:

(11.1) the dynamic type of the object,
(11.2) a cv-qualified version of the dynamic type of the object,
(11.3) a type similar (as defined in 7.5) to the dynamic type of the object,
(11.4) a type that is the signed or unsigned type corresponding to the dynamic type of the object,
(11.5) a type that is the signed or unsigned type corresponding to a cv-qualified version of the dynamic type of the object,
(11.6) an aggregate or union type that includes one of the aforementioned types among its elements or non-static data members (including, recursively, an element or non-static data member of a subaggregate or contained union),
(11.7) a type that is a (possibly cv-qualified) base class type of the dynamic type of the object,
(11.8) a char, unsigned char, or std::byte type.


---------
Pointer interconvertible
-------------

Two objects a and b are pointer-interconvertible if:
- they are the same object
- one is a union object and the other is a non-static data member of that object ([class.union]), or
- one is a standard-layout classobject and the other is the first non-static data member of that object, or, if the object has no non-static data members, any base class subobject of that object ([class.mem]), or
- there exists an object c such that a and c are pointer-interconvertible, and c and b are pointer-interconvertible.
If two objects are pointer-interconvertible, then they have the same address, and it is possible to obtain a pointer to one from a pointer to the other via a reinterpret_­cast. [ Note: ***************** ->  An array object and its first element are not pointer-interconvertible, even though they have the same address. — end note <- ********* ]

Practically, the standard tries to place as little restriction as possible on implementations. Pointer-interconvertibility is the condition that pointers may be reinterpret_cast and yield the correct result.


If a standard-layout class object has any non-static data members, its address is the same as the address of its first non-static data member if that member is not a bit-field. Its address is also the same as the address of each of its base class subobjects.
[Note 11: There can therefore be unnamed padding within a standard-layout struct object inserted by an implementation, but not at its beginning, as necessary to achieve appropriate alignment. — end note]


Because the comittee wants to make clear that an array is a low level concept an not a first class object: you cannot return an array nor assign to it for example. Pointer-interconvertibility is meant to be a concept between objects of same level: only standard layout classes or unions


 ---------

     ************reinterpret_cast ******************


     IMPORTANT POINTS: 
      - 0,NULL CANNOT be converted to the null pointer of the target type . For that static_cast / implicit convertions must be used!
      - you can convert nullptr to integer type, but nothing can be converted to std::nullptr_t
      - The null pointer value of any pointer type can be converted to any other pointer type, resulting in the null pointer value of that type. Note that the null pointer constant nullptr or any other value of type std::nullptr_t cannot be converted to a pointer with reinterpret_cast: implicit conversion or static_cast should be used for this purpose.
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


 //////


      The result of the expression reinterpret_cast<T>(v) is the result of converting the expression v to type T. If T is an lvalue reference type or an rvalue reference to function type, the result is an lvalue; if T is an rvalue reference to object type, the result is an xvalue; otherwise, the result is a prvalue and the lvalue-to-rvalue, array-to-pointer, and function-to-pointer standard conversions are performed on the expression v. Conversions that can be performed explicitly using reinterpret_cast are listed below. No other conversion can be performed explicitly using reinterpret_cast.




***************static_cast ************************ *

MAJOR:
 - allows explicit constructors to be called! 

- Conversion of any pointer to pointer to void and back to pointer to the original(or more cv - qualified) type preserves its original value.

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

static_cast<T*>(   ... <---------------for this to work------------->........ void* )


----------
C-style casts
---------

When the C-style cast expression is encountered, the compiler attempts to interpret it as the following cast expressions, in this order:
a) const_cast<new-type>(expression);
b) static_cast<new-type>(expression), with extensions: pointer or reference to a derived class is additionally allowed to be cast to pointer or reference to unambiguous base class (and vice versa) even if the base class is inaccessible (that is, this cast ignores the private inheritance specifier). Same applies to casting pointer to member to pointer to member of unambiguous non-virtual base;
c) static_cast (with extensions) followed by const_cast;
d) reinterpret_cast<new-type>(expression);
e) reinterpret_cast followed by const_cast.
 The first choice that satisfies the requirements of the respective cast operator is selected, even if it cannot be compiled (see example). If the cast can be interpreted in more than one way as static_cast followed by a const_cast, it cannot be compiled.
 In addition, C-style cast notation is allowed to cast from, to, and between pointers to incomplete class type. If both expression and new-type are pointers to incomplete class types, it is unspecified whether static_cast or reinterpret_cast gets selected



----
implicit  created objects
----

Some operations are described as implicitly creating objects within a specified region of storage. For each operation that is specified as implicitly creating objects, that operation implicitly creates and starts the lifetime of zero or more objects of implicit-lifetime types ([basic.types.general]) in its specified region of storage if doing so would result in the program having defined behavior. If no such set of objects would give the program defined behavior, the behavior of the program is undefined. If multiple such sets of objects would give the program defined behavior, it is unspecified which such set of objects is created.
[Note 4: Such operations do not start the lifetimes of subobjects of such objects that are not themselves of implicit-lifetime types. — end note]
11
#
Further, after implicitly creating objects within a specified region of storage, some operations are described as producing a pointer to a suitable created object. These operations select one of the implicitly-created objects whose address is the address of the start of the region of storage, and produce a pointer value that points to that object, if that value would result in the program having defined behavior. If no such pointer value would give the program defined behavior, the behavior of the program is undefined. If multiple such pointer values would give the program defined behavior, it is unspecified which such pointer value is produced.

12
#
[Example 3:
#include <cstdlib>
struct X {
  int a, b; };
X *make_x() {
  // The call to std​::​malloc implicitly creates an object of type X
  // and its subobjects a and b, and returns a pointer to that X object
  // (or an object that is pointer-interconvertible ([basic.compound]) with it),
  // in order to give the subsequent class member access operations
  // defined behavior.
  X *p = (X *)std::malloc(sizeof(struct X));
  p->a = 1;
  p->b = 2;
  return p;
}

A class S is an implicit-lifetime class if
- it is an aggregate whose destructor is not user-provided or
- it has at least one trivial eligible constructor and a trivial, non-deleted destructor.


