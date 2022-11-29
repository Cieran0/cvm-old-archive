#pragma once
#include <iostream>
#include <stack>

#define MEM_SIZE 134217728

typedef unsigned long u64;
typedef unsigned char u8;
typedef char s8;
typedef unsigned char instruction;
extern u64* MAIN_MEM;
extern instruction* IP;
extern std::stack<u64> Stack;
extern std::stack<u64> returnStack;

extern bool end;
extern u8 program[]; 
extern u8 program1[];

extern u64 program_len;

extern u64 reg[5]; 
extern u64* RAX;
extern u64* RBX;
extern u64* RCX;
extern u64* RDX;
extern u64* RSX;

void inc_IP(u64 pos);