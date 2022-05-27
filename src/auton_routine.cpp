#include "auton_routine.h"

using namespace GPIO;

void Auton::run_auton()
{
    gpio_init();

    // To begin we must home the arm

    home(0.0001);

    // sleep(5); // Don't need now because included in function
    Log::log_info("Auton::run_auton - Homed");

    // then move the belt forward with the two blocks

    // void use_belt(int pulse, int direction, int enable);
    use_belt(pulse,direction,enable); // moves 10,000 steps forward
    
    // then position the arm above the left hand box
    // move_to_sync({600,-80}); // Bottom left

    // lower the pneumatics 
    // Pneumatics
    //  2 - Piston Down
    //  3 - Piston Up
    use_pneumatics(2);

    // close the gripper

    gripper_close(2);

    // Move the pneumatics up again
    use_pneumatics(3);

    // Move the arm over to the side
    // move_to_sync({420,300}); // Middle right

    // Move the pneumatics down
    use_pneumatics(2);

    // release the gripper to place the box
    gripper_open(2);

    // Move the pneumatics up again
    use_pneumatics(3);

    // move to second box position
    // move_to_sync({600,370}); // Bottom right

    // lower pneumatics
    use_pneumatics(2);

    // close gripper
    gripper_close(2);

    // lift pneumatics
    use_pneumatics(3);

    // move box to same position on the side
    // move_to_sync({420,300}); // Middle right

    // lower pneumatics
    use_pneumatics(2);

    // release gripper
    gripper_open(2);

    // return arm to home and finish
    home(0.0001);

    // ~OFFSET,1,2500
    // sleep(2);
    // tm.send_terminal("OFFSET,3,130"); // 2500, -1250, 130
    // sleep(4);
    // tm.send_terminal("ROFFSET");
    // home(0.0001);
    
}
    
// Run conveyor belt
void Auton::use_belt(int pulse, int direction, int enable)
{
    gpio_set_function(enable, PI_FUNCTION::OUTPUT);
    gpio_set_function(direction, PI_FUNCTION::OUTPUT);
    gpio_set_function(pulse, PI_FUNCTION::OUTPUT);
    gpio_write(enable, PI_OUTPUT::LOW);
    gpio_write(direction, PI_OUTPUT::LOW);
    int step_count = 0;

    while (step_count < 10000)
    {
        gpio_write(pulse, PI_OUTPUT::HIGH);
        usleep(10);
        gpio_write(pulse, PI_OUTPUT::LOW);
        usleep(10);
        step_count++;
    }

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
    

    Log::log_info("Belt moved forward");
}

// Move the arm
void Auton::use_pneumatics(int pin_direction)
{
    // Pneumatics
    //  2 - Piston Down
    //  3 - Piston Up

    gpio_set_function(pin_direction, PI_FUNCTION::OUTPUT);

    usleep(100);

    gpio_write(pin_direction, PI_OUTPUT::HIGH);
    sleep(4); // give time for pneumatics to finish
    gpio_write(pin_direction, PI_OUTPUT::LOW); // set low again ready for other solenoid to run
    Log::log_info("Piston moved");
    // this is what it was and I think this was wrong, was writing both high and doing this same thing every 4 sec. 
    // Never did we write the pin low. What we need to do is write it low after a period of time, so that when the 
    // other pin is set high it will trigger the other solenoid to run. 

    // while (true)
    // {
    //     gpio_write(2, PI_OUTPUT::HIGH);
    //     gpio_write(3, PI_OUTPUT::HIGH);
    //     sleep(4);

    //     std::cout << "pneumatics up" << std::endl;
    // }
}

void Auton::gripper_close(int pin)
{
    Servo s = Servo(pin);
    Log::log_info("Auton::run_auton - Setting servo to Min");
    for (int i = 1500; i > 400; i -= 100)
    {
        s.write_servo(i);
        sleep(1);
        std::cout << "SERVO TO : " << i << std::endl;
    }
    sleep(5);
    Log::log_info("Gripper closed");
}

void Auton::gripper_open(int pin)
{
    Servo s = Servo(pin);
    Log::log_info("Auton::run_auton - Setting servo to Max");
    for (int i = 100; i < 1400; i += 100)
    {
        s.write_servo(i);
        sleep(1);
        std::cout << "SERVO TO : " << i << std::endl;
    }
    sleep(5);
    Log::log_info("Gripper opened");
}

void Auton::home(double _move_accel)
{
    tm.send_terminal("HOME," + std::to_string(_move_accel));
    sleep(5); 
    Log::log_info("Home");
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
    Log::log_info("Arm repositioned");
    // {
    //     GPIO::gpioWrite(17, 1);
    //     usleep(1);
    //     GPIO::gpioWrite(17, 0);
    //     usleep(1);
    // }
}