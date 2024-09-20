/* EX/MEM latch */

struct exmemLatch{
    bool validate;  // next stage로 넘어가도 됨
    bool memory;

    int cpc;
    int npc;

    int rs;
    int rt;

    int opcode;
    int aluResult;
    int rtData;
    int wReg;
    int wData;
    int bidx;

    int jAddr;
    int branchAddr;
    bool jr;
    int jrAddr;
};

void print_EXMEM(struct exmemLatch exmem);