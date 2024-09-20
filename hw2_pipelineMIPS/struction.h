struct inst{
        int i;
        int opcode;
        int rs;
        int rd;
        int rt;
        int shamt;
        int func;
        int imm;
        int imm_s;
};

void print_instruct(struct inst in);
