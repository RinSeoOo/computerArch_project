#define _GNU_SOURCE
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
//#include "getline_header.h"

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
  * EXTRA PROBLEM
  * 1. C: if same op, result is 0, if op1 > op2, result is +1, if op1 < op2, result is -1
  * 2. J:
  * 3. BEQ
  * 4. GCD -> make program GCD 15, 96
  */
void Get_Operation(FILE*, char*, char*, char*);


void JumpOp(FILE* file, int cnt) {
    char* line = NULL;
    size_t len = 0;
    ssize_t nread;
    char op;
    int lineNumber = 0;
    fseek(file, 0, SEEK_SET); // go to starting point
    long position = ftell(file);
    //printf("Current file position: %ld\n", position);

    while ((nread = getline(&line, &len, file)) != -1) {
        ++lineNumber;
        if (cnt <= lineNumber) {
            fwrite(line, nread, 1, stdout);

            char* opCode = NULL, * op1 = NULL, * op2 = NULL;        // 부호, 실제 계산 숫자/reg 저장(2개)

            char* temp = NULL;
            opCode = strtok_r(line, " ", &temp); // 공백을 기준 으로 문자열 자르기

            if (*opCode == 'H') {
            printf("reg[1] is %d. Program is done\n", reg[1]);
            break;
            }
            op1 = strtok_r(temp, " ", &temp);       // operand1
            if (*opCode != 'J' || *opCode != 'B') {
                op2 = strtok_r(temp, " ", &temp);       // operand2
            }
            else {
                op2 = NULL;
            }
            lineNumber = 0;

            Get_Operation(file, opCode, op1, op2);
            //lineNumber = 0;
        }

    }
}

// 값 입력 받기
void Get_Operation(FILE* file, char* opCode, char* op1, char* op2) {
    int regNum1 = 0, regNum2 = 0;   // store register number
    int num1 = 0, num2 = 0;
    long int result = 0;

    // check op1, op2 (number OR register), and change
    if(*opCode == 'J' || *opCode == 'B'){
        num1 = strtol(op1, NULL, 10);
    }
    else if (*opCode != 'J' && *opCode != 'B'){
        if (op2 == NULL){
        num1 = strtol(op1, NULL, 16);
        }
        else {
            if (*op1 == '0') {
                num1 = strtol(op1, NULL, 16);
                if (*op2 == '0') {
                    num2 = strtol(op2, NULL, 16);
                    //printf(", num2= %d\n", num2);
                }
                else {
                    regNum2 = strtol(&(*(op2 + 1)), NULL, 10);
                    //printf(", regNum2= %d(r[%d]=%d)\n",  regNum2, regNum2, reg[regNum2]);
                }
            }
            else {
                //regNum1 = atoi(op1);
                //regNum1 = strtol(op1, NULL, 10);
                regNum1 = strtol(&(*(op1 + 1)), NULL, 10);
                if (*op2 == '0') {
                    num2 = strtol(op2, NULL, 16);
                }
                else {
                    regNum2 = strtol(&(*(op2 + 1)), NULL, 10);
                    }
                }
            }

    }


//    printf("num1: %d, num2: %d, regNum1: %d, regNum2: %d\n", num1, num2, regNum1, regNum2);


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
            if (*op2 == 'R') {
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
        printf("R[%d] -> R[%d] = %d \n\n", regNum2, regNum1, reg[regNum1]);
        break; //strtok_r


    case 'C':
        // compare register and put the number in R0
        if (*(reg + regNum1) > *(reg + regNum2)) {
            reg[0] = 1;
            printf("op1 > op2 -> r[0] = %d\n\n", reg[0]);
        }
        else if (*(reg + regNum1) < *(reg + regNum2)) {
            reg[0] = -1;
            printf("op1 < op2 -> r[0] = %d\n\n", reg[0]);
        }
        else {
            reg[0] = 0;
            printf("op1 = op2 -> r[0] = %d\n\n", reg[0]);
        }

        break;

    case 'J':
        printf("go to %d line \n", num1);
        JumpOp(file, num1);
        break;
    case 'B':
        printf("r[0] = %d\n", reg[0]);
        if(reg[0] == 0){
            printf("go to %d line\n", num1);
            JumpOp(file, num1);
        }
        //printf("r[0] is not zero\n");
        break;

    }


    return;
}


int main(int argc, char* argv[]) {

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
    stream = fopen(argv[1], "r");
    if (stream == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }



    while ((nread = getline(&line, &len, stream)) != -1) {
        fwrite(line, nread, 1, stdout);

        char* opCode = NULL, * op1 = NULL, * op2 = NULL;
        char* temp = NULL;

        opCode = strtok_r(line, " ", &temp);
        if (*opCode == 'H') {
            printf("reg[1] is %d. Program is done\n", reg[1]);
            break;
        }
        op1 = strtok_r(temp, " ", &temp);       // operand1
        if (*opCode != 'J' || *opCode != 'B') {
            op2 = strtok_r(temp, " ", &temp);       // operand2
        }
        else {
            op2 = NULL;
        }

        Get_Operation(stream, opCode, op1, op2);
    }


    free(line);
    fclose(stream);


    return 0;
}