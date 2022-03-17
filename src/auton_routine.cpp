#include "auton_routine.h"

void Auton::run_auton()
{
    TaskManager tm;
    tm.send_ping_terminal();
    sleep(2);
}