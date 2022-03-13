#include "auton_routine.h"

void Auton::run_auton()
{
    TaskManager m_task_manager;
    m_task_manager.send_ping_terminal();
    sleep(2);
}