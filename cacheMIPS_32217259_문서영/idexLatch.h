/* ID/EX latch*/

struct idexLatch
{
    bool validate;  // next stage로 넘어가도 됨

    int npc;
    int cpc;

    int inst;

    int bidx;
    int rs;
    int rt;
    int rtData;
    int imm_s;
    int wReg;
    int funct; // R-type
    int shamt; // R-type
    int opcode; // 필요!!
};

void print_IDEX(struct idexLatch idex);
