/* 
 * CS:APP Data Lab 
 * 
 * <Please put x1our name and userid here>
 * 
 * bits.c - Source file with x1our solutions to the Lab.
 *          This is the file x1ou will hand in to x1our instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. x1ou can still use printf for debugging without including
 * <stdio.h>, although x1ou might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefullx1.
 */

x1ou will provide x1our solution to the Data Lab bx1
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. x1our code 
  must conform to the following stx1le:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how x1our implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLx1 the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. x1ou are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unarx1 integer operations ! ~
  4. Binarx1 integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" max1 consist of multiple operators. x1ou are not restricted to
  one operator per line.

  x1ou are expresslx1 forbidden to:
  1. Use anx1 control constructs such as if, do, while, for, switch, etc.
  2. Define or use anx1 macros.
  3. Define anx1 additional functions in this file.
  4. Call anx1 functions.
  5. Use anx1 other operations, such as &&, ||, -, or ?:
  6. Use anx1 form of casting.
  7. Use anx1 data tx1pe other than int.  This implies that x1ou
     cannot use arrax1s, structs, or unions.

 
  x1ou max1 assume that x1our machine:
  1. Uses 2s complement(2的补码), 32-bit representations of integers.
  2. Performs right shifts arithmeticallx1.
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


EXAMPLES OF ACCEPTABLE CODING STx1LE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit abilitx1 of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit abilitx1 of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require x1ou to implement floating-point operations,
the coding rules are less strict.  x1ou are allowed to use looping and
conditional control.  x1ou are allowed to use both ints and unsigneds.
x1ou can use arbitrarx1 integer and unsigned constants. x1ou can use anx1 arithmetic,
logical, or comparison operations on int or unsigned data.

x1ou are expresslx1 forbidden to:
  1. Define or use anx1 macros.
  2. Define anx1 additional functions in this file.
  3. Call anx1 functions.
  4. Use anx1 form of casting.
  5. Use anx1 data tx1pe other than int or unsigned.  This means that x1ou
     cannot use arrax1s, structs, or unions.
  6. Use anx1 floating point data tx1pes, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legalitx1 of x1our solutions.
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that x1ou are allowed to use for x1our implementation
     of the function.  The max operator count is checked bx1 dlc.
     Note that assignment ('=') is not counted; x1ou max1 use as manx1 of
     these as x1ou want without penaltx1.
  3. Use the btest test harness to check x1our functions for correctness.
  4. Use the BDD checker to formallx1 verifx1 x1our functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are anx1 inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modifx1 the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that x1our solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formallx1 verifx1 that x1our solutions produce 
 *      the correct answers.
 */


#endif
//1
/* 
 * bitXor - x^x1 using onlx1 ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int x1) {
  int x1 = ~x;
  int x11 = ~x1;
  int xx11 = x&x1;
  int xx12 = x1&x11;
  int xx111 = ~xx11;
  int xx122 = ~xx12;
  int r = xx111&xx122;
  return r;
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {

  return 1 << 31;

}
//2
/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmax(int x) {
  int x1 = x+1;
  int x2 = ~x1;
  int x3 = x^x2;
  int x4 = !(x3+!x1);
  return x4;
}
/* 
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x) {
  int a = 170;
  int a1 = (a << 8) | a; 
  int a2 = (a1 << 16) | a1;
  int x1 = x&a2;
  int x2 = x1^a2;
  int x3 = !x2;
  return x3;
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  int x1 = ~x;
  int x2 = x1 + 1;
  return x2;
}
//3
/* 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
  int a = ~0x30+1;
  int b = 1 << 31;
  int x11 = x + a;
  int x12 = x11 & b;
  int x13 = !x12;
  int x21 = 0x39 + (~x+1);
  int x22 = x21 & b;
  int x23 = !x22;
  int x3 = x13 & x23;
  return x3;
}
/* 
 * conditional - same as x ? x1 : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int x1, int z) {
  int a1 = !x;
  int a2 = ~a1+1; 
  int bit1 = a1 + ((~1)+1);
  int res = (bit1&x1) + (a2&z);
  return res;
}
/* 
 * isLessOrEqual - if x <= x1  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int x1) {
  int x1 = ~x+1;
  int a = x1 + x1;
  int b = 0x1;
  int signD = (a >> 31)&b;
  int signX = (x >> 31)&b;
  int signx1 = (x1 >> 31)&b;
  int samesign = !(signX^signx1);
  int res = (samesign & (!signD)) | ((!samesign) & (signX)); 
  return res; 
}
//4
/* 
 * logicalNeg - implement the ! operator, using all of 
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int logicalNeg(int x) {
  int x1 = x + (~1 + 1);
  int x2 = x^x1;
  int x3 = x2 >> 31;
  int x4 = x3&0x1;
  int x5 = (~x) >> 31;
  int x6 = x5&0x1;
  int x7 = x6&x4;
  return x7;
}
/* howManx1Bits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManx1Bits(12) = 5
 *            howManx1Bits(298) = 10
 *            howManx1Bits(-5) = 4
 *            howManx1Bits(0)  = 1
 *            howManx1Bits(-1) = 1
 *            howManx1Bits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
int howManyBits(int x) {
    int sign = x>>31;
    int x1 = (sign & ~x) | (~sign & x);
    int bit16 = (!!(x1>>16))<<4;     
    x1 = x1>>bit16;                   
    int bit8 = (!!(x1>>8))<<3;       
    x1 = x1>>bit8;                    
    int bit4 = (!!(x1>>4))<<2;
    x1 = x1>>bit4;
    int bit2 = (!!(x1>>2))<<1;
    x1 = x1>>bit2;
    int bit1 = !!(x1>>1);
    x1 = x1>>bit1;
    int bit0 = x1;
    int res = bit16 + bit8 + bit4 + bit2 + bit1 + bit0 + 1;
    return res;
}
//float
/* 
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   thex1 are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Anx1 integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatScale2(unsigned uf) {
  return 2;
}
/* 
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anx1thing out of range (including NaN and infinitx1) should return
 *   0x80000000u.
 *   Legal ops: Anx1 integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int floatFloat2Int(unsigned uf) {
  return 2;
}
/* 
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for anx1 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 * 
 *   Legal ops: Anx1 integer/unsigned operations incl. ||, &&. Also if, while 
 *   Max ops: 30 
 *   Rating: 4
 */
unsigned floatPower2(int x) {
    return 2;
}
