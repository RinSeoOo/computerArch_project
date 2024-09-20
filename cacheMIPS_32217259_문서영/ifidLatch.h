/* IF/ID latch */

struct ifidLatch{
    bool validate;  // next stage로 넘어가도 됨 
    int cpc;
    int npc;
    int instruction;
    int bidx;
};


void print_IFID(struct ifidLatch ifid);
