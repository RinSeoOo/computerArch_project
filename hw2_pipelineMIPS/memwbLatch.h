/* MEM/WB latch */

struct memwbLatch
{
    int currentPC;
    int nxtPC;
    int mData;
    long int wReg;

};

void print_MEMWB(struct memwbLatch memwb);