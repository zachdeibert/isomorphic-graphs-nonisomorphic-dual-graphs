// http://hackersdelight.org/hdcodetxt/snoob.c.txt

/* This version is from Hakmem (item 175) converted to C.
Caution: Do not call snoob with x = 0; it then does a
divide by 0. All the methods below which involve the
"ripple" variable fail if x is of the form 11...100...0
(1's followed by 0's).
   Snoob is basically seven ops, one of which is division. */

// ------------------------------ cut ----------------------------------
unsigned long long snoob(unsigned long long x) {
   unsigned long long smallest, ripple, ones;
                                // x = xxx0 1111 0000
   smallest = x & -x;           //     0000 0001 0000
   ripple = x + smallest;       //     xxx1 0000 0000
   ones = x ^ ripple;           //     0001 1111 0000
   ones = (ones >> 2)/smallest; //     0000 0000 0111
   return ripple | ones;        //     xxx1 0000 0111
}
// ---------------------------- end cut --------------------------------
