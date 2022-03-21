#pragma once

#include <cstdint>
#include "async_handler.h"
#include "timer.h"
#include "terminal.h"

enum class MachineState
{
    STOPPED = 0,
    RUNNING = 1
};

enum class CallbackType
{
    undefined = 0,
    terminal_callback = 1
};

struct CallbackAction
{
    CallbackType type = CallbackType::undefined;
    TerminalAction terminal_action;
};

class TaskManager : public AsyncHandler<struct CallbackAction>
{
public:
    TaskManager();
    virtual ~TaskManager();

    void handle_trigger_action(struct CallbackAction& action) override;

    void send_terminal(std::string _msg);
    void set_machine_state(MachineState _machine_state);
    MachineState get_machine_state();
    
private: // Functions
    void terminal_callback();

private: // Variables
    Terminal m_terminal;
    std::atomic<MachineState> m_machine_state;
};