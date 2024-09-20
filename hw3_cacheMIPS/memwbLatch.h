/* MEM/WB latch */

struct memwbLatch
{
    bool validate;  // next stage로 넘어가도 됨

    bool jr;

    int jAddr;
    int branchAddr;
    int jrAddr;

    int cpc;
    int npc;

    int opcode;
    int mData;
    int aluResult;
    int wReg;
    int bidx;

};


void print_MEMWB(struct memwbLatch memwb);