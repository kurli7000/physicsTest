#ifndef CommandDispatcher_hpp
#define CommandDispatcher_hpp

#include <stdio.h>
#include <list>
#include "Simulation.hpp"
#include "Command.hpp"

class CommandDispatcher
{
public:
    struct CommandToSend
    {
        Command* command;
        int sendAtTick;
        bool sent;
        CommandToSend(Command* cmd, int tick) : command(cmd), sendAtTick(tick), sent(false) {};
    };
    
    CommandDispatcher(Simulation* sim1, Simulation* sim2) : simulation1(sim1), simulation2(sim2) {};
    void update(int tick);
    void init();
    
private:
    Simulation* simulation1;
    Simulation* simulation2;
    std::list<CommandToSend> commands1;
    std::list<CommandToSend> commands2;
    
    void generateCommand(int currentTick);
    void sendCommands(int currentTick);
    
    int* byteCode;
    
    int lastCommandTick;
};


#endif /* CommandDispatcher_hpp */
