# similar types

Two types are similar if (ignoring cv):
    - they are the same type;
or -they are both pointers, and the pointed - to types are similar;
or -they ar pointers to member of the same class,
    and the types of the pointed - to members are similar;
or -they are both arrays of the same size or
    at least one of them is array of unknown bound,
    and the array element types are sie bothmilar.


# literal type

A literal type is one whose layout can be determined at compile time. The following are the literal types:
Literals are the tokens of a C++ program that represent constant values embedded in the source code

void
scalar types
references
Arrays of void, scalar types or references
A class that has a constexpr destructor, and one or more constexpr constructors that are not move or copy constructors. Additionally, all its non-static data members and base classes must be literal types and **not volatile**.


Specifies that a type is a literal type. Literal types are the types of constexpr variables and they can be constructed, manipulated, and returned from constexmemset pr functions.



# fundamental types

int is the least type possible for arithmetic operations
IMPORTANT: ***(uint8_t)3 + (uint8_t)3 -> still result type is int ***

# char

arithmetic on char types make them converted into int!
decltype(auto) c_type = (char)'4' >> 2; 
(char)'f' + (char)'4' <--- int

compiler can choose either signed or unsigned for char as ASCII only asks for 127 max!

# std::byte 

```
enum class byte : unsigned char {};
```

equivilent to unsigned char, except airthmetic is not supported!

A numeric value n can be converted to a byte value using std::byte{n}, due to C++17 relaxed enum class initialization rules.

A byte can be converted to a numeric value (such as to produce an integer hash of an object) the usual way with an explicit conversion or alternatively with std::to_integer.

std::byte is a distinct type that implements the concept of byte as specified in the C++ language definition.

Like unsigned char, it can be used to access raw memory occupied by other objects (object representation), but unlike unsigned char, it is not a character type and is not an arithmetic type. std::byte models a mere collection of bits, supporting only bitwise and comparison operations

uint8_t implies an interpretation of the bit pattern. But if the blob you're dealing with is actually structured data, that's the wrong interpretation.

std::byte is a type that specifically says "I have no idea what this bag of bytes represents


# void
void — type with an empty set of values. It is an incomplete type that cannot be completed (consequently, objects of type void are disallowed). There are no arrays of void, nor references to void. However, pointers to void and functions returning type void (procedures in other languages) are permitted.



#  int
int - at least 16 bits by standard

# long
long - at least 32 bits (for 64 Bit linux it got 64 bit , but ON WINDOES 32 bit lenght!)

