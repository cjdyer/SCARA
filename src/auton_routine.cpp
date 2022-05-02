#include "auton_routine.h"

using namespace GPIO;

void Auton::run_auton()
{
    gpio_init();
    Servo s = Servo(26);
    s.write_servo(1500);
    sleep(3);

    // home(0.0001);
    
    // 17 - Step 
    // 27 - Direction
    // 22 - Enable
    //  2 - Piston Down
    //  3 - Piston Up

    // GPIO::gpioSetMode(17, PI_OUTPUT);
    // GPIO::gpioSetMode(27, PI_OUTPUT);
    // GPIO::gpioSetMode(22, PI_OUTPUT);

    // Log::log_info("Auton::run_auton - GPIO set");

    // GPIO::gpioWrite(27, 0);
    // GPIO::gpioWrite(22, 0);

    // Log::log_info("Auton::run_auton - Stepper configured");

    // sleep(5); // No response to check for homed so have to sleep
    // Log::log_info("Auton::run_auton - Homed");

    // move_to_sync({240,370}); // Top right
    // move_to_sync({420,300}); // Middle right (so doesnt hit the wall, like dumb robot)
    // move_to_sync({600,370}); // Bottom right
    // move_to_sync({600,-80}); // Bottom left
    // move_to_sync({420,  0}); // Middle left (so doesnt hit the wall, like dumb robot)
    // move_to_sync({240,-80}); // Top left
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
    while(tm.get_machine_state() == MachineState::RUNNING);
    // {
    //     GPIO::gpioWrite(17, 1);
    //     usleep(1);
    //     GPIO::gpioWrite(17, 0);
    //     usleep(1);
    // }
}