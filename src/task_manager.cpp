#include "task_manager.h"

TaskManager::TaskManager() : AsyncHandler(), m_terminal("/dev/ttyACM0")
{
    m_timer.set_timer_callback(std::bind(&TaskManager::timer_callback, this));
    m_terminal.start_polling();

    Log::log_info("TaskManager::TaskManager - Started Task Manager");
}

TaskManager::~TaskManager()
{
    stop_handler(); // Stop async handler

    m_terminal.stop_polling();
    m_timer.stop_timer();
    m_timer.unset_timer_callback();
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
    const char* msg = "~PING\r";
    m_terminal.write_serial(msg);
}