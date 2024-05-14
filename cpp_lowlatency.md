# PCI latency

fastest latency is around 20-40ns

# things which may DEGRADADE performance at micro level

- The data interdependencies between instructions stall the pipeline, decreasing performance. The worst cases occur when the same register is read and modified by several consecutive instructions (think about rflags!)

- pow can be slow massively

- std::function may allocate memory

- ? : can disable RVO
  multiple return statements can disable RVO
  returning passed argument can disable RVO



# things which may IMPROVE performance at micro level

- auto may improve perfomance because there are not implicit conversions involved

- std::to_string relies on the current locale for formatting purposes, and therefore concurrent calls to std::to_string from multiple threads may result in partial serialization of calls. C++17 provides std::to_chars as a higher-performance locale-independent alternative.

- always_inline and noinline atrributes better then inline

- make switch faster
```
 swtich (num)
  case 1:
  case 2
  case N+1:
```
  if increasing by step 1 then compiler can easily optimizy it by just applying offset!

- In order to effectively write your code to take advantage of these rules: when writing if-else or switch statements, check the most common cases first and work progressively down to the least common

-  fast way to check if N is power of 2
```
   static_assert(
      (kMaxCpus & (kMaxCpus - 1)) == 0,
      "kMaxCpus should be a power of two so modulo is fast")
```


# performance pitfalls

## stdring move consturcot

std::string's move constructor is actually pretty expensive: it has several branches to handle SSO, and copies 24 or 32 bytes per stack frame depending on which stdlib implementation you're using

# code tricks

## prefetch

there is hardware prefetch, which can be used wisefly
for example, imagine you got a batch of queries for a sparse hash map to do. instead of doing queries one by one whic would take a lot of time due
to memory acceses, one may run pre-fetch and quickly do another pre-fetch for a given batch, after which we run a batch to access data.

### Nano threads
batching prefetch and access

## Alternating prefetch/access

instead we can run prefetch for X+n batch, then we start access data for X1 and so on  by alternating prefetch and access

## fast modulo

modulo could be done like x & (b - 1)
but carefull abotu sequence points! readPos = ++readPos & (N - 1); - BUG!
shall be readPos++; readPos = readPos & (N - 1)

## Branch elimination techniques!

(1) r = (A >= B) ? CONST 1 : CONST2

xor ebx, ebx ; Clear ebx (X in the C code)
cmp A, B
setge bl ; When ebx = 0 or 1
; OR the complement condition
sub ebx, 1 ; ebx=11...11 or 00...00
and ebx, CONST3; CONST3 = CONST1-CONST2
add ebx, CONST2; ebx=CONST1 or CONST2

## branches

https://johnnysswlab.com/how-branches-influence-the-performance-of-your-code-and-what-can-you-do-about-it/

### branchless ariphmetic 

```
// With branch
if (a > b) {
    x += y;
}
// Branchless
x += -(a > b) & y; //here trick is that a > b will return -1 which would be a number with all 1s bits set!
```

# compiler tricks 

## - fast-math -msse4.1 

improves math ops performance by compromising precision. speeds up pow

## how inlining workings

- If a called function is defined in another .C or .CPP file, it cannot be automatically inlined, unless link time optimizations are enabled.
- In C++, if class method is defined in the class declaration, it will be inlined unless too large.
- Functions marked as static will probably be automatically inlined.
- If a C++ method is marked as virtual, it cannot be automatically inlined (there are exceptions to this).
- If a function is called using a function pointer, it cannot be inlined. On the other hand, if a function is called as a lambda expression, it can probably be inlined.
- If a function is to long, the compiler will probably not inline it. This decision is motivated by performance, long function is not worth inlining because the function itself takes long time and call overhead is small.

__attribute__((always_inline)) – forces the compiler to always inline a function. In case it is not possible to inline it, it will generate a compilation warning

__attribute__((flatten)) - if this keyword is in the declaration of a function, all calls made to other functions from that function will be replaced with inline versions (if possible)

__attribute__((cold)) - to let the compiler know they execute rarely so the compiler can move them away from the hot path

__attribute__((noinline))

Turn on link-time optimizations to allow code from other modules to be inlined.

## pragma pack

"#pragma pack" instructs the compiler to pack structure members with particular alignment. Most compilers, when you declare a struct, will insert padding between members to ensure that they are aligned to appropriate addresses in memory (usually a multiple of the type's size). This avoids the performance penalty (or outright error) on some architectures associated with accessing variables that are not aligned properly. For example, given 4-byte integers and the following struct:

struct Test
{
   char AA;
   int BB;
   char CC;
};
The compiler could choose to lay the struct out in memory like this:

|   1   |   2   |   3   |   4   |  

| AA(1) | pad.................. |
| BB(1) | BB(2) | BB(3) | BB(4) | 
| CC(1) | pad.................. |
and sizeof(Test) would be 4 × 3 = 12, even though it only contains 6 bytes of data. The most common use case for the #pragma (to my knowledge) is when working with hardware devices where you need to ensure that the compiler does not insert padding into the data and each member follows the previous one. With #pragma pack(1), the struct above would be laid out like this:

|   1   |

| AA(1) |
| BB(1) |
| BB(2) |
| BB(3) |
| BB(4) |
| CC(1) |


## cpu features

### CPU dispatching

https://gcc.gnu.org/wiki/FunctionMultiVersioning

gcc and clang allow you to override functions bades on CPU features!

```
__attribute__((__target__ ("avx512f")))
void add_vectors(float* res, float* in1, float* in2) {
    ...
}
__attribute__((__target__ ("default")))
void add_vectors(float* res, float* in1, float* in2) {
    ...
}
```


dispatch functions can even be manually created
```
__attribute__((ifunc ("my_function_dispatch")))
void my_function(float* a, float* b, float* res, int len);
typedef void my_function_t(float*, float*, float*, int);
extern "C" {
    my_function_t* my_function_dispatch() {
        int dispatch_criteria = get_dispatch_criteria();
        if (dispatch_criteria > 10) {
            return &my_function_optimized;
        } else {
            return &my_function_default;
        }
    }
}
```


# gcc performance issues

- no string optimization in gcc < 5.1 and redhat

- no variant optimization for std::visit until gcc 12 - basically before that std::visit was using jump table based on function pointers, but now for up to 11 long variant it uses switch statement

- placement new does null pointer check in GCC < 7.1 and without c++17
  

# low level 

- _COARSE clock_gettime does not do a kernel mode switch
- trivial classes/structs can be copied with memcpy which is an important optimization!
   pair and tuple are not trivial classes!
 - Pause prevents speculative execution, eliminating the branch misprediction penalty, but it takes much longer these days up to 40ns

- A type that is trivial can be statically initialized
  standart layoout classes can be used in other programming languages!
  (gcc does not allow __attribute__((__packed__)) on structs that
  * contain non-POD data)

- Trivial class req (can be statically initialized and memcpy optimized)


# data structures

## vector like

### stable_vector

Each element is stored in its own separate node. All the nodes are referenced from a contiguous array of pointers, but also every node contains an "up" pointer referring back to the associated array cell. This up pointer is the key element to implementing stability and random accessibility:

Iterators point to the nodes rather than to the pointer array. This ensures stability, as it is only the pointer array that needs to be shifted or relocated upon insertion or deletion. Random access operations can be implemented by using the pointer array as a convenient intermediate zone

### devector
devector is a hybrid of the standard vector and deque containers originally written by Thaler Benedek. It offers cheap (amortized constant time) insertion at both the front and back ends, while also providing the regular features of vector, in particular the contiguous underlying memory.

Unlike vector, devector can have free capacity both before and after the elements. This enables efficient implementation of methods that modify the devector at the front. In general, devector's available methods are a superset of those of vector with similar behaviour, barring a couple of iterator invalidation guarantees that differ.


## Linked lists

### Unrolled linked lists

https://plflib.org/list.htm

The whole idea is to pack multiple elements into a single node, which would improve cache efficincy

### list implemented in vector

the idea is to use vector to store list nodes in slots and use the same vector to store free slots in case they get deallocated in the middle of the list!

### filtered list

When doing data filtering, first you are filling your linked list with data, and then you are removing data that is not according to a certain criterion. Linked lists are also used for this, but the process can be optimized.

In this particular case ordering of values doesn’t change if we remove other values. If we use std::vector to store our data, we can get rid of the next pointer, saving some space but also increasing the data cache hit rate. We can store information about used and freed values in a bitset. Each bit in a bitset corresponds to a value: if the bit set the value is present, otherwise it is left out.

Similarly, for a linked list backed by a vector, we can perform a compact operation to get rid of the gaps and make the memory access more data cache-friendly, after we have performed a sufficient number of element removal.

# how things work

## switch statements

gcc employs when compiling switch statements, including:

Compressing multiple case labels into a bitset.
Transforming the switch statement into a jump table.
Transforming the switch statement into a binary decision tree


# strict aliasign

linux kernel disables it ! that what Torvalds said:

In x86, I doubt _any_ amount of alias analysis makes a huge difference (as long as the compiler at least doesn't think that local variable spills can alias with anything else). Not enough registers, and generally pretty aggressively OoO (with alias analysis in hardware) makes for a much less sensitive platform

#  gperf

GNU gperf is a perfect hash function generator. For a given list of strings, it produces a hash function and hash table, in form of C or C++ code, for looking up a value depending on the input string. The hash function is perfect, which means that the hash table has no collisions, and the hash table lookup needs a single string comparison only

# SSO string

```
// gcc
struct string
{
    char* ptr;
    size_t size;
    union {
        size_t capacity;
        char buf[16];
    };

    bool is_large() { return ptr != buf; }
    auto data() { return ptr; }
    auto size() { return size; }
    auto capacity() { return is_large() ? capacity : 15; }
};
```

```
struct SmallString
{
    // When flag == 0, the entire structure
    // is a null-terminated string (SSO).  Otherwise 'big' points
    // to the string data, and 'sz' is its size.
    
    char const *c_str() const {
        return flag == 0
            ? reinterpret_cast<char const *>(&big)
            : big;
    }

    size_t size() const {
        return flag == 0 ? strlen(c_str()) : sz;
    }

    char *big;
    uint32_t sz;
    char pad[3];
    char flag;
};

char const *Get_c_str(SmallString const &str) {
    return str.c_str();
}

size_t Get_size(SmallString const &str) {
    return str.size();
}
```

## GCC / CLANG strict aliasing

Allow the compiler to assume the strictest aliasing rules applicable to the language being compiled. For C (and C++), this activates optimizations based on the type of expressions. In particular, an object of one type is assumed never to reside at the same address as an object of a different type, unless the types are almost the same. For example, an unsigned int can alias an int, but not a void* or a double. A character type may alias any other type.

Pay special attention to code like this:

```
union a_union {
  int i;
  double d;
};

int f() {
  union a_union t;
  t.d = 3.0;
  return t.i;
}
```

The practice of reading from a different union member than the one most recently written to (called “type-punning”) is common. Even with -fstrict-aliasing, type-punning is allowed, provided the memory is accessed through the union type. So, the code above works as expected. See Structures, Unions, Enumerations, and Bit-Fields. However, this code might not:

```
int f() {
  union a_union t;
  int* ip;
  t.d = 3.0;
  ip = &t.i;
  return *ip;
}
```

Similarly, access by taking the address, casting the resulting pointer and dereferencing the result has undefined behavior, even if the cast uses a union type, e.g.:

```
int f() {
  double d = 3.0;
  return ((union a_union *) &d)->i;
}
```

The -fstrict-aliasing option is enabled at levels -O2, -O3, -Os.
