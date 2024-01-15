# similar types

Two types are similar if (ignoring cv):
    - they are the same type;
or -they are both pointers, and the pointed - to types are similar;
or -they are both pointers to member of the same class,
    and the types of the pointed - to members are similar;
or -they are both arrays of the same size or
    at least one of them is array of unknown bound,
    and the array element types are similar.


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

# integral promotions

C++ promotions are "value-preserving." That is, the value after the promotion is guaranteed to be the same as the value before the promotion. In value-preserving promotions, objects of shorter integral types (such as bit fields or objects of type char) are promoted to type int if int can represent the full range of the original type. If int cannot represent the full range of values, then the object is promoted to type unsigned int. Although this strategy is the same as that used by ANSI C, value-preserving conversions do not preserve the "signedness" of the object.

Value-preserving promotions and promotions that preserve signedness normally produce the same results. However, they can produce different results if the promoted object is one of the following:

 - An operand of /, %, /=, %=, <, <=, >, or >=

    These operators rely on sign for determining the result. Therefore, value-preserving and sign-preserving promotions produce different results when applied to these operands.

- The left operand of >> or >>=

    These operators treat signed and unsigned quantities differently when performing a shift operation. For signed quantities, shifting a quantity right causes the sign bit to be propagated into the vacated bit positions. For unsigned quantities, the vacated bit positions are zero-filled.

## signed to unsgined 

Objects of signed integral types can be converted to corresponding unsigned types. When these conversions occur, the actual bit pattern does not change; however, the interpretation of the data changes


## floating types

An object of a floating type can be safely converted to a more precise floating type — that is, the conversion causes no loss of significance. For example, conversions from float to double or from double to long double are safe, and the value is unchanged.

An object of a floating type can also be converted to a less precise type, if it is in a range representable by that type. (See Floating Limits for the ranges of floating types.) If the original value cannot be represented precisely, it can be converted to either the next higher or the next lower representable value. If no such value exists, the result is undefined

## integer to float

Certain expressions can cause objects of floating type to be converted to integral types, or vice versa. When an object of integral type is converted to a floating type and the original value cannot be represented exactly, the result is either the next higher or the next lower representable value.


## float to integer


When an object of floating type is converted to an integral type, the fractional part is **truncated**. No rounding takes place in the conversion process. Truncation means that a number like 1.3 is converted to 1, and –1.3 is converted to –1

## ariphmetic converion rules

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