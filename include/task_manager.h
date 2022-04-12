#pragma once

#include <cstdint>
#include "log.h"
#include "terminal.h"
// #include "gpio.h"

enum class MachineState
{
    STOPPED = 0,
    RUNNING = 1
};

class TaskManager
{
public:
    TaskManager();
    virtual ~TaskManager();

    void send_terminal(std::string _msg);

    void set_machine_state(MachineState _machine_state);
    MachineState get_machine_state();
    
private: // Functions
    void terminal_callback();

private: // Variables
    Terminal m_terminal;
    std::atomic<MachineState> m_machine_state;
    
}; // class TaskManager