struct control{
        int regdest;    // op = 0 (r-type)
        int pcsrc1;     // op = j / jal
        int pcsrc2;     // op = Bxx && b 만족
        int memread;    // op = lw
        int memtoreg;   // op = lw
        int aluop;      // op = 
        int memwrite;   // op = sw
        int alusrc;     // op != 0, beq, bne
        int regwrite;   // sw
};

void print_control(struct control con);