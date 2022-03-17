#include "task_manager.h"

TaskManager::TaskManager() : AsyncHandler()
{
    //m_timer.set_timer_callback(std::bind(&TaskManager::timer_callback, this));
    m_terminal.start_polling();

    Log::log_info("TaskManager::TaskManager - Started Task Manager");
}

TaskManager::~TaskManager()
{
    m_terminal.stop_polling();
    m_timer.stop_timer();
    m_timer.unset_timer_callback();

    stop_handler(); // Stop async handler
    Log::log_info("TaskManager::~TaskManager - Task Manager terminated gracefully...");
}

void TaskManager::handle_trigger_action(struct CallbackAction& action)
{   
    handle_states(action);
    Log::log_info("TaskManager::handle_trigger_action - Action triggered");
}

void TaskManager::timer_callback()
{
    Log::log_info("TaskManager::timer_callback - Enter function");

    // Trigger this action to be handled on a different thread, so this callback can return immediately
    struct CallbackAction action;
    action.type = CallbackType::timeout_callback;
    trigger_handler_thread(action);
}

void TaskManager::handle_states(const struct CallbackAction& action)
{
    // State machine
}

void TaskManager::send_ping_terminal()
{
    //const char* msg = "~ID\r";
    //const char* msg = "~PING\r";
    //const char* msg = "~PROX\r";
    //const char* msg = "~ROFFSET\r";
    const char* msg = "~HOME,0.00005\r";
    //const char* msg = "~MOVE,300,300,0\r";
    m_terminal.write_serial(msg);
}