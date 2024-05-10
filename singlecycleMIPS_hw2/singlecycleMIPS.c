/*HW2_single cycle MIPS project*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "struction.h"
#include "control.h"
#include "register.h"
#include "aluInst.h"
#include "memory.h"
// #define DEBUG_PC

// initalize value
#define MAX 0x400000

void print_instruct(struct inst in){
        printf("opcode: 0x%x, rs: %d, rt: %d, rd: %d, imm_s: 0x%x \n", in.opcode, in.rs, in.rt, in.rd, in.imm_s);
        printf("R[rs]: 0x%x, R[rt]: 0x%x, R[rd]: 0x%x\n", in.rs, in.rt, in.rd);
        printf("funct: 0x%x, shamt : 0x%x\n", in.func, in.shamt);
}

void print_control(struct control con){
        printf("regdest: %d, pcsrc1: %d, pcsrc2: %d, memread: %d, memtoreg: %d\n", con.regdest, con.pcsrc1, con.pcsrc2, con.memread, con.memtoreg);
        printf("aluop: %d, memwrite: %d, alusrc: %d, regwrite: %d\n", con.aluop, con.memwrite, con.alusrc, con.regwrite);
}

void print_regist(struct regist re){
        printf("readreg1: %d, readreg2: %d, writerreg: %d, writerdata: 0x%x\n", re.readreg1, re.readreg2, re.writereg, re.writedata);
        printf("rddata1: 0x%x, rddata2: 0x%x\n", re.rddata1, re.rddata2);
}

void print_alu(struct alu alu){
        printf("rddata1: 0x%x, rddata2: 0x%x, aluresult: 0x%x\n", alu.rddata1, alu.rddata2, alu.aluresult);
}

void print_memory(struct memor me){
        printf("address: 0x%x, wirtedataMem: 0x%x, readdataMem: 0x%x\n", me.address, me.writedataMem, me.readdataMem);
}


int mem[0x400000];
long int reg[32] = { 0 };
int pc = 0;
int rtype = 0, itype = 0, jtype = 0, instruction = 0;
int memoryaccess = 0, branch = 0;


struct instmem{
        int readaddr;
        int instruction;
};

// 메모리 읽기 함수
int read_memory(int readdataMem, int r, int imm_s) {
    if (readdataMem = 1) {
        // 메모리 읽기
        return mem[(reg[r] + imm_s)/4];/* 메모리 값 */
    } else {
        // 메모리 읽기 비활성화 상태일 때의 처리
        return 0; // 또는 다른 값 또는 오류 처리
    }
}

void controlunit(int opcode, struct control *cont_, int funct, int rs, int rt){
         int addr = 0;
                if(opcode == 0)
                        cont_->regdest = 1; // r-type
                else cont_->regdest = 0;
                if(opcode != 0x0 && opcode != 0x4 && opcode != 0x5)
                        cont_->alusrc = 1; // r-type, beq, bne 빼고 다 
                else cont_->alusrc = 0;
                if(opcode == 0x23)
                        cont_->memtoreg = 1; // lw
                else cont_->memtoreg = 0;
                if(opcode != 0x2b && opcode != 0x4 && opcode != 0x5 && opcode != 0x2 && !(opcode == 0 && funct == 0x8) && !(opcode == 0 && funct == 9))
                        cont_->regwrite = 1; // sw, Bxx, j, jr 빼고 다  ( + jalr 추가)
                else cont_->regwrite = 0;
                if(opcode == 0x23) // lw
                        cont_->memread = 1;
                else cont_->memread = 0;
                if(opcode == 0x2b) // sw
                        cont_->memwrite = 1;
                else cont_->memwrite = 0;
                if(opcode == 0x2 || opcode == 0x3) // j, jal
                        cont_->pcsrc1 = 1;
                else cont_->pcsrc1 = 0;
                if((opcode == 0x4 && (reg[rs] == reg[rt])) || (opcode == 0x5 && (reg[rs] != reg[rt])))
                        cont_->pcsrc2 = 1; // bxx operate(만족시)
                else cont_->pcsrc2 = 0;
                
                if(opcode == 0x0) cont_->aluop = 1;
                else if(opcode == 0x2 || opcode == 0x3) cont_->aluop = 0;
                else if(opcode == 0x23 || opcode == 0x2b) cont_->aluop = 2;
                else if (opcode == 0x4 || opcode == 0x5) cont_->aluop = 3;
                else {
                        cont_->aluop = 4; // I-type
                        itype++;
                }
}

// r-type(sw, lw, bxx 제외)
int alu(int opcode, struct regist *regist_, struct alu *alu_){
        int zeroimm = 0xffff & alu_->rddata2;
        unsigned int Alures_;
        signed int Alures;
        switch(opcode){
                /* rt <- rs, imm_s */
                case 0xc:  //andi
                        Alures = alu_->rddata1 & zeroimm;
                        // printf("0x0 andi(0xc)\n");
                        regist_->writedata = Alures;
                        break;
                case 0x9: // addiu
                        Alures_ = alu_->rddata1 + alu_->rddata2;
                        //reg[rs] + imm_s;
                        // printf("0x9 addiu : 0x%x + 0x%x = 0x%x\n", alu_->rddata1, alu_->rddata2, Alures_);
                        regist_->writedata = Alures_;
                        break;

                case 0xa: // slti
                        Alures_ = (signed int)((alu_->rddata1 < alu_->rddata2) ? 1 : 0);
                        // printf("0xa slti  \n");
                        regist_->writedata = Alures_;
                        break;
                case 0xb: // sltiu
                        Alures = (unsigned int)((alu_->rddata1 < alu_->rddata2) ? 1 : 0);
                        // printf("0xa sltiu\n");
                        regist_->writedata = Alures;
                        break;


                /* rt <- imm */
                case 0xf: // lui
                        regist_->writedata = alu_->rddata2 << 16;
                        // printf("0xf lui \n");
                        break;
                /* rt <- imm */
                case 0xd: // ori
                        regist_->writedata = alu_->rddata1 | zeroimm;
                        // printf("0xd ori\n");
                        break;
                case 0x8:
                        // printf("jr\n");
                        printf("\n");
                        break;

                 // sw & lw  : 다른 곳에 구현함
                case 0x2b:
                case 0x23:
                        break;

                // Bxx operate: rs VS rt -> pc = pc + 4 + imm_s * 4;
                // 이미 다른 곳에 구현 완료
                case 0x4:
                case 0x5:
                        // printf("bne & beq\n");
                        break;

                default:
                        // printf("nothing\n");
                        break;
        }
        return regist_->writedata;
}


int opzero(int funct, struct regist *regist_, int shamt){
        int altresu = 0;
        unsigned int altresu_ = 0;
        switch(funct){
                /* rd <- rs, rt */
                case 0x20:  //add
                        altresu = regist_->rddata1 + regist_->rddata2;
                        // printf("0x0 add(0x20) \n");
                        reg[regist_->writereg] = altresu;
                        break;
             
                case 0x21:   //addu
                        altresu_ = regist_->rddata1 + regist_->rddata2;
                        // printf("0x0 addu(0x20) %d + %d = %u\n", regist_->rddata1, regist_->rddata2, altresu_);
                        reg[regist_->writereg] = altresu_;
                        break;
                        
                case 0x22:  //sub
                        altresu = regist_->rddata1 - regist_->rddata2;
                        // printf("0x0 sub(0x22)\n");
                        reg[regist_->writereg] = altresu;
                        break;
              
                case 0x23:  //subu
                        altresu_ = regist_->rddata1 - regist_->rddata2;
                        // printf("0x0 subu(0x23)\n");
                        reg[regist_->writereg] = altresu_;
                        break;
               
                case 0x24: //and
                        reg[regist_->writereg] = regist_->rddata1 & regist_->rddata2;
                        // printf("0x0 and(0x24)\n");
                        break;
               
                case 0x25: //or
                        reg[regist_->writereg] = regist_->rddata1 | regist_->rddata2;
                        // printf("0x0 or(0x25)\n");
                        break;

                case 0x27: //nor
                        reg[regist_->writereg] = ~(regist_->rddata1 | regist_->rddata2);
                        // printf("0x0 nor(0x27)\n");
                        break;


                case 0x2a: //slt
                        altresu = (regist_->rddata1 < regist_->rddata2) ? 1 : 0;
                        // printf("0x0 slt(0x2a)\n");
                        reg[regist_->writereg] = altresu;
                        break;
                
                case 0x2b: //sltu
                        altresu_ = (regist_->rddata1 < regist_->rddata2) ? 1 : 0;
                        // printf("0x0 sltu(0x2b)\n");
                        reg[regist_->writereg] = altresu_;
                        break;
                
                /* rd & rt & shamt */
                case 0x0: //sll
                        reg[regist_->writereg] = regist_->rddata2 << shamt;
                        // printf("0x0 sll(0x0)\n");
                        break;
              
                case 0x2:  //srl
                        reg[regist_->writereg] = regist_->rddata2 >> shamt;
                        // printf("0x0 srl(0x2)\n");
                        break;

                /* jr : pc & rs */
                case 0x08: //jr   
                        // printf("0x0 jr(0x8) pc = 0x%x \n", pc);
                        break;
                case 0x9: // jalr
                        altresu = pc;
                        /*
                        rd = reg[31] ($ra에 저장될 주소 받을 reg) - 되돌아올 주소
                        rs = Jumpaddr(점프할 대상 주소)
                        -> address = reg[rs]
                        -> reg[rd] = address;
                        r
                        */
                        break;
                
                default:
                        // printf("nothing\n");
                        break;
        }

        return reg[regist_->writereg];
}


int main(int argc, char*argv[]){
        FILE* fp;
        // file open
        if(argc != 2){
                fprintf(stderr, "Usage %s <file>\n", argv[0]);
                exit(EXIT_FAILURE);
        }


        fp = fopen(argv[1], "rb");
        //fp = fopen("input4.bin", "rb");
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

                //printf("var: 0x%x ", var);
                unsigned int bin1, bin2, bin3, bin4;
                bin1 = var & 0x000000ff;
                bin2 = var & 0xff00;
                bin3 = var & 0x00ff0000;
                bin4 = (var >> 24) & 0xff;
                int resu = (bin1 << 24) | (bin2 << 8) | (bin3 >> 8) | bin4;
                mem[i] = resu;
                //printf(" mem[0x%x]: 0x%x\n", i, mem[i]);
                i++;
        }
        fclose(fp);
        // 완료!!!

        // MIPS var formats
        reg[29] = 0x1000000;
        reg[31] = 0xffffffff;
        int opcode;
        int rs, rd, rt;
        int shamt, funct, imm, imm_s;
        int addr;
        struct inst i_;
        struct control cont_;

        

        while(1){
#ifdef DEBUG_PC
                printf("pc: 0x%x, mem[0x%x]: 0x%x\n", pc, pc/4, mem[pc/4]);
#endif
                // terminal condition check
                if(pc == 0xffffffff) break;

                // Instruction Fetch
                int inst = mem[pc/4];
                // Instruction Decode
                opcode = (inst >> 26) & 0x3f;
                rs = (inst >> 21) & 0x1f;
                rt = (inst >> 16) & 0x1f;
                rd = (inst >> 11) & 0x1f; // && -> &
                shamt = (inst >> 6) & 0xf;
                funct = inst & 0x3f;
                imm = inst & 0xffff;
                imm_s = (imm & 0x8000) ? (imm | 0xffff0000) : imm;

                i_.opcode = opcode;
                i_.rs = rs;
                i_.rt = rt;
                i_.rd = rd;
                i_.shamt = shamt;
                i_.func = funct;
                i_.imm = imm;
                i_.imm_s = imm_s;

#ifdef DEBUG_PC
             print_instruct(i_);
#endif

                controlunit(opcode, &cont_, funct, rs, rt);

#ifdef DEBUG_PC       
                printf("[ncontrol]\n");
                print_control(cont_);
#endif

                struct instmem instmem_;
                struct regist regist_;
                struct alu alu_;
                struct memor memor_;

                /*---------- register ----------*/
                regist_.readreg1 = rs;
                regist_.readreg2 = rt; // r-type일 때

                //regist_.rddata1 = reg[regist_.readreg1]; // rs
        
                regist_.rddata1 = reg[regist_.readreg1]; //reg[rs];
                regist_.rddata2 = reg[regist_.readreg2]; //reg[rt];
                
                
                // regdest: rt, rd 선택 
                if(cont_.regdest == 1){ // r-type -> rd
                        regist_.writereg = rd;
                        //opzero(pc, funct, rs, rt, rd, shamt, imm_s);
                        rtype++;
                }
                else {  // I-type & J-type -> rt
                        regist_.writereg = rt;
                        regist_.rddata2 = reg[regist_.writereg];
                }

                // if(cont_.regwrite == 1){
                //         reg[regist_.writereg] = regist_.writedata;
                // }

#ifdef DEBUG_PC
                printf("[register]\n");
                print_regist(regist_);
#endif

                /*---------- ALU ----------*/
                // alusrc: rddata2, imm_s 선택
                alu_.rddata1 = regist_.rddata1; // reg[rs]
                if(cont_.alusrc == 1){
                        // r-type, beq, bne 빼고 나머지
                        alu_.rddata2 = imm_s;
                        //imm_s = imm_s * 4;
                }else alu_.rddata2 = regist_.rddata2; //rt


                // alu control
                switch(cont_.aluop){
                        case 0: // j-type operation
                                break;
                        case 1:
                                alu_.aluresult = opzero(funct, &regist_, shamt);
                                break;
                        case 2: // sw & lw
                                alu_.aluresult = alu_.rddata1 + alu_.rddata2;
                                memoryaccess++;
                                // printf("sw/lw\n");
                                // mem[(reg[r] + imm_s)/4]
                                break;
                        case 3: // beq & bne: aluop: bxx일 때, imm_s*4 추가
                                // alu_.aluopdata = imm_s * 4;
                                alu_.aluresult = imm_s * 4;
                                branch++;
                                break;
                        case 4: // I-type
                                alu_.aluresult = alu(opcode, &regist_, &alu_);
                                reg[regist_.writereg] = alu_.aluresult; // >> write back
                                // printf("aluresult: 0x%x\n", reg[regist_.writereg]);
                                break;
                }
#ifdef DEBUG_PC
                printf("[alu]\n");
                print_alu(alu_);
#endif

                /* lw & sw */
                memor_.address = alu_.aluresult / 4;
                // memor_.writedataMem = regist_.rddata2; // rt

                /* control unit: memory */
                if(cont_.memwrite == 1){
                        // sw: mem[rs+imm] = rt
                        // printf("(rs)alu_.rddata1: 0x%x, regist_.rddata1: 0x%x\t", alu_.rddata1, regist_.rddata1);
                        memor_.writedataMem = regist_.rddata2; // rt > memorydata에 rt 값 저장
                        mem[memor_.address] = memor_.writedataMem; // memory addr에 rt로 받은 값 저장
                        // printf("sw: mem[0x%x] = %d\n", memor_.address, memor_.writedataMem);
                        itype++;
                }
                
                if(cont_.memread == 1){ // lw
                        memor_.readdataMem = mem[memor_.address]; // memory에서 address에 대한 mem을 읽음
                        // printf("lw (r[%d] = mem[0x%x + 0x%x] (0x%x)) \t ", rt, reg[rs] + imm_s, mem[(reg[rs] + imm_s)/4]);
                        // printf("lw -> addr = mem[0x%x] (0x%x) \t", memor_.address, memor_.readdataMem);
                        // reg[memor_.address] = mem[memor_.readdataMem];
                        //rt = mem[rs+imm]
                        itype++;
                }
                if(cont_.memtoreg == 1){ // >> write back
                        regist_.writedata = memor_.readdataMem; // memory -> register
                        reg[regist_.writereg] = regist_.writedata; // register에 writedata 저장
                        // printf("lw: reg[%d] = 0x%x \n", regist_.writereg, reg[regist_.writereg]);
                }
                else {
                        regist_.writedata = alu_.aluresult;
                }

#ifdef DEBUG_PC
                printf("[memory]\n");
                print_memory(memor_);
#endif


                /* control unit: pc update */
                if(cont_.pcsrc1 == 1){
                        // printf("j-type operation\t");
                        memor_.address = (inst & 0x3ffffff) * 4; // j-type: j, jal (JumpAddr)
                        if(opcode == 0x3) reg[31] = pc + 8;
                        pc = memor_.address; // j-type: j, jal
                        jtype++;
                }
                else {
                        if(cont_.pcsrc2 == 1)
                                // pc = pc + 4 + alu_.aluopdata; // Bxx operate
                                pc = pc + 4 + alu_.aluresult; // Bxx operate
                        else if(cont_.pcsrc2 != 1){
                                if(opcode == 0x0 && (funct == 0x8 || funct == 0x9)){
                                        // pc = reg[rs] - 4;
                                        pc = reg[regist_.readreg1] - 4;
                                        // printf("jal pc: 0x%x, r[%d]: 0x%lx\n", pc, regist_.readreg1, reg[regist_.readreg1]); // jr pc 바꾸기
                                }
                                pc = pc + 4;    // r-type
                        }
                        
                        
                }
                instruction++;

                // printf("return value(value in r2) : %ld(0x%lx)\n", reg[2], reg[2]);
                // printf("----------------------------------------\n\n");
                
        }
                printf("----------------------------------------\n");
                printf("return value(value in r2) : %ld(0x%lx)\n", reg[2], reg[2]);
                printf("number of executed instruction : %d\n", instruction);
                printf("number of (executed) R-type instruction : %d\n", rtype);
                printf("number of I-type instruction : %d\n", itype);
                printf("number of J-type instruction : %d\n", jtype);
                printf("number of memory access instruction : %d\n", memoryaccess);
                printf("number of taken branches : %d\n",  branch);
                printf("----------------------------------------\n");

        return 0;
}