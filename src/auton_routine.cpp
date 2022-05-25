#include "auton_routine.h"

using namespace GPIO;

void Auton::run_auton()
{
    gpio_init();
    // Servo s = Servo(13);
    // Log::log_info("Auton::run_auton - Setting servo to Min");
    // for (int i = 1500; i > 400; i -= 100)
    // {
    //     s.write_servo(i);
    //     sleep(1);
    //     std::cout << "SERVO TO : " << i << std::endl;
    // }
    sleep(1);


    // ~OFFSET,1,2500
    // sleep(2);
    // tm.send_terminal("OFFSET,3,130"); // 2500, -1250, 130
    // sleep(4);
    // tm.send_terminal("ROFFSET");
    // home(0.0001);
    
    // 17 - Step 
    // 27 - Direction
    // 22 - Enable
    //  2 - Piston Down
    //  3 - Piston Up

    gpio_set_function(2, PI_FUNCTION::OUTPUT);
    gpio_set_function(3, PI_FUNCTION::OUTPUT);

    usleep(100);

    while (true)
    {
        gpio_write(2, PI_OUTPUT::HIGH);
        gpio_write(3, PI_OUTPUT::HIGH);
        sleep(4);

        std::cout << gpio_read(2) << std::endl;
    }

    // // Log::log_info("Auton::run_auton - GPIO set");
    // usleep(100);


    // gpio_set_function(16, PI_FUNCTION::OUTPUT);
    // gpio_set_function(20, PI_FUNCTION::OUTPUT);
    // gpio_set_function(21, PI_FUNCTION::OUTPUT);
    // gpio_write(16, PI_OUTPUT::LOW);
    // gpio_write(20, PI_OUTPUT::LOW);
    // int step_count = 0;

    // while (step_count < 10000)
    // {
    //     gpio_write(21, PI_OUTPUT::HIGH);
    //     usleep(10);
    //     gpio_write(21, PI_OUTPUT::LOW);
    //     usleep(10);
    //     step_count++;
    // }
    

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