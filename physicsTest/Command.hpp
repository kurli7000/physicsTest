#ifndef Command_hpp
#define Command_hpp

#include <stdio.h>
#include "Unit.hpp"
#include "Vec.h"

class Command
{
public:
    Command(int tick, int* bCode);
    void execute(std::vector<Unit*>* units);
    int getTick() { return tick; }
    
    static int* readBytecode(std::string filename);
    
    enum Bytecode
    {
        PUSH = 1,
        DUP = 2,
        SUB = 3,
        RAND = 4,
        PROD_UNIT = 5,
        JUMP_GT = 6,
        END = 7
    };

private:
    void push(int value);
    int pop();
    int peek();
    
    int tick;
    int* byteCode;
    static const int MAX_STACK = 32;
    int stackSize;
    int stack[MAX_STACK];
};

#endif /* Command_hpp */
