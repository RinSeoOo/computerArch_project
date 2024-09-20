/* ID/EX latch*/

struct idexLatch
{  
    int jAddr;
    int pc;
    int rdata1;
    int rdata2;
    int imm_s;
    long int wReg;

};

void print_IDEX(struct idexLatch idex);
