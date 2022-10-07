/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
int bitXor(int x, int y) {
  int notx_y = ~x & y ;
  int not_notx_y = ~notx_y;
  int x_noty = x & ~y;
  int not_x_noty = ~x_noty;
  int x_bitxor_y = ~( not_notx_y & not_x_noty );
  return x_bitxor_y;
}
int tmin(void) {
  return 1 << 31;
}

int isTmax(int x) {
  int _x = 1;
  int ans = _x << 31;
  int _MAX = ans + (~_x + 1);
  return !(_MAX - x);
}

int allOddBits(int x) {
  int a = 0xaa;
  int a_8 = a<<8;
  int  low_16  = a|a_8;
  int high_16 = low_16<<16;
  int num = low_16 | high_16;
  int check = (x&num)^num;
  return !check;
}
int negate(int x){
    return ~x+1;
}
int isAsciiDigit(int x) {
  int x1 = 0x30;
  int x2 = 0x39;
  int a = x + ~x1 + 1;
  int b = x2 + ~x + 1;
  return !(a >> 31)&!(b >> 31);
}

int conditional(int x, int y, int z) {
  int x1 =~(!!x)+1;
  return (x1 & y) | (~x1 & z);
} 
int isLessOrEqual(int x, int y) {
  int x1 , y1 , m , n;
  x1 = x >> 31;
  y1 = y >> 31;
  m = x1 ^ y1;
  n = y + ~x +1;
  return( m & !!x1)|( !m & !(n>>31));
}
int logicalNeg(int x) {
  return ((x | (~x + 1)) >> 31) + 1;
}
int howManyBits(int x) {
  int b16,b8,b4,b2,b1,b0;
  int mask = x >> 31;
  x = ( mask & ~x ) | ( ~mask & x);
  b16 = !!(x>>16)<<4;
  x >>= b16;
  b8 = !!(x>>8)<<3;
  x >>= b8;
  b4 = !!(x>>4)<<2;
  x >>= b4;
  b2 = !!(x>>2)<<1;
  x >>= b2;
  b1 = !!(x>>1);
  x >>= b1;
  b0 = x;
  return b16 + b8 + b4 + b2 + b1 + b0 + 1;
}
unsigned floatScale2(unsigned uf) {
  int exp = (uf & 0x7f800000) >> 23;
  int sign = uf & (1 << 31);
  if(exp == 255)  return uf;
  if(exp == 0)  return (uf<<1) | sign;
  exp++;
  if(exp == 255)  return 0x7f800000 | sign;
  else  return (uf & 0x807fffff) | (exp << 23);
}
int floatFloat2Int(unsigned uf) {
  unsigned flag = uf >> 31;
  unsigned exp = ( uf >> 23) & 0xff;
  unsigned last_num = uf & 0x7fffff;
  int real_exp = exp - 127;
  unsigned real_last_num = last_num | 0x800000;
  if(exp == 0)
    return 0;
  if(exp == 255)
    return 0x80000000;
  if(real_exp >= 0)
  {
    if(real_exp>=31)
      return 0x80000000; 
    real_last_num = real_last_num >> (23 - real_exp); 
    if(flag)
      real_last_num= ~real_last_num+1;  
    return real_last_num;
  }
  return 0;
}
unsigned floatPower2(int x) {
  if(x>127)
    return 0x7f800000;
  if(x>=-126)
  {
    unsigned exp = x+127;
    return exp<<23;
  }
  if(x>=-149)
    return 0x1<<(x+149);
  return 0;
}