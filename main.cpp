#include "cvm.h"
#include "commands.h"

char* INSTRUCTIONS[] = {
    "do_nothing",
    "mov_reg_u64",
    "mov_reg_reg",
    "mov_reg_mem",
    "mov_mem_u64",
    "mov_mem_reg",
    "push",
    "pop",
    "syscall",
    "jmp",
    "jmpz",
    "jmpnz",
    "cmp",
    "call",
    "ret",
    "add_reg_u64",
    "add_reg_reg",
    "add_reg_mem",
    "add_mem_u64",
    "add_mem_reg"
};

u64 reverseu64(u64 num) {
    u64 temp = 0;
    u8* tempPtr = (u8*)&temp;
    u8* numPtr = (u8*)&num;
    for (int i = 0; i < 8; i++)
    {
        tempPtr[i] = numPtr[7-i];
    }
    return temp;
}

void load_program() {
    IP = program;
}

void init_mem() { 
    MAIN_MEM = (u64*)malloc(MEM_SIZE); 
    *MAIN_MEM = reverseu64(0x48656c6c6f2c2057); 
    *(MAIN_MEM+1) = reverseu64(0x6f726c64210a); 
}

void init_regs(){
    RAX = &reg[0];
    RBX = &reg[1];
    RCX = &reg[2];
    RDX = &reg[3];
    RSX = &reg[4];
}

void inc_IP(u64 pos) {
    IP = (instruction*)((u64)IP + pos);
}

void ExecuteCurrentInstruction(){
    //std::cout << "Executing instruction: [" << int(*IP) << "] - "  << INSTRUCTIONS[*IP] << std::endl;
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
            jmp(*((u64*)(IP+1)));
            inc_IP(8);
            break;
        case 10:
            jmpz(*((u64*)(IP+1)));
            inc_IP(8);
            break;
        case 11:
            jmpnz(*((u64*)(IP+1)));
            inc_IP(8);
            break;
        case 12:
            cmp(*(IP+1));
            inc_IP(1);
            break;
        case 13:
            call(*((u64*)(IP+1)));
            break;
        case 14:
            ret();
            break;
        case 15:
            add_reg_u64(*(IP+1),*((u64*)(IP+2)));
            inc_IP(9);
            break;
        case 16:
            add_reg_reg(*(IP+1), *(IP+2));
            inc_IP(2);
            break;
        case 17:
            add_reg_mem(*(IP+1),*((u64*)(IP+2)));
            inc_IP(9);
            break;
        case 18:
            add_mem_u64(*((u64*)(IP+1)),*((u64*)(IP+9)));
            inc_IP(16);
            break;
        case 19:
            add_mem_reg(*((u64*)(IP+1)),*(IP+9));
            inc_IP(9);
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