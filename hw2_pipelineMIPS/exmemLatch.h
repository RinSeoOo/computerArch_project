/* EX/MEM latch */

struct exmemLatch{    
    int currentPC;
    int nxtPC;
    int aluResult;
    int rt;
    long int wReg;

};

void print_EXMEM(struct exmemLatch exmem);