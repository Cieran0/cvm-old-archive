#include "cvm.h"
#include "commands.h"

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

void inc_IP(u64 pos) {
    IP = (instruction*)((u64)IP + pos);
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