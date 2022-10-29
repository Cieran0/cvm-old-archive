#include <iostream>
#include <stack>

#define MEM_SIZE 134217728

typedef unsigned long u64;
typedef unsigned char u8;
typedef char s8;
typedef unsigned char instruction;
static u64* MAIN_MEM;
static instruction* IP;
static std::stack<u64> Stack;
static std::stack<u64> returnStack;

static bool end = false;
static u8 program[] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 17, 0, 0, 0, 0, 0, 0, 0, 8, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 6, 3, 4, 1, 0, 0, 0, 0, 0, 0, 0, 6, 11, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 255 };
static u8 program1[] = {1,4, 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x2C, 0x20, 0x57, 6,
                        1,4, 0x6F, 0x72, 0x6C, 0x64, 0x21, 0x0A, 0x00, 0x00, 6,
                        7,5, 1,0,0,0,0,0,0,0,4,7,5, 0,0,0,0,0,0,0,0, 4,
                        1,0, 1,0,0,0,0,0,0,0, 1,2, 0,0,0,0,0,0,0,0, 1,3, 14,0,0,0,0,0,0,0, 8, 255};
// 0x48 0x65 0x6C 0x6C 0x6F 0x2C 0x20 0x57
// 0x6F 0x72 0x6C 0x64 0x21 0x0A 0x00 0x00

static u64 program_len = sizeof(program);

static u64 reg[5]; 
static u64* RAX;
static u64* RBX;
static u64* RCX;
static u64* RDX;
static u64* RSX;

void load_program() {
    IP = program;
    }

void init_mem() { MAIN_MEM = (u64*)malloc(MEM_SIZE); }

void init_regs(){
    RAX = &reg[0];
    RBX = &reg[1];
    RCX = &reg[2];
    RDX = &reg[3];
    RSX = &reg[4];
}

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

void inc_IP(u64 pos) {
    IP = (instruction*)((u64)IP + pos);
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
    IP = (instruction*)(returnStack.top()-1);
    returnStack.pop();
}

void ExecuteCurrentInstruction(){
    switch (*IP)
    {
        case 0:
            break;
        case 1:
            mov_reg_u64(*(IP+1),*((u64*)(IP+2)));
            inc_IP(9);
            break;
        case 2:
            mov_reg_reg(*(IP+1), *(IP+2));
            inc_IP(2);
            break;
        case 3:
            mov_reg_mem(*(IP+1),*((u64*)(IP+2)));
            inc_IP(9);
            break;
        case 4:
            mov_mem_u64(*((u64*)(IP+1)),*((u64*)(IP+9)));
            inc_IP(16);
            break;
        case 5:
            mov_mem_reg(*((u64*)(IP+1)),*(IP+9));
            inc_IP(9);
            break;
        case 6:
            push();
            break;
        case 7:
            pop();
            break;
        case 8:
            syscall();
            break;
        case 9:
            jmpz(*((u64*)(IP+1)));
            break;
        case 10:
            jmpnz(*((u64*)(IP+1)));
            break;
        case 11:
            cmp(*(IP+1));
            break;
        case 12:
            jmp(*((u64*)(IP+1)));
            break;
        case 13:
            call(*((u64*)(IP+1)));
            break;
        case 14:
            ret();
            break;
        case 255:
            end = true;
            break;
        default:
            break;
    }
    inc_IP(1);
}

int main(){
    init_mem();
    init_regs();
    load_program();
    //std::cout << (u64)(IP-program) << std::endl;
    //std::cout << program_len << std::endl;
    while ((u64)(IP-program) <= program_len && !end)
    {
        ExecuteCurrentInstruction();
    }
    //std::cout << (u64)(IP-program) << std::endl;
    //std::cout << end << std::endl;
    return 0;
}