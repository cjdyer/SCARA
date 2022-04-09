#include "auton_routine.h"

void Auton::run_auton()
{
    home(0.0001);
    sleep(5); // No response to check for homed so have to sleep
    move_to_sync({240,370}); // Top right
    move_to_sync({420,300}); // Middle right (so doesnt hit the wall, like dumb robot)
    move_to_sync({600,370}); // Bottom right
    move_to_sync({600,-80}); // Bottom left
    move_to_sync({420,  0}); // Middle left (so doesnt hit the wall, like dumb robot)
    move_to_sync({240,-80}); // Top left
}

void Auton::home(double _move_accel)
{
    tm.send_terminal("HOME," + std::to_string(_move_accel)); 
}

void Auton::move_to(MoveData _move_pos)
{
    tm.send_terminal("MOVE," + std::to_string(_move_pos.x) + "," + std::to_string(_move_pos.y) + ",0");
}

void Auton::move_to_sync(MoveData _move_pos)
{
    tm.set_machine_state(MachineState::RUNNING);
    move_to(_move_pos);
    while(tm.get_machine_state() == MachineState::RUNNING) usleep(100);
}