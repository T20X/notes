Now suppose that the lowest exponent that can be represented is -100. So the smallest number that can be represented in normal form is 1\*10-100. However, if we relax the constraint that the leading bit be a one, then we can actually represent smaller numbers in the same space. Taking a decimal example we could represent 0.1\*10-100. This is called a subnormal number. The purpose of having subnormal numbers is to smooth the gap between the smallest normal number and zero.
---------------------
It is very important to realise that subnormal numbers are represented with less precision than normal numbers. In fact, they are trading reduced precision for their smaller size. Hence calculations that use subnormal numbers are not going to have the same precision as calculations on normal numbers. So an application which does significant computation on subnormal numbers is probably worth investigating to see if rescaling (i.e. multiplying the numbers by some scaling factor) would yield fewer subnormals, and more accurate result
------------------------------------
https://docs.oracle.com/cd/E19957-01/806-3568/ncgTOC.html

# std::round & std::rint

std::round only suppports rounding halfway from zero
std::rint on the other hand uses custom rounding mode set by fesetround! Note that by default it is FE_TONEAREST (with half EVEN).

round()'s rounding is different from the IEEE754 default round to nearest mode with even as a tie-break. Nearest-even avoids statistical bias in the average magnitude of numbers, but does bias towards even numbers

For setting rounding mode through assembly, you may need to resort to assembly. In this case, you may want to set the rounding for both the x87 unit (via the fldcw instruction) and SSE (via the ldmxcsr instruction)

Prefer rint() for performance reasons: gcc and clang both inline it more easily, but gcc never inlines nearbyint() (even with -ffast-math)


float       round ( float arg );
float       roundf( float arg );
(1)	(since C++11)
double      round ( double arg );
Computes the nearest integer value to arg (in floating-point format), rounding halfway cases away from zero, regardless of the current rounding mode.



HALF EVEN rounding - it rounds towards nearest value and if both are equidistant then it rounds towards an even number. Not present in C++ standart. 
--->GNU<----: In this mode results are rounded to the nearest representable value. If the result is midway between two representable values, the even representable is chosen. Even here means the lowest-order bit is zero

-------------------------
The feenableexcept() function unmasks the floating-point exceptions represented by excepts. The future floating-point operations that produce excepts will trap, and a SIGFPE will be delivered to the process.

->>>>>> excetions can  be thrown as signals! NOT TRHOWING BY DEFAULT <<<<<---
or with  as normal c++ exceptions
GNU libc function feenableexcept() enables trapping of the floating-point exceptions, which generates the signal SIGFPE. If the compiler option -fnon-call-exceptions was used, the handler for that signal may throw a user-defined C++ exception.
-------------------------------------

The fedisableexcept() function masks the floating-point exceptions represented by excepts. All exceptions are masked by default.

The fegetexcept() function returns the current exception mask.
---------

Well, for a number to be finitely represented the denominator in a fraction should be a power of the system base. For example, for the base-10 system, the denominator should be a power of 10, that’s why we can finitely represent 0.625 in decimal:


and can’t finitely represent 1/3:



----------------------
Negative exponents could pose a problem in comparisons.

For example (with two's complement):

 	Sign	Exponent	Mantissa
1.0 × 2-1	0	11111111	0000000 00000000 00000000
1.0 × 2+1	0	00000001	0000000 00000000 00000000
With this representation, the first exponent shows a "larger" binary number, making direct comparison more difficult.

To avoid this, Biased Notation is used for exponents.

If the real exponent of a number is X then it is represented as (X + bias)

IEEE single-precision uses a bias of 127. Therefore, an exponent of

0 < e < 255	(-1)s × 2e-127 × 1.f (normal numbers)
e = 0; f  0 (at least one bit in f is nonzero)	(-1)s × 2-126 × 0.f (subnormal numbers)
e = 0; f = 0 (all bits in f are zero)	(-1)s × 0.0 (signed zero)
s = 0; e = 255; f = 0
(all bits in f are zero)	+INF (positive infinity)
s = 1; e = 255; f = 0
(all bits in f are zero)	-INF (negative infinity)
s = u; e = 255; f  0 (at least one bit in f is nonzero)	NaN (Not-a-Number)

---------------------------------------------
0/0 will give a nan for floating points which is not comparable!
12/0 is kind of ok for floating points as the result would be just infinity
----------bits
-fno-signaling-nans
-fno-trapping-math

IEEE standard recommends that implementations allow trap handlers to handle exceptions like divide by zero and overflow. This flags assumes that no use-visible trap will happen.

-funsafe-math-optimizations - These optimizations break the laws of floating-point arithmetic and may replace them with the laws of ordinary infinite-precision arithmetic:

Due to roundoff errors the associative law of algebra do not necessary hold for floating point numbers and thus expressions like (x + y) + z are not necessary equal to x + (y + z).

-ffinite-math-only - Special quantities such as inf or nan are assumed never to appear, this saves the time spent looking for them and handling them appropriately. For example, should x−x always be equal to 0.0?

-fno-errno-math

disables setting of the errno variable as required by C89/C99 on calling math library routines. For Fortran this is the default.

-fcx-limited-range

causes the range reduction step to be omitted when performing complex division. This uses a/b=((ar∗br+ai∗bi)/t)+i((ai∗br−ar∗bi)/t) with t=br∗br+bi∗bi and might not work well on arbitrary ranges of the inputs.

-fno-rounding-math

-fno-signed-zeros
Due to roundoff errors the associative law of algebra do not necessary hold for floating point numbers and thus expressions like (x + y) + z are not necessary equal to x + (y + z).
-------------
-------------
f the number is not normalized, then you can subtract 1 from the exponent while multiplying the mantissa by the base, and get another floating point number with the same value. Normalization consists of doing this repeatedly until the number is normalized. Two distinct normalized floating point numbers cannot be equal in value.
-------------
in the actual bits representing the floating point number, the exponent is biased by adding a constant to it, to make it always be represented as an unsigned quantit
-------------
Round to nearest.
This is the default mode. It should be used unless there is a specific need for one of the others. In this mode results are rounded to the nearest representable value. If the result is midway between two representable values, the even representable is chosen. Even here means the lowest-order bit is zero. This rounding mode prevents statistical bias and guarantees numeric stability: round-off errors in a lengthy calculation will remain smaller than half of FLT_EPSILON
--------------------------
general idea to reduce rounding error is to keep calculations with similar exponents to each other! keep big number with big number, little numbers with little numbers!
-----------------
1.0e-37. Minimum natural epxonent for base 10 which can be repsented normalized! Below that value would be non-normalized and matissa will keep destoring the more exponent goes down or just drops to 0!

1.0e-128 is 0!
1.oe-45 = 1.4013e-45
-----------------
relative error - diffrence between real number and appromixed number devided by the real number

(r - a) / r = "relative error"

----------------------------
A NaN never compares equal to itself. Copying a NaN may not preserve its bit representation
--------------------------

    int root = sqrt(n));
    return n == root * root
This won't always work, due to floating-point error: if sqrt() or cbrt() happens to return epsilon less than the actual root, the cast to an integer will truncate that, and the check will fail. To be completely bullet-proof against that, you also need to check if n == (root + 1) * (root + 1) for the square root case or if n == (root + 1) * (root + 1) * (root + 1) for the cube root cas
--------

https://www.youtube.com/watch?time_continue=23&v=mKJiD2ZAlwM - add two numbers!

123.45 = 0.12345 � 10+3. - true normalized form
a/b (b - denominator)
1/1000

Significand Exponent Scientific notation Fixed-point value
1.5 4 1.5 104 15000

Single precision Double precision
Total bits 32 64
Sign bits 1 1
Significand bits 23 52
Exponent bits 8 11
Smallest number 2-126 � 1.2 � 10-38 2
-1022 � 2.2 � 10-308
Largest number ca. 2 � 2
127 � 3.4 � 1038 ca. 2 � 2
1023 � 1.8 � 10308

The actual bit sequence is the sign bit first, followed by the exponent and finally the
significand bits.
� The exponent does not have a sign; instead an exponent bias is subtracted from it (127
for single and 1023 for double precision). This, and the bit sequence, allows floating-point
numbers to be compared and sorted correctly even when interpreting them as integers.
� The significand�s most significant bit is assumed to be 1 and omitted, except for special cases.
� There are separate positive and a negative zero values, differing in the sign bit, where all
other bits are 0. These must be considered equal even though their bit patterns are different.
� There are special positive and negative infinity values, where the exponent is all 1-bits and
the significand is all 0-bits. These are the results of calculations where the positive range of
the exponent is exceeded, or division of a regular number by zero.
� There are special not a number (or NaN) values where the exponent is all 1-bits and the
significand is not all 0-bits. These represent the result of various undefined calculations (like
multiplying 0 and infinity, any calculation involving a NaN value, or application-specific
cases). Even bit-identical NaN values must not be considered equal.



While the errors in single floating-point numbers are very small, even simple calculations on them
can contain pitfalls that increase the error in the result way beyond just having the individual errors
�add up�.
In general:
� Multiplication and division are �safe� operations
� Addition and subtraction are dangerous, because when numbers of different magnitudes are
involved, digits of the smaller-magnitude number are lost.
� This loss of digits can be inevitable and benign (when the lost digits also insignificant for the
final result) or catastrophic (when the loss is magnified and distorts the result strongly).
� The more calculations are done (especially when they form an iterative algorithm) the more
important it is to consider this kind of problem.
12
5 APENDIX
� A method of calculation can be stable (meaning that it tends to reduce rounding errors) or
unstable (meaning that rounding errors are magnified). Very often, there are both stable and
unstable solutions for a problem.

-------------------
In addition GCC offers the -ffast-math flag which is a shortcut for several options, presenting the least conforming but fastest math mode. It enables -fno-trapping-math, -funsafe-math-optimizations, -ffinite-math-only, -fno-errno-math, -fno-signaling-nans, -fno-rounding-math, -fcx-limited-range and -fno-signed-zeros. Each of these flags violates IEEE in a different way. -ffast-math also may disable some features of the hardware IEEE implementation such as the support for denormals or flush-to-zero behavior. An example for such a case is x86_64 with it's use of SSE and SSE2 units for floating point math. The flags under -ffast-math can be divided into four categories according to the different aspect of IEEE they violate; as described in the following table:

Category

Flags

Comments

Trap handlers and exceptions

-fno-trapping-math, -fno-signaling-nans

IEEE standard recommends that implementations allow trap handlers to handle exceptions like divide by zero and overflow. This flags assumes that no use-visible trap will happen.

Rounding

-fno-rounding-math

IEEE has four rounding modes. This flag assumes that the rounding mode is round to nearest.

Languages and compilers

-funsafe-math-optimizations

Due to roundoff errors the associative law of algebra do not necessary hold for floating point numbers and thus expressions like (x + y) + z are not necessary equal to x + (y + z).

Special quantities (Nans, signed zeros and infinity)

-------
Generally, the purpose of a signaling NaN (sNaN) is for debugging. For example, floating-point objects might be initialized to sNaN. Then, if the program fails to one of them a value before using it, an exception will occur when the program uses the sNaN in an arithmetic operation. A program will not produce an sNaN inadvertently; no normal operations produce sNaNs. They are only created specifically for the purpose of having a signaling NaN, not as the result of any arithmetic