#pragma once

#include <cstdint>
#include "timer.h"
#include "terminal.h"
#include "async_handler.h"

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

    void ehh()
    {
        const char* msg = "~PING\r";
        m_terminal.write_serial(msg);
    }
    
private: // Functions
    void timer_callback();
    void input_callback();
    void handle_states(const struct CallbackAction& action);

private: // Variables
    Timer m_timer;
    Terminal m_terminal;
};