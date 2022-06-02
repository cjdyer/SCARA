#include "task_manager.h"

TaskManager::TaskManager()
{
    m_terminal.start_polling();
    m_terminal.set_read_callback(std::bind(&TaskManager::terminal_callback, this));

    Log::log_info("TaskManager::TaskManager - Started Task Manager");
}

TaskManager::~TaskManager()
{
    // m_terminal.stop_polling();
    // m_terminal.unset_read_callback();

    Log::log_info("TaskManager::~TaskManager - Task Manager terminated gracefully...");
}

void TaskManager::terminal_callback()
{
    Log::log_info("TaskManager::terminal_callback");

    // Trigger this action, so this callback can return immediately
    m_machine_state.store(MachineState::STOPPED);
}

void TaskManager::set_machine_state(MachineState _machine_state)
{
    m_machine_state.store(_machine_state);
}

MachineState TaskManager::get_machine_state()
{
    return m_machine_state.load();
}

void TaskManager::send_terminal(std::string _msg)
{
    m_terminal.write_serial(("~" + _msg + "\r").c_str());
}