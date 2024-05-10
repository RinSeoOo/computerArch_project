#include <stdlib.h>
#include <stdio.h>

#include "struction.h"
#include "control.h"
// #define DEBUG_PC

// initalize value
#define MAX 0x400000

void print_instruct(struct inst in){
        printf("opcode: 0x%x, rs: %d, rt: %d, rd: %d, imm_s: 0x%x \n", in.opcode, in.rs, in.rt, in.rd, in.imm_s);
        printf("R[rs]: 0x%x, R[rt]: 0x%x, R[rd]: 0x%x\n", in.rs, in.rt, in.rd);
        printf("funct: 0x%x, shamt : 0x%x\n", in.func, in.shamt);
}


int mem[0x400000];
long int reg[32] = { 0 };
int pc = 0;
int instruction = 0;


int opzero(int pc_, int funct, int rs, int rt, int rd, int shamt, int imm_s){
        int zeroimm = 0xffff & imm_s;
        int altresu = 0;
        unsigned int altresu_ = 0;
        switch(funct){
                //add
                case 0x20:
                        altresu = reg[rs] + reg[rt];
#ifdef DEBUG_PC
                        printf("0x0 add(0x20) : r[%d] = r[%d] (0x%lx) + r[%d] (0x%lx) = 0x%x \n", rt, rs, reg[rs], rt, reg[rt], altresu);
#endif
                        reg[rd] = altresu;
                        pc_ += 4;
                        break;
                //addu
                case 0x21:
                        altresu_ = reg[rs] + reg[rt];
#ifdef DEBUG_PC
                        printf("0x0 addu(0x20) : r[%d] = r[%d] (0x%lx) + r[%d] (0x%lx) = 0x%x \n", rd, rs, reg[rs], rt, reg[rt], altresu_);
#endif                        
                        reg[rd] = altresu_;
                        pc_ += 4;
                        break;
                //sub
                case 0x22:
                        altresu = reg[rs] - reg[rt];
#ifdef DEBUG_PC
                        printf("0x0 sub(0x22) : r[%d] = r[%d] (0x%lx) - r[%d] (0x%lx) = 0x%x\n", rd, rs, reg[rs], rt, reg[rt], altresu);
#endif
                        reg[rd] = altresu;
                        pc_ += 4;
                        break;
                //subu
                case 0x23:
                        altresu_ = reg[rs] - reg[rt];
#ifdef DEBUG_PC                        
printf("0x0 subu(0x23) : r[%d] = r[%d] (0x%lx) - r[%d] (0x%lx) = 0x%x\n", rd, rs, reg[rs], rt, reg[rt], altresu_);
#endif
                        reg[rd] = altresu_;
                        pc_ += 4;
                        break;
                //and
                case 0x24:
                        reg[rd] = reg[rs] & reg[rt];
#ifdef DEBUG_PC                        
                        printf("0x0 and(0x24) : r[%d] = r[%d] (0x%lx) & r[%d] (0x%lx) = 0x%lx\n", rd, rs, reg[rs], rt, reg[rt], reg[rd]);
#endif
                        pc_ += 4;
                        break;
                //andi
                case 0xc:
                        reg[rd] = reg[rs] & zeroimm;
#ifdef DEBUG_PC                        
                        printf("0x0 andi(0xc) : r[%d] = r[%d] (0x%lx) & 0x%x = 0x%lx\n", rd, rs, reg[rs], zeroimm, reg[rd]);
#endif
                        pc_ += 4;
                        break;
                //or
                case 0x25:
                        reg[rd] = reg[rs] | reg[rt];
#ifdef DEBUG_PC                        
                        printf("0x0 or(0x25) : r[%d] = r[%d] (0x%lx) | r[%d] (0x%lx) = 0x%lx\n", rd, rs, reg[rs], rt, reg[rt], reg[rd]);
#endif
                        pc_ += 4;
                        break;
                //nor
                case 0x27:
                        reg[rd] = ~(reg[rs] | reg[rt]);
#ifdef DEBUG_PC                        
                        printf("0x0 nor(0x27) : r[%d] = ~ (r[%d] (0x%lx) | r[%d] (0x%lx)) = 0x%lx\n", rd, rs, reg[rs], rt, reg[rt], reg[rd]);
#endif
                        pc_ += 4;
                        break;
                //sll
                case 0x0:
                        reg[rd] = reg[rt] << shamt;
#ifdef DEBUG_PC                        
                        printf("0x0 sll(0x0) : r[%d] = r[%d] (0x%lx) << %d = 0x%lu\n", rd, rt, reg[rt], shamt, reg[rd]);
#endif
                        pc_ += 4;
                        break;
                //srl
                case 0x2:
                        reg[rd] = reg[rt] >> shamt;
#ifdef DEBUG_PC                        
                        printf("0x0 srl(0x2) : r[%d] = r[%d] (0x%lx) >> %d = 0x%lu\n", rd, rs, reg[rs], shamt, reg[rd]);
#endif
                        pc_ += 4;
                        break;

                //slt
                case 0x2a:
                        altresu = (reg[rs] < reg[rt]) ? 1 : 0;
#ifdef DEBUG_PC                        
                        printf("0x0 slt(0x2a) : r[%d] = (r[%d] 0x%lx < r[%d] 0x%lx) ? 1 : 0 = 0x%x\n", rd, rs, reg[rs], rt, reg[rt], altresu);
#endif
                        reg[rd] = altresu;
                        pc_ += 4;
                        break;
                //sltu
                case 0x2b:
                        altresu_ = (reg[rs] < reg[rt]) ? 1 : 0;
#ifdef DEBUG_PC                        
                        printf("0x0 sltu(0x2b) : r[%d] = (r[%d] 0x%lx < r[%d] 0x%lx) ? 1 : 0 = 0x%x\n", rd, rs, reg[rs], rt, reg[rt], altresu_);
#endif
                        reg[rd] = altresu_;
                        pc_ += 4;
                        break;

                //jr
                case 0x08:
                        pc_ = reg[rs];
#ifdef DEBUG_PC                        
                        printf("0x0 jr(0x8) : PC = r[%d] (0x%x) \n", rs, pc_);
#endif
                        break;
                default:
#ifdef DEBUG_PC                        
                        printf("nothing\n");
#endif
                        break;
        }
        return pc_;
}



int main(int argc, char*argv[]){
        FILE* fp;

        // file open
/*
        if(argc != 2){
                fprintf(stderr, "Usage %s <file>\n", argv[0]);
                exit(EXIT_FAILURE);
        }
*/

        fp = fopen(argv[1], "rb");
        // fp = fopen("input4.bin", "rb");
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
                //printf("pc: 0x%x, mem[0x%x]: 0x%x\n", pc, pc/4, mem[pc/4]);
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
        //      print_instruct(i_);
#endif


                //pc += 4;
                int addr = 0;
                int rtype = 0, itype = 0, jtype = 0;
                int zeroimm = 0xffff & imm_s;
                unsigned int AluResult = 0;
                switch(opcode){
                    
                        case 0x0:
                                cont_.regdest = 0;
                                pc = opzero(pc, funct, rs, rt, rd, shamt, imm_s);
                                rtype++;
                                break;
                        // addiu
                        case 0x9:
                                AluResult = reg[rs] + imm_s;
#ifdef DEBUG_PC                        
                                printf("0x9 addiu : r[%d] (0x%lx) + 0x%x = r[%d] (0x%x)\n", rs, reg[rs], imm_s, rt, AluResult);
#endif
                                reg[rt] = AluResult;
                                pc += 4;
                                break;
                        // sw (store word)
                        case 0x2b:
                                addr = reg[rs] + imm_s;
                                mem[addr/4] = reg[rt];
#ifdef DEBUG_PC                        
                                printf("0x%x sw : mem[r[%d] (0x%lx) +0x%x] = mem[0x%x] = r[%d] (0x%x)\n", opcode, rs, reg[rs],  imm_s, addr, rt, mem[addr/4]);
#endif
                                pc += 4;
                                break;
                        case 0x23:
                                addr = reg[rs] + imm_s;
                                reg[rt] = mem[addr/4];
#ifdef DEBUG_PC                        
                                printf("0x23 lw : r[%d] = mem[r[%d] (0x%lx) + 0x%x] = mem[0x%x] = 0x%lx\n", rt, rs, reg[rs], imm_s, addr, reg[rt]);
#endif
                                pc += 4;
                                break;
                        case 0xf:
                                reg[rt] = imm_s << 16;
#ifdef DEBUG_PC                        
                                printf("0xf lui : r[%d] = 0x%x << 16 = 0x%lx\n", rt, imm_s, reg[rt]);
#endif
                                pc += 4;
                                break;
                        case 0xd:
                                reg[rt] = reg[rs] | zeroimm;
#ifdef DEBUG_PC                        
                                printf("0xd ori : r[%d] = r[%d] (0x%lx) | 0x%x = 0x%lx\n", rt, rs, reg[rs], zeroimm, reg[rt]);
#endif
                                pc += 4;
                                break;


                        case 0x2:
                                addr = inst & 0x3ffffff;
                                pc = addr*4;
                                //printf("addr: 0x%x, 4*addr: 0x%x\n", addr, addr*4);
#ifdef DEBUG_PC                        
                                printf("0x2 j : pc = 0x%x \n", pc);
#endif
                                break;
                        case 0x3:
                                addr = inst & 0x3ffffff;
                                reg[31] = pc + 8;
                                pc = addr*4;
#ifdef DEBUG_PC                        
                                printf("0x3 jal : pc = 0x%x, r[31] = 0x%lx\n", pc, reg[31]);
#endif
                                break;
                        case 0x5:
                                if(reg[rs] != reg[rt]){
                                        pc = pc + 4 + imm_s*4;
#ifdef DEBUG_PC                        
                                        printf("0x5 bne : r[%d] (0x%lx) != r[%d] (0x%lx) --> pc = pc + 4 + 0x%x = 0x%x\n", rs, reg[rs], rt, reg[rt], imm_s*4, pc);
#endif
                                }
                                else {
                                        pc = pc + 4;
#ifdef DEBUG_PC                        
                                        printf("0x5 bne : r[%d] (0x%lx) == r[%d] (0x%lx) --> pc = pc + 4 = 0x%x\n", rs, reg[rs], rt, reg[rt], pc);
#endif
                                }
                                break;
                        case 0x4:
                                if(reg[rs] == reg[rt]){
                                        pc = pc + 4 + imm_s*4;
#ifdef DEBUG_PC                        
                                        printf("0x4 beq : r[%d] (0x%lx) -= r[%d] (0x%lx) --> pc = pc + 4 + 0x%x = 0x%x\n", rs, reg[rs], rt, reg[rt], imm_s*4, pc);
#endif
                                }
                                else {
                                        pc = pc + 4;
#ifdef DEBUG_PC                        
                                        printf("0x4 beq : r[%d] (0x%lx) != r[%d] (0x%lx) --> pc = pc + 4 = 0x%x\n", rs, reg[rs], rt, reg[rt], pc);
#endif
                                }
                                break;



                        case 0xa:
                                addr = ((reg[rs] < imm_s) ? 1 : 0);
#ifdef DEBUG_PC                        
                                printf("0xa slti : r[%d] = (r[%d] 0x%lx < 0x%x) ? 1 : 0 = 0x%x\n", rt, rs, reg[rs], imm_s, addr);
#endif
                                reg[rt] = addr;
                                pc += 4;
                                break;
                        case 0xb:
                                addr = ((reg[rs] < imm_s) ? 1 : 0);
#ifdef DEBUG_PC                        
                                printf("0xa sltiu : r[%d] = (r[%d] 0x%lx < 0x%x) ? 1 : 0 = 0x%x\n", rt, rs, reg[rs], imm_s, addr);
#endif
                                reg[rt] = addr;
                                pc += 4;
                                break;


                        default:
                                pc += 4;
                                break;
                }


        /*
                 * pc 항상 4씩 증가
                 * 그다음 메모리 접근 (메모리 읽기)
                 * 레지스터에 값 전달
                 * ALU 연산
        */
                // printf("\n");
                instruction++;
        }


        printf("----------------------------------------\n");
        printf("return value(value in r2) : %ld(0x%lx)\n", reg[2], reg[2]);
        printf("number of executed instruction : %d\n", instruction);
        printf("----------------------------------------\n");
        return 0;
}
