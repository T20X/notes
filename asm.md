# Links

https://en.wikibooks.org/wiki/X86_Assembly/X86_Architecture#:~:text=For%20example%2C%20CL%20is%20the,%2C%20and%208%2Dbit%20MSB.

# Tricks

- xor rax, rax - quick way to zero something!

# Endian 

Big endian multibyte numbers are stored in memory starting with the most significant bytes.

Little endian multibyte numbers are stored in memory starting with the least significant bytes.

The advantage of little endian is that we can discard the most significant bytes effectively converting the number from a wider format to a narrower one, like 8 bytes.




# syscall 

note that glibc may not have all the supported linux kernel syscalls, but still it gives you entrypoint called "syscall API" which allows you to call a specific syscall supported by the kernel. You may wish not to use glibc at all and use minimalistic libarary shipped with each kernel called **nolibc**

2. A system-call is done via the syscall instruction. The kernel destroys
registers %rcx and %r11. syscall leaves rcx and r11 changed
3. The number of the syscall has to be passed in register %rax.
4. System-calls are limited to six arguments, no argument is passed directly on
the stack.
5. Returning from the syscall, register %rax contains the result of the
system-call. A value in the range between -4095 and -1 indicates an error,
it is -errno.
6. Only values of class INTEGER or class MEMORY are passed to the kernel

CC dictates how the stack is used. Contract between caller and callee on x86 for REGULAR FUNCTION CALLS:

    * after call instruction:
          o %eip points at first instruction of function
          o %esp+4 points at first argument
          o %esp points at return address 
    * after ret instruction:
          o %eip contains return address
          o %esp points at arguments pushed by caller
          o called function may have trashed arguments
          o %eax contains return value (or trash if function is void)
          o %ecx, %edx may be trashed
          o %ebp, %ebx, %esi, %edi must contain contents from time of call 
    * Terminology:
          o %eax, %ecx, %edx are "caller save" registers
          o %ebp, %ebx, %esi, %edi are "callee save" registers

syscall leaves rcx and r11 changed




# JUMPS

cmd  - just computes arithmetic diffrence and set ZF if it evaluates to 0
jxxxx set of instructions - do comparions (neeed to use diffrent ones per signed/unsigned) as well as makes a jump by label. NOT function!
Each Jxxxx function has logical NOT alternative
for example ja and jna(!ja)
test - performs bitwise comparison


# STACK

        push    rbp
        mov     rbp, rsp
        mov     DWORD PTR [rbp-4], edi (-4) is required because the stack is growing downwards!
        
        Example: 
                     RSP --->128 (Prev RBP(starts at 128!))
                             124 <----EDI has to be written here! that is why -4 comes into play like that!
                             120 


64 bit stack alignment is mandatory


## Stack alignment

"Stack alignment" just means the address of the stack (SP or ESP) is a multiple of the machine word size (so always divisible by 8 for 64-bit mode, 4 for 32-bit, 2 for 16-bit). Generally this means you assume your BIOS / bootloader / OS / runtime initializes it that way, and you only change ESP by multiples of the word size. Writing 64-bit software and want to push/pop a single byte on the stack? To keep the stack aligned, you should change ESP by 8 (the number of bytes in 64 bits). You don't have to do anything with the extra 7 bytes, but you need to allocate them to maintain the invariant that ESP is always a multiple of 8.

Why is this a thing? It's because CPU / cache / memory generally talk to each other only about aligned addresses (this allows less address pins and hashmap pressure issues because low bits are always zero). But it complicates implementation of what the hardware should do when software asks for an un-aligned read/write (you'll need to split it into two when it crosses the alignment boundary). Some CPU's handle this by making unaligned memory access an error.

What about x86? Unaligned memory access is allowed on x86, but it's slower. Which means it's better for software to avoid it where possible [1]. So low-level libraries, OS's, compilers, etc. are all written to keep ESP aligned. And it's also advised for any hand-written assembly code.


# MATH

Do not confuse the "carry" flag with the "overflow" flag in integer
arithmetic.  Each flag can occur on its own, or both together.  The CPU's
ALU doesn't care or know whether you are doing signed or unsigned
mathematics; the ALU always sets both flags appropriately when doing any
integer math.  The ALU doesn't know about signed/unsigned; the ALU just
does the binary math and sets the flags appropriately.  It's up to you,
the programmer, to know which flag to check after the math is done

## division

the same asm command idiv used for division and modulo calc. It takes even for int about 20-30 cycles. And there is normally only one on core...It is smart mostly to avoid it  by using some clever bit magic if you optimize at least with O2.

## multiplication

normally takes about 4 cycles

![](images/asm/binary_multiplications.JPG)


## subtraction

Note that the main about about subtraction in binary is to borrow 1s from the left (most significant side)) in case there is 0 - 1 case (other wise it would be negative number). As you borrow from the left, it becomes 2 so that 2 - 1 is 1.

![](images/asm/binary_subtraction.JPG)




## Flags

Carry Flag (CF) - this flag is set to 1 when there is an unsigned overflow. For example when you add bytes 255 + 1 (result is not in range 0...255). When there is no overflow this flag is set to 0.

Zero Flag (ZF) - set to 1 when result is zero. For none zero result this flag is set to 0.

Sign Flag (SF) - set to 1 when result is negative. When result is positive it is set to 0. Actually this flag take the value of the most significant bit.

Overflow Flag (OF) - set to 1 when there is a signed overflow. For example, when you add bytes 100 + 50 (result is not in range -128...127).

### Carry Flag

https://teaching.idallen.com/dat2343/10f/notes/040_overflow.txt

The rules for turning on the carry flag in binary/integer math are two:

1. The carry flag is set if the addition of two numbers causes a carry
   out of the most significant (leftmost) bits added.

   1111 + 0001 = 0000 (carry flag is turned on)

2. The carry (borrow) flag is also set if the subtraction of two numbers
   requires a borrow into the most significant (leftmost) bits subtracted.

   0000 - 0001 = 1111 (carry flag is turned on)

Otherwise, the carry flag is turned off (zero).
 * 0111 + 0001 = 1000 (carry flag is turned off [zero])
 * 1000 - 0001 = 0111 (carry flag is turned off [zero])

In unsigned arithmetic, watch the carry flag to detect errors.
In signed arithmetic, the carry flag tells you nothing interesting and you need to watch overflow flag (OF).

### Overflow Flag

The rules for turning on the overflow flag in binary/integer math are two:

1. If the sum of two numbers with the sign bits off yields a result number
   with the sign bit on, the "overflow" flag is turned on.

   0100 + 0100 = 1000 (overflow flag is turned on)

2. If the sum of two numbers with the sign bits on yields a result number
   with the sign bit off, the "overflow" flag is turned on.

   1000 + 1000 = 0000 (overflow flag is turned on)


# MEMORY

It is, however, important to mention that despite the fact that it is a 64-bit architecture, the MMU only uses the first 48 bits of the virtual address (also called the linear addresss

Address bits	Meaning
0 - 11	Offset into a 4 KB page
12 - 20	Index of a page entry in the table of 512 pages
21 - 29	Index of a page table entry in the page directory
30 - 38	Index of a page directory entry in the page directory pointer table
39 - 47	Index of a page directory pointer table in the page-map level 4 table


# SEGMENTS and GDT

You can’t change cs directly

Each of segment registers cs, ds, ss, es, gs, and fs stores so-called segment selector , containing an index in a special segment descriptor table and a little additional information


This way we could create numerous segments with various permissions and use RPL values in segment selectors to define which of them are accessible to us right now (given our privilege level)

Privilege levels are the same thing as protection rings!

For example, we are currently in ring0 and ds= 0x02. Even though the lowest two bits of cs and ss are 0 (as we are inside ring0), we can’t access data in a segment with privilege level higher than 2 (like 1 or 0)

If you try, for instance, to load a new value into a segment register, the CPU needs to perform safety and access control checks to see whether you're actually entitled to access that specific memory area

You might think that every memory transaction needs another one now to read GDT contents. This is not true: for each segment register there is a so-called shadow register, which cannot be directly referenced. It serves as a cache for GDT contents. It means that once a segment selector is changed, the corresponding shadow register is loaded with the corresponding descriptor from GDT. Now this register will serve as a source of all information needed about this segment.

(PROTECTED MODE) Linear address = segment base (taken from system table) + offset
(REAL MODE) physical address = segment base * 16 + offset
(REAL MODE) All memory addressing through main segment registers (cs, ds, es, and ss) do not consider the GDT values of base and offset anymore. The segment base is always fixed at 0x0 no matter the descriptor contents; the segment sizes are not limited at all. The other descriptor fields, however, are not ignored

# REGISTERS

GDTR is a register to store GDT address and size.

GS is commonly used as a pointer to a thread local storage (TLS) 


if you write to EAX (any other 32 bit register), it would zero our remaning bit in RAX (any other 64 bit register)

 RIP register
  . It is a 64-bit register, which always stores an address of the **next** instruction to be executed. Branching instructions (e.g., jmp) are in fact modifying it. So, every time any instruction is being executed, rip stores the address of the next instruction to be execute

rax..rdx - general purpose! (Accumulator, Base, Counter, Data)

  RAX - normally holds return value in function calls

GS register stores the base address for per-cpu are
ECX(RCX) - counter register - used mostly within loops
EAX/EDX - accumulators (have special meaning for multiplication and division)
ESI and EDI registers serve as source and destination index registers in string operations, where ESI contains the source address and EDI, obviously, the destination address

CR3: This register is used when virtual addressing is enabled (paging) and contains the physical address of the page directory, page directory pointer table, or page map level 4 table, depending on the current mode of operation.

RFlags  register
Bit #0 - carry flag
The carry flag (CF) is mostly used for the detection of carry/borrow in arithmetic operations and is set if the bit width result of the last such operation (such as addition and subtraction) exceeds the width of the ALU. For example, the addition of two 8-bit values, 255 and 1, would result in 256, which requires at least nine bits to be stored. In such a case, bit eight (the ninth bit) is placed into the CF, thus, letting us and the processor know that the last operation ended with carry.

Bit #6 - zero flag
The zero flag (ZF) is set when the result of an arithmetic or bitwise operation is 0. This includes operations that do not store the result (for example, comparison and bit test). like if the expression evaluates to 0,  the flag itself would be set to 1

Bit #9 - interrupt enable flag
The interrup enable flag (IF) defines whether processor will or will not react to incoming interrupts. This flag is only accessible in real mode or at the Ring 0 protection level in other mode

Bit #7 - sign flag
sets (SF) is operation result is negative

Bit #11 - overflow flag
The overflow flag (OF) is sometimes perceived as two's complement form of the carry flag, which is not really the case. OF is set when the result of the operation is either too small or too big a number to fit into the destination operand.


----
params
---------          <--- frame pointer!
return adddress
-------
locals
----               < -- stack pointer!

r12 - temporary register free to use in just current subroutine! gets cleared before 
subrouting calls

rbp is the frame pointer on x86_64. In your generated code,
 it gets a snapshot of the stack pointer (rsp) so that when adjustments are made 
to rsp (i.e. reserving space for local variables or pushing values on to the stack), 
local variables and function parameters are still accessible from a constant offset from rbp.
A lot of compilers offer frame pointer omission as an optimization option; this will make the
 generated assembly code access variables relative to rsp instead and free up rbp as another
 general purpose register for use in function

 The first six arguments are passed in rdi, rsi, rdx, rcx, r8, and r9, respectively. The rest is passed on to the stack in reverse order



In common usage, the word size refers to the size of a processor register. However historically word is 16 bit...
 On a 32 bit processor, this will be 32 bits. On a 64 bit processor, this will be 64 bits

# VDSO

![](images/asm/vdso.JPG)

# FUNCTION CALL on x64

![](images/asm/function_call.JPG)


User-level applications use as integer registers for passing the sequence
%rdi, %rsi, %rdx, %rcx, %r8 and %r9. The kernel interface uses %rdi,
%rsi, %rdx, %r10, %r8 and %r9.

If you change rbx, rbp, rsp, or r12-r15, change them back!
If you need any other register to survive function call, save it yourself before calling!

So, the pattern of calling a function is as follows:
1. Save all caller-saved registers you want to survive function call (you can use push for that). Caller saved registers are all NOT these ones 
rbx, rbp, rsp, r12-r15

2. saves RIP on the stack as well and sets to the function address to be called
rbx, rbp, rip, rsp, r12-r15

2. Store arguments in the relevant registers (rdi, rsi, etc.).
4. Invoke function using call.
5. On callee side, called function must save calle-saved registers if it wants to use them **rbx, rbp, rsp, r12-r15** !
6. Execute called function 
7. Restore calle-saved registers **rbx, rbp, rsp, r12-r15** !
8. Return back to the caller
9. Caller would restore its own registers



# INSTRUCTIONS

## SAR / SHR 


The SHR (Shift Right) and SAR (Arithmetic Shift Right) instructions are both used in computer programming for shifting the bits of a binary number to the right. The main difference between the two instructions lies in how they handle the most significant bit (MSB) during the shift. The SHR instruction shifts all the bits to the right, including the MSB, and fills the vacated MSB position with a 0. On the other hand, the SAR instruction also shifts all the bits to the right, but it preserves the original MSB. This means that the SAR instruction fills the vacated MSB position with the original value of the MSB, effectively preserving the sign of the number when used in signed number representation. In summary, SHR is a logical shift that always fills the MSB with 0, while SAR is an arithmetic shift that preserves the sign of the number by filling the MSB with the original value of the MSB

## PUSH

This instructs the processor to store the value of the operand onto a stack and decrements stack pointer (ESP register on 32-bit systems and RSP register on 64-bit ones).

## MOV

This is the most commonly used instruction for transferring data:
It moves data between registers of the same size
It loads a register with either an immediate value or a value read from memory
It stores the content of a register to memory
It stores the immediate value to memory

mov rsi, [rax]—copies memory contents (8 sequential bytes) starting at address, stored in rax, into rsi


## MOVZX

This is less powerful than MOV by means of addressing modes, as it may only transfer data from register to register or from memory to register, but it has its special feature--the value being transferred is converted to a wider (one that uses more bits) one and is zero extended. As to the addressing modes supported by this instruction, it may only do the following:
It moves the byte value from register or memory to a word-sized register and extends the resulting value with zeroes (one byte would be added)
It moves byte value from register or memory to a double word-sized register, in which case three bytes would be added to the original value and the value itself would be extended with zeroes
It moves word-sized value from register or memory to a double word-sized register, adding two bytes and filling them with the extension value of 0

## MOVSX

is similar to MOVZX; however, the extended bits are filled with the sign bit of the source operand.

## BSWAP/MOVBE

 The BSWAP instruction is the easiest way to switch the endianness of a value; however, it is not really a transfer instruction as it only rearranges data within a register. The BSWAP instruction only works on 32/64-bit operands. MOVBE is a more convenient instruction for swapping byte order as it also moves data between the operands. This instruction works on 16, 32, and 64-bit operands. It cannot move data from register to register.

## POP

 This retrieves values previously stored on stack. The only operand of this instruction is the destination where the value should be stored, and it may be a register or a location in memory. This instruction increments the stack pointer register as well.

## LEA

; rsi <- address of label 'codes', a number
mov rsi, codes

; rsi <- memory contents starting at 'codes' address
; 8 consecutive bytes are taken because rsi is 8 bytes long
mov rsi, [codes]

; rsi <- address of 'codes'
; in this case it is equivalent of mov rsi, codes
; in general the address can contain several components
lea rsi, [codes]

; rsi <- memory contents starting at (codes+rax)
mov rsi, [codes + rax]

; rsi <- codes + rax
; equivalent of combination:
; -- mov rsi, codes
; -- add rsi, rax
; Can't do it with a single mov!
lea rsi, [codes + rax]

**THIS IS REALLY TRICKY COMMAND**

MOV with brackets will get what's at the address written in the brackets (which can be arithmetic or not). Without the brackets (no arithmetic possible), it will write the address to the register. LEA only works with the brackets (so arithmetic possible) and will basically do what MOV does when the brackets are not provided. So both are useful. Aside from going on the given address, MOV can also do what LEA does, except it can only do it without the brackets - but without them, no arithmetic is possible. So LEA "corrects" that


LEA EAX, [ EAX + 1 ] and INC EAX is that the latter changes EFLAGS but the former does not; this preserves CMP state


**WARNING** There's a big difference between 2 operand LEA which is fast and 3 operand LEA which is slow. The Intel Optimization manual says fast path LEA is single cycle and slow path LEA takes three cycles. Moreover, on Skylake there are two fast path functional units (ports 1 and 5) and there's only one slow path functional unit (port 1)

out-of-order CPUs typically run LEA on ALUs

LEA doesn't always calculate; it calculates if the effective address expressed in the source operand calculates. LEA EAX, GLOBALVAR doesn't calculate; it just moves the address of GLOBALVAR into EAX. LEA is not a specific arithmetic instruction; it is a way of intercepting the effective address arising from any one of the processor's memory addressing modes.

This is the reason why people who think that the brackets in LEA are superfluous are severely mistaken; the brackets are not LEA syntax but are part of the addressing mode

LEA is real at the hardware level. The generated instruction encodes the actual addressing mode and the processor carries it out to the point of calculating the address. Then it moves that address to the destination instead of generating a memory reference. (Since the address calculation of an addressing mode in any other instruction has no effect on CPU flags, LEA has no effect on CPU flags.)

## CMP

cmp arg2, arg1

We use cmp arg2, arg1 when we care about whether arg1 and arg 2 are equal. The processor determines this by subtracting arg2 from arg1, then looking at the result. If the result is zero (that is, arg1 = arg2), then the processor sets the zero flag (by "sets the flag", we mean it sets it to 1). Conversely, if the result isn't zero (that is, arg1 != arg2), then the processor clears the zero flag (i.e, sets it to 0). The result itself is discarded, because we don't care what it is, only whether it's zero or not, which we now know based on whether the zero flag is set. We can then use instructions like JE, JNE, JZ and JNZ that examine the zero flag and jump (or not) based on its value. In the case of JE (jump if equal), the jump will happen if the zero flag is set, which (as we learned above) it will be if the arguments in the cmp were equa

the results of CMP is changing the values of ZF and CF

# integration with C/C++

if you want to globally enable Intel syntax, just pass this in compile flags -masm=intel. This works in both g++ and clang(as of version 14). In reality this Intel syntax for GAS GNU Assembler is only emerging and it does not work very well with inline assembly (though one can use mixed mode when some parts of inline asm are in Intel ASM and another in AT&T one, but again it won't work very well particularly with clang). In fact only -masm=intel seems to work with clang which also breaks mixing Intel and AT&T

For standalone asm files, many people just prefer NASM!

## extended asm

Extended asm statements have to be inside a C function, so to write inline assembly language at file scope (‘top-level’), outside of C functions, you must use basic asm. You can use this technique to emit assembler directives, define assembly language macros that can be invoked elsewhere in the file, or write entire functions in assembly language. Basic asm statements outside of functions may not use any qualifiers.

With extended asm you can read and write C variables from assembler and perform jumps from assembler code to C labels. Extended asm syntax uses colons (:) to delimit the operand parameters after the assembler template:

asm asm-qualifiers ( AssemblerTemplate
                 : OutputOperands
                 [ : InputOperands
                 [ : Clobbers ] ])

asm asm-qualifiers ( AssemblerTemplate
                      : OutputOperands
                      : InputOperands
                      : Clobbers
                      : GotoLabels)

if it can make any side-effect it has to be declared volatile

**AssemblerTemplate**
This is a literal string that is the template for the assembler code. It is a combination of fixed text and tokens that refer to the input, output, and goto parameters. See Assembler Template.

**OutputOperands**
A comma-separated list of the C variables modified by the instructions in the AssemblerTemplate. An empty list is permitted. See Output Operands.

**InputOperands**
A comma-separated list of C expressions read by the instructions in the AssemblerTemplate. An empty list is permitted. See Input Operands.

**Clobbers**
A comma-separated list of registers or other values changed by the AssemblerTemplate, beyond those listed as outputs. An empty list is permitted. See Clobbers and Scratch Registers.

**GotoLabels**
When you are using the goto form of asm, this section contains the list of all C labels to which the code in the AssemblerTemplate may jump. See Goto Labels.
asm statements may not perform jumps into other asm statements, only to the listed GotoLabels. GCC’s optimizers do not know about other jumps; therefore they cannot take account of them when deciding how to optimize.
The total number of input + output + goto operands is limited to 30.



## basic asm

volatile by default

Functions declared with the naked attribute also require basic asm (see Declaring Attributes of Functions).

Safely accessing C data and calling functions from basic asm is more complex than it may appear. To access C data, it is better to use extended asm.
Do not expect a sequence of asm statements to remain perfectly consecutive after compilation. If certain instructions need to remain consecutive in the output, put them in a single multi-instruction asm statement. Note that GCC’s optimizers can move asm statements relative to other code, including across jumps. asm statements may not perform jumps into other asm statements. GCC does not know about these jumps, and therefore cannot take account of them when deciding how to optimize. Jumps from asm to C labels are only supported in extended asm.

For basic asm with non-empty assembler string GCC assumes the assembler block does not change any general purpose registers, but it may read or write any globally accessible variable

## links

https://gcc.gnu.org/onlinedocs/gcc/Using-Assembly-Language-with-C.html

# tools

## disassble execuable

objdump -d  ./a.out 

## gdb

disassemble

must have 
-------
layout asm
layout reg
-----------


i r <register_name>: print a single register, e.g i r rax, i r eax
i r <register_name_1> <register_name_2> ...: print multiple registers, e.g i r rdi rsi,
i r: print all register except floating point & vector register (xmm, ymm, zmm).
i r a: print all register, include floating point & vector register (xmm, ymm, zmm).
i r f: print all FPU floating registers (st0-7 and a few other f*)

# YASM

Previously, because NASM was licensed under LGPL, it led to development of Yasm, a complete rewrite of the NASM under the New BSD License. Yasm offered support for x86-64 earlier than NASM. It also added support for GNU Assembler syntax. So it is preffered assember 

## default rel

enables RIP addressing

# NASM

better stick with it

# GAS AT&T syntax

## PIE data access
       lea     message(%rip), %rsi   #  PIE : necessary to do like this for position independed code    */

## Non-PIE data acces
 lea     message, %rsi     # Absolute addressing: necessary to do like this for position independed code 

