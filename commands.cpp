#include "commands.h"

void mov_reg_u64(u8 regNum, u64 num) {
    reg[regNum] = num;
}

void mov_reg_reg(u8 regNum1, u8 regNum2) {
    reg[regNum1] = regNum2;
}

void mov_reg_mem(u8 regNum, u64 memPos) { 
    reg[regNum] = MAIN_MEM[memPos];
}

void mov_mem_u64(u64 memPos, u64 num) {
    MAIN_MEM[memPos] = num;
}

void mov_mem_reg(u64 memPos, u8 regNum) {
    MAIN_MEM[memPos] = reg[regNum];
}

void push() {
    Stack.push(*RSX);
}

void pop() {
    *RSX = Stack.top();
    Stack.pop();
}

void write() {
    std::ostream &output = (*RBX  == 0) ?  std::cout : std::cout;
    for (u64 i = 0; i < *RDX; ++i)
    {
        output << *(((u8*)(MAIN_MEM+*RCX))+i);
    }
}

void read() {
    std::istream &input = (*RBX  == 0) ?  std::cin : std::cin;
    input.getline(((s8*)(MAIN_MEM+*RCX)), (*RDX));
}

void syscall() {
    switch (*RAX)
    {
    case 0:
        read();
        break;
    case 1:
        write();
        break;
    default:
        break;
    }
}



void jmp(u64 pos) {
    IP = program + (pos-1);
} 

void jmpz(u64 pos) {
    if(Stack.top()) { Stack.pop(); return; }
    jmp(pos);
}

void jmpnz(u64 pos) {
    if(!Stack.top()) { Stack.pop(); return; } 
    jmp(pos);
}

void cmp(u8 type) {
    u64 one = Stack.top(); Stack.pop();
    u64 two = Stack.top(); Stack.pop();
    switch (type)
    {
        case 0:
            Stack.push(one == two);
            break;
        case 1:
            Stack.push(one < two);
            break;
        case 2:
            Stack.push(one > two);
            break;
        case 3:
            Stack.push(one <= two);
            break;
        case 4:
            Stack.push(one >= two);
            break;
    default:
        break;
    }
}

void call(u64 pos)
{
    returnStack.push(((u64)IP)+9);
    jmp(pos);
}

void ret(){
    if(returnStack.empty()) {
        end = true;
        return; 
    }
    IP = (instruction*)(returnStack.top()-1);
    returnStack.pop();
}