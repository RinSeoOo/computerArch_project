#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// single mips structure
#include "struction.h"
#include "control.h"
#include "register.h"
#include "aluInst.h"
#include "memory.h"

// Latch structure
#include "ifidLatch.h"
#include "idexLatch.h"
#include "exmemLatch.h"
#include "memwbLatch.h"

#define DEBUG_PC
// 실행: ./test ../../../camp_test/test_prog/simple.bin

// fatch
void print_IFID(struct ifidLatch ifid){
    printf("pc: 0x%x, instruction: 0x%x\n", ifid.pc, ifid.instruction);
}

void print_IDEX(struct idexLatch idex){
    printf("pc: 0x%x, Jaddr: 0x%x, rData1: 0x%x, rData2: 0x%x, imm_s: 0x%x, wReg: 0x%lx\n", idex.pc, idex.jAddr, idex.rdata1, idex.rdata2, idex.imm_s, idex.wReg);
}

void print_EXMEM(struct exmemLatch exmem){
    printf("currentPC: 0x%x, nextPC: 0x%x, aluResult: 0x%x, rt: 0x%x, wReg: 0x%lx\n", exmem.currentPC, exmem.nxtPC, exmem.aluResult, exmem.rt, exmem.wReg);
}

void print_MEMWB(struct memwbLatch memwb){
    printf("currentPC: 0x%x, nextPC: 0x%x, memoryData: 0x%x, wReg: 0x%lx\n", memwb.currentPC, memwb.nxtPC, memwb.mData, memwb.wReg);
}

struct instmem{
        int readaddr;
        int instruction;
};

struct validate{
    bool valid;
    int regData;
    int teg;
};


int mem[0x400000];
int pc = 0;
long int reg[32] = { 0 };

int main(int argc, char*argv[]){
    FILE* fp;
    // file open
    if(argc != 2){
            fprintf(stderr, "Usage %s <file>\n", argv[0]);
            exit(EXIT_FAILURE);
    }

    fp = fopen(argv[1], "rb");
    if(fp == NULL){
            perror("fopen");
            exit(EXIT_FAILURE);
    }

    // file store in memory
    int var = 0;
    int i = 0;
    while(1){
            int ret = fread(&var, sizeof(var), 1, fp);
            if(ret == 0) break;

            unsigned int bin1, bin2, bin3, bin4;
            bin1 = var & 0x000000ff;
            bin2 = var & 0xff00;
            bin3 = var & 0x00ff0000;
            bin4 = (var >> 24) & 0xff;
            int resu = (bin1 << 24) | (bin2 << 8) | (bin3 >> 8) | bin4;
            mem[i] = resu;
            i++;
    }
    fclose(fp);

    reg[29] = 0x1000000;
    reg[31] = 0xffffffff;

    // include structure
    struct ifidLatch ifid[2];
    struct idexLatch idex[2];
    struct exmemLatch exmem[2];
    struct memwbLatch memwb[2];

    // 1. fetch
    ifid[0].pc = pc + 4;
    ifid[0].instruction = mem[pc/4];

#ifdef DEBUG_PC
    print_IFID(ifid[0]);
#endif

    // 2. decode
    ifid[1].pc = ifid[0].pc;
    ifid[1].instruction = ifid[0].instruction;

    int opcode = (inst >> 26) & 0x3f;
    long int rs = (inst >> 21) & 0x1f;
    long int rt = (inst >> 16) & 0x1f;
    long int rd = (inst >> 11) & 0x1f; // && -> &
    int shamt = (inst >> 6) & 0xf;
    int funct = inst & 0x3f;
    int imm = inst & 0xffff;
    int imm_s = (imm & 0x8000) ? (imm | 0xffff0000) : imm;

    
    

#ifdef DEBUG_PC
    print_IFID(ifid[1]);
    print_IDEX(idex[0]);
#endif

    // 3. execution

#ifdef DEBUG_PC
    print_IDEX(idex[1]);
    print_EXMEM(exmem[0]);
#endif

    // 4. write back

#ifdef DEBUG_PC
    print_EXMEM(exmem[1]);
    print_MEMWB(memwb[0]);
#endif

        
    return 0;
}