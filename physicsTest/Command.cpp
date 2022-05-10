#include <fstream>
#include "Command.hpp"
#include <iostream>
#include <sstream>

using namespace std;

Command::Command(int tick, int* bCode) :
    tick(tick),
    stackSize(0),
    byteCode(bCode)
{}

void Command::execute(vector<Unit*>* units)
{
    srand(getTick());
    
    int* head = byteCode;
    while (*head != Command::Bytecode::END)
    {
        Bytecode command = (Bytecode)*head;
        
        switch (command) {
            case Bytecode::PUSH:
                head++;
                push(*head);
                break;
            case Bytecode::DUP:
                push(peek());
                break;
            case Bytecode::SUB:
                {
                    int value2 = pop();
                    int value1 = pop();
                    push(value1 - value2);
                }
                break;
            case Bytecode::RAND:
                push(rand() % pop());
                break;
            case Bytecode::PROD_UNIT:
                {
                    int value3 = pop();
                    int value2 = pop();
                    int value1 = pop();
                    Unit* u = (*units)[value1];
                    u->increaseVelocity(Vec(value2, value3));
                }
                break;
            case Bytecode::JUMP_GT:
                {
                    int index = pop();
                    int value2 = pop();
                    int value1 = pop();
                    if (value1 > value2)
                    {
                        head = &byteCode[index];
                    }
                }
                break;
            default:
                break;
        }
        head++;
    }
}

void Command::push(int value)
{
    assert(stackSize < MAX_STACK);
    stack[stackSize++] = value;
}

int Command::pop()
{
    assert(stackSize > 0);
    return stack[--stackSize];
}

int Command::peek()
{
    assert(stackSize > 0);
    return stack[stackSize - 1];
}


int* Command::readBytecode(string filename)
{
    ifstream infile(filename);
    string line;
    vector<int> buffer;
    
    if (!infile.is_open())
    {
        cout << "ERROR: Can't find file " << filename << endl;
        return nullptr;
    }

    while (getline(infile, line))
    {
        string cleaned = line.substr(0, line.find("#"));
        stringstream ss;
        
        ss << cleaned;
        string cmd;
        int value;
        while (ss >> cmd)
        {
            if (cmd == "PUSH")
            {
                buffer.push_back(Bytecode::PUSH);
                ss >> value;
                buffer.push_back(value);
            }
            else if (cmd == "DUP")
            {
                buffer.push_back(Bytecode::DUP);
            }
            else if (cmd == "SUB")
            {
                buffer.push_back(Bytecode::SUB);
            }
            else if (cmd == "RAND")
            {
                buffer.push_back(Bytecode::RAND);
            }
            else if (cmd == "PROD_UNIT")
            {
                buffer.push_back(Bytecode::PROD_UNIT);
            }
            else if (cmd == "JUMP_GT")
            {
                buffer.push_back(Bytecode::JUMP_GT);
            }
        }
    }
    
    auto lenght = buffer.size();
    auto byteCode = new int[lenght + 1];
    for (int i = 0; i < lenght; i++)
    {
        byteCode[i] = buffer[i];
    }
    byteCode[lenght] = Bytecode::END;
    buffer.clear();
    
    return byteCode;
}
