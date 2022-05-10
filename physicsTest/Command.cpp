#include <fstream>
#include "Command.hpp"
#include <iostream>
#include <sstream>

using namespace std;

Command::Command(int unit, int tick, Vec velocityIncrease) :
    unit(unit),
    tick(tick),
    velocity(velocityIncrease)
{}

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
            else if (cmd == "SUB")
            {
                buffer.push_back(Bytecode::RAND);
            }
            else if (cmd == "SUB")
            {
                buffer.push_back(Bytecode::PROD_UNIT);
            }
            else if (cmd == "SUB")
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

void Command::execute(vector<Unit*>* units)
{
    Unit* u = (*units)[unit];
    u->increaseVelocity(velocity);
}
