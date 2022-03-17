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
    user_input_callback = 1,
    timeout_callback = 2
};

struct CallbackAction
{
    CallbackType type = CallbackType::undefined;
    UserAction user_action;
};

class TaskManager : public AsyncHandler<struct CallbackAction>
{
public:
    TaskManager();
    virtual ~TaskManager();

    void handle_trigger_action(struct CallbackAction& action) override;

    void send_ping_terminal();
    
private: // Functions
    void timer_callback();
    void input_callback();
    void handle_states(const struct CallbackAction& action);

private: // Variables
    Timer m_timer;
    Terminal m_terminal;
};