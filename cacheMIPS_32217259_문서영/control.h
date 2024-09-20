

struct control{
        // Decode
        int regdest;    // op = 0 (r-type) -> rt(I-type) / rd(R-type) choice
        int regwrite;   // sw -> register write check

        // Execute
        bool use_rs;
        bool use_rt;
        int alusrc;     // op != 0, beq, bne -> regData / imm_s choice
        int aluop;      // op = ? -> alu control(r-type: opcode(zeroOp()) / i-type: opfunc() choice, lw: select addition, sw: select addition, bxx: bcond func, __:dont care)
        
        int pcsrc1;     // op = j / jal -> O (Jump)
        int pcsrc2;     // op = Bxx && b 만족 -> O (Branch)

        // Memory
        int memread;    // op = lw -> memory read check
        // r-type, i-type - 0 | lw -> 1 
        int memwrite;   // op = sw -> memory write check

        // Write Back
        int memtoreg;   // op = lw -> memoryData / aluresult choice
        // int regwrite;   // sw -> register write check
        // int pcsrc1;     // op = j / jal -> O (Jump)
        // int pcsrc2;     // op = Bxx && b 만족 -> O (Branch)

        
        // regData2 -> r-type, imm_s -> i-type
};

void print_control(struct control con);