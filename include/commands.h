#pragma once
#include "cvm.h"
void mov_reg_u64(u8 regNum, u64 num);
void mov_reg_reg(u8 regNum1, u8 regNum2);
void mov_reg_mem(u8 regNum, u64 memPos);
void mov_mem_u64(u64 memPos, u64 num);
void mov_mem_reg(u64 memPos, u8 regNum);
void push();
void pop();
void write();
void read();
void syscall();
void jmp(u64 pos);
void jmpz(u64 pos);
void jmpnz(u64 pos);
void cmp(u8 type);
void call(u64 pos);
void ret();