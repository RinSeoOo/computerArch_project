#define _GNU_SOURCE
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 100

// register global변수로 할당
int reg[32] = { 0 };

/*
 * ---------------------- PROBLEM ------------------------
 * 10 registers
 * basic arithmetic binary operations -> +, -, *, /
 * result = R0 register
 * M -> Move operation
 * store in input.txt
 *
 * prints out display
 * -------------------------------------------------------
 */

/*
 * ------------------- INPUT EXAMPLE --------------------
        input     output
 * + 0x3 0x2 -> R0: 5 = 3+2
 * - 0x1 0x2 -> R0: -1 = 1-2
 * M R2 0x3 -> R2: 3
 * + R0 R2 -> R0: 2 = -1+3
 * -------------------------------------------------------
 */

/*
 * EXTRA PROBLEM
 * 1. C: if same op, result is 0, if op1 > op2, result is +1, if op1 < op2, result is -1
 * 2. J:
 * 3. BEQ
 * 4. GCD -> make program GCD 15, 96
 */
void Get_Operation(char*, char*, char*);


// 값 입력 받기
void Get_Operation(char* opCode, char* op1, char* op2) {
        int regNum1 = 0, regNum2 = 0;   // store register number
        int num1 = 0, num2 = 0;
        long int result = 0;

        if (*op1 == '0') {
                num1 = strtol(op1, NULL, 16);
                //printf("num1= %d", num1);
                if (*op2 == '0') {
                        num2 = strtol(op2, NULL, 16);
                        //printf(", num2= %d\n", num2);
                }
                else {
                        //regNum2 = strtol(op2, NULL, 10);
                        regNum2 = strtol(&(*(op2 + 1)), NULL, 10);
                        //printf(", regNum2= %d(r[%d]=%d)\n",  regNum2, regNum2, reg[regNum2]);
                }
        }
        else {
                //regNum1 = atoi(op1);
                //regNum1 = strtol(op1, NULL, 10);
                regNum1 = strtol(&(*(op1 + 1)), NULL, 10);
                //printf("regNum1= %d(r[%d]=%d), ", regNum1, regNum1, reg[regNum1]);
                if (*op2 == '0') {
                        num2 = strtol(op2, NULL, 16);
                        //printf("num2= %d\n", num2);
                }
                else {
                        regNum2 = strtol(&(*(op2 + 1)), NULL, 10);
                        //printf("regNum2= %d(r[%d]=%d)\n",  regNum2, regNum2, reg[regNum2]);
                }
        }

        //printf("num1 = %d, num2 = %d, regNum1 = %d, regNum2 = %d\n", num1, num2, regNum1, regNum2); // check the value


        switch (*opCode) {
                case '+':
                        if (*op1 == 'R') {
                                if (*op2 == 'R') {
                                        *(reg + regNum1) += *(reg + regNum2);
                                        printf("R[%d]: %d = %s + %s\n", regNum1, reg[regNum1], op1, op2);
                                }
                                else {
                                        *(reg + regNum1) += num2;
                                        printf("R[%d]: %d = %s + %s", regNum1, reg[regNum1], op1, op2);
                                }
                        }
                        else {
                                if(*op2 == 'R'){
                                        reg[0] = num1 + *(reg + regNum2);
                                        printf("R[%d]: %d = %s + %s\n", regNum1, reg[regNum1], op1, op2);
                                }
                                else {
                                        reg[0] = num1 + num2;
                                        printf("R[%d]: %d = %s + %s\n", regNum1, reg[regNum1], op1, op2);
                                }
                        }
                        break;

                case '-':
                        if (*op1 == 'R') {
                                if (*op2 == 'R') {
                                        *(reg + regNum1) -= *(reg + regNum2);
                                        printf("R[%d]: %d = %s - %s\n", regNum1, reg[regNum1], op1, op2);
                                }
                                else {
                                        *(reg + regNum1) -= num2;
                                        printf("R[%d]: %d = %s - %s\n", regNum1, reg[regNum1], op1, op2);
                                }
                        }
                        else {
                                if (*op2 == 'R') {
                                        reg[0] = num1 - *(reg + regNum2);
                                        printf("R[%d]: %d = %s - %s\n", regNum1, reg[regNum1], op1, op2);
                                }
                                else {
                                        reg[0] = num1 - num2;
                                        printf("R[%d]: %d = %s - %s\n", regNum1, reg[regNum1], op1, op2);
                                }
                        }
                        break;

                case '/':
                        if (*op1 == 'R') {
                                if (*op2 == 'R') {
                                        *(reg + regNum1) /= *(reg + regNum2);
                                        printf("R[%d]: %d = %s / %s\n", regNum1, reg[regNum1], op1, op2);
                                }
                                else {
                                        *(reg + regNum1) /= num2;
                                        printf("R[%d]: %d = %s / %s\n", regNum1, reg[regNum1], op1, op2);
                                }
                        }
                        else {
                                if (*op2 == 'R') {
                                        reg[0] = num1 / *(reg + regNum2);
                                        printf("R[%d]: %d = %s / %s\n", regNum1, reg[regNum1], op1, op2);
                                }
                                else {
                                        reg[0] = num1 / num2;
                                        printf("R[%d]: %d = %s / %s\n", regNum1, reg[regNum1], op1, op2);
                                }
                        }
                        break;

                case '*':
                        if (*op1 == 'R') {
                                if (*op2 == 'R') {
                                        *(reg + regNum1) *= *(reg + regNum2);
                                        printf("R[%d]: %d = %s * %s\n", regNum1, reg[regNum1], op1, op2);
                                }
                                else {
                                        *(reg + regNum1) *= num2;
                                        printf("R[%d]: %d = %s * %s\n", regNum1, reg[regNum1], op1, op2);
                                }
                        }
                        else {
                                if (*op2 == 'R') {
                                        reg[0] = num1 * *(reg + regNum2);
                                        printf("R[%d]: %d = %s * %s\n", regNum1, reg[regNum1], op1, op2);
                                }
                                else {
                                reg[0] = num1 * num2;
                                printf("R[%d]: %d = %s * %s\n", regNum1, reg[regNum1], op1, op2);
                                }
                        }
                        break;

                case 'M':
                        int tmp2 = reg[regNum2];
                        int tmp1 = reg[regNum1];
                        printf("R[%d] = %d, R[%d] = %d \n", regNum2, reg[regNum2], regNum1, reg[regNum1]);
                        reg[regNum1] = tmp2;
                        reg[regNum2] = 0;
                        //printf("op1 = r%d, op2 = r%d\n", regNum1, regNum2);
                        //printf("R[%d] = %d, R[%d] = %d \n", regNum2, reg[regNum2], regNum1, reg[regNum1]);
                        printf("R[%d] -> R[%d] = %d \n\n", regNum2, regNum1, reg[regNum1]);
                        break; //strtok_r
                case 'C':
                        // compare op1, op2
                        int com1 = 0, com2 = 0;
                        if (*op1 == 'R') {
                                if (*op2 == 'R') {
                                        com1 = *(reg + regNum1);
                                        com2 = *(reg + regNum2);
                                }
                                else {
                                        com1 = *(reg + regNum1);
                                        com2 = num2;
                                }
                        }
                        else {
                                if (*op2 == 'R') {
                                        com1 = num1;
                                        com2 = *(reg + regNum2);
                                }
                                else {
                                        com1 = num1;
                                        com2 = num2;
                                }
                        }

                        // put the number in R0
                        if(com1 > com2) {
                                reg[0] = 1;
                                printf("op1 > op2 -> r[0] = 1\n\n");
                        }
                        else if(com1 < com2) {
                                reg[0] = -1;
                                printf("op1 < op2 -> r[0] = -1\n\n");
                        }
                        else {
                                reg[0] = 0;
                                printf("op1 == op2 -> r[0] = 0\n\n");
                        }

                        break;
        }


        //Get_Operation();
        //if (*opCode == 'H') return;
        return;
}


int main(int argc, char *argv[]) {

        FILE* stream;
        char* line = NULL;
        size_t len = 0;
        ssize_t nread;
        char op;
        int lineNumber = 0;

        if (argc != 2) {
                fprintf(stderr, "Usage: %s <file>\n", argv[0]);
                exit(EXIT_FAILURE);
        }
        stream = fopen(argv[1], "r");   // warning C4047: '=': 'FILE *'의 간접 참조 수준이 'errno_t'과(와) 다릅니다.
        if (stream == NULL) {
                perror("fopen");
                exit(EXIT_FAILURE);
        }

        //readFromFile(stream);




        while ((nread = getline(&line, &len, stream)) != -1){
                fwrite(line, nread, 1, stdout);


                //strok for parase input line

                //char getOP[20] = { 0 };
                char* opCode = NULL, * op1 = NULL, * op2 = NULL;// 부호, 실제 계산 숫자/reg 저장(2개)

                char* temp = NULL;
                opCode = strtok_r(line, " ", &temp); //공백을 기준으로 문자열 자르기
                if(*opCode == 'H') {
                        printf("Program is done\n");
                        break;
                }                       //opCode = strtok_r(getOP, " ", &temp); //공백을 기준으로 문자열 자르기
                op1 = strtok_r(temp, " ", &temp);       // operand1
                op2 = strtok_r(temp, " ", &temp);       // operand2

                //printf("opCode: %s, op1: %s, op2: %s", opCode, op1, op2);
                Get_Operation(opCode, op1, op2);
        }

        //Get_Operation();

        free(line);
        fclose(stream);

        //GetFile();
        //Get_Operation();

        return 0;
}
