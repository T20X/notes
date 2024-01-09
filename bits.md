# integers

2-complemantry format is been used officially

note that max vals for signed numbers normally smaller that for min values as absolute

for eample it is 127 for int8 and -128 for int8 min

unsgiend numbers do wrap nicely, but note that max + 1 would be equal to 0, so you always use one on each wrap. For example max + max would be max - 1

int sum = -total - that is a nifty trick to make the number negative instead of doing -1 * total


# binary multiplication

![](images/asm/binary_multiplications.JPG)

# any type to bits

```
template <class T> std::string get_bits2(const T &v) {
  const size_t bits_num = sizeof(T) * 8u;
  std::string r(bits_num, '0');

  unsigned char underlying_bits[sizeof(T)];
  memcpy((void *)underlying_bits, &v, sizeof(v));

  const unsigned char base{1u << 7u};
  for (size_t i = 0; i < bits_num; ++i) {
    uint8_t byte_idx = i / 8u;
    uint8_t bit_idx = i % 8u;
    char bit = (underlying_bits[byte_idx] & (base >> bit_idx)) ? '1' : '0';
    r[i] = bit;
  }

  return r;
}
```

# clear least significant bit
a & (a - 1)

# set K's bit to 1
x | (1 << K)

# set K's bit to 0

x & ~(1 << k)

# get number with all 1s bits set

-1!

(int)-1 will get you number will all 1s bits set
(size_t)-1 will get you number will all 1s bits set

#  Why 1U shall be used instead of 1 for bit manipulations

What problems happen when number is a wider type than 1?
x may be too great for the shift 1 << x leading to undefined behavior (UB). Even if x is not too great, ~ may not flip enough most-significant-bits.

// Assume 32 bit int/unsigned
unsigned long long number = foo();

unsigned x = 40; 
number |= (1 << x);  // UB
number ^= (1 << x);  // UB
number &= ~(1 << x); // UB

x = 10;

Generally prefer to use unsigned types for bit manipulation


# fast abs calc

```
int abs(int a) {
  int const mask = 
        a >> sizeof(int) * CHAR_BIT - 1;
    return  = (a + mask) ^ mask;
}
```



(0) -1 trick! N - 1 will get all 1111 to right if N is power of 2
 For example: 4(0100) - 1 = 3(0011)
(1) LSB The expression x & (-x) clears all but the least-significant 1 bit,
(2) MSB

    msb = 1 << (BITS - 1);
    /* Perform bitwise AND with msb and num */
    if(num & msb)
        printf("MSB of %d is set (1).", num);
    else
        printf("MSB of %d is unset (0).", num);

(2) if N is power of 2! N & ( N - 1) == 0
(3) modulo!  pos = (pos+1) & (N - 1)

(4) bool odd(int n) { return n & 0x1; }
(5) int half(int n) { return n >> 1; }

(6) toggle j-th bit status
  i ^= (1 << j)

(7) swap without tmp variable
a = a^b      # mix of a and b
b = a^b      # a^b^b = a
a = a^b      # a^b^a = b

And XOR swap fails if you accidentally try to swap a value with itsel

(8) x = x | 0 and x = x ^ 0. Both are equivalent!

(9)     // k = 3, p = 1 
    int singleNumber(vector<int>& nums)
    {
        // even though we use integers here, but all the arhmnetic can be thought of as happening
        // per bit basis
        
        int x1 = 0, x2 = 0;
        for (auto v : nums)
        {
            // increment 2-bit counter rth bit counter for v( three times fit into 2 bits!)
            // example
            // 1st item got 1 in rth bit, then x1 rth bit is also 1
            // 2s item got 1 in rth bit, then x2 rth bit is also 1, but x1 gets XORed to 0
            // 3s item got 1 in rth bit, then x1 rth bit becomes 1
            x2 ^= (x1 & v);
            x1 ^= v;
            
            // need to reset the counter if it reaches 3!
            // for each rth bit of x1 and x2 we want to have a mask which would have for corresponding
            // bit 0 if we want to clear it with (&) and 1 otherwise if we want to keep the counter
            int mask = ~(x1 & x2); // if k is 4 then we'd have x1,x3,x3 for counters (3-bit counters) and
            //mask would have been 1(x3)0(x2)0(x1), which means formula for mask would become 
            //int mask = ~(~x1 & ~x2 & x3)
            
            //now need to apply the mask to reset correspondign counters 
            x2 &= mask;
            x1 &= mask;
        }
        
        return x1;
    }