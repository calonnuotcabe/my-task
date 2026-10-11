#include <isa.h>

//Program
struct Program { 
    struct Instruction *instructions;
    size_t count;
    size_t capasity;
}