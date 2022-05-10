#include "CommandDispatcher.hpp"

using namespace std;


void CommandDispatcher::generateCommand(int currentTick)
{
    int cmdTick = currentTick + 1000 / Simulation::MS_PER_TICK; // 1 second into future
    Vec velocity = simulation1->getAttackDirection();
    
    Command* cmd = new Command(0, cmdTick, velocity);
    
    // send command to simulation1 immediately
    commands1.push_back(CommandToSend(cmd, currentTick));

    // delay simulation2 command randomly
    int delay = rand() % (1000 / Simulation::MS_PER_TICK * 3); // max 3 second delay from current time
    commands2.push_back(CommandToSend(cmd, currentTick + delay));
    
    cout << "New command, tick: " << cmd->getTick() << ", Simulation1 tick " << currentTick << ", Simulation2 tick " << currentTick + delay << endl;
    
    lastCommandTick = cmdTick;
}

void CommandDispatcher::sendCommands(int currentTick)
{
    // simulation1
    for (auto it = commands1.begin(); it != commands1.end(); it++)
    {
        if ((*it).sendAtTick <= currentTick && !(*it).sent)
        {
            simulation1->addCommand((*it).command);
            (*it).sent = true;
        }
    }

    // simulation2
    for (auto it = commands2.begin(); it != commands2.end(); it++)
    {
        if ((*it).sendAtTick <= currentTick && !(*it).sent)
        {
            simulation2->addCommand((*it).command);
            (*it).sent = true;
        }
    }
}

void CommandDispatcher::init()
{
    generateCommand(0);

    auto buffer = Command::readBytecode("random_prod.bytecode");
    
    while (*buffer != Command::Bytecode::END)
    {
        cout << "bytecode: " << *buffer << endl;
        buffer++;
    }
}

void CommandDispatcher::update(int tick)
{
    // if all commands are sent, generate a new command every 3 seconds
    if (tick > lastCommandTick + 1000 / Simulation::MS_PER_TICK * 2)
    {
        generateCommand(tick);
    }
    
    sendCommands(tick);
}

