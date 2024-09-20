/* branch prediction */

struct BTB{
    // int notfst;
    int pc;
    int btarget;
    int bValid;
};

void print_BTB(struct BTB btb);
