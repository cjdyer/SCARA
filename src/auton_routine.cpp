#include "auton_routine.h"

// using namespace GPIO;

void Auton::run_auton()
{
    // gpio_init();
    wiringPiSetup();

    // tm.send_terminal("OFFSET,1,2500"); // 2500, -1250, 130
    // sleep(1);
    // tm.send_terminal("OFFSET,2,-1250"); // 2500, -1250, 130
    // sleep(1);
    // tm.send_terminal("OFFSET,3,130"); // 2500, -1250, 130
    // sleep(1);
    // tm.send_terminal("ROFFSET");
    // sleep(10);

    // // To begin we must home the arm

    // home(0.0001);

    // tm.send_terminal("ROFFSET");

    // sleep(1);

    // // sleep(5); // Don't need now because included in function
    // Log::log_info("Auton::run_auton - Homed");


    // // then move the belt forward with the two blocks
    Log::log_info("run belt");

    // // void use_belt(int pulse, int direction, int enable);
    use_belt(pulse,direction,enable); // moves 10,000 steps forward
    sleep(1);
    // // then position the arm above the left hand box
    // move_to_sync({575,60}); // Bottom left
    // sleep(5);

    // // lower the pneumatics 
    // // Pneumatics
    // //  WiringGPIO 8 - GPIO2 - Piston Up
    // //  WiringGPIO 9 - GPIO3 - Piston Down
    Log::log_info("Piston down");
    use_pneumatics(9);

    // // close the gripper

    gripper_close(23);
    sleep(5);

    // Move the pneumatics up again
    Log::log_info("Piston up");
    use_pneumatics(8);

    // // Move the arm over to the side
    // move_to_sync({375,360}); // to the side for stacking
    // sleep(5);

    // // Move the pneumatics down
    // Log::log_info("Piston down");
    use_pneumatics(9);

    // release the gripper to place the box
    gripper_open(23);

    // Move the pneumatics up again
    Log::log_info("Piston up");
    use_pneumatics(8);

    // // move to second box position
    // move_to_sync({575,190}); // second box position
    // sleep(5);

    // lower pneumatics
    Log::log_info("Piston down");
    use_pneumatics(9);

    // close gripper
    gripper_close(23);

    // lift pneumatics
    Log::log_info("Piston up");
    use_pneumatics(8);

    // // move box to same position on the side
    // move_to_sync({375,360}); // to the side for stacking
    // sleep(5);

    // lower pneumatics
    Log::log_info("Piston down");
    use_pneumatics(9);

    // release gripper
    gripper_open(23);

    // raise pneumatics
    Log::log_info("Piston up");
    use_pneumatics(8);

    // return arm to home and finish
    // home(0.0001);

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
    // gpio_set_function(enable, PI_FUNCTION::OUTPUT);
    // gpio_set_function(direction, PI_FUNCTION::OUTPUT);
    // gpio_set_function(pulse, PI_FUNCTION::OUTPUT);
    // gpio_write(enable, PI_OUTPUT::LOW);
    // gpio_write(direction, PI_OUTPUT::LOW);

    pinMode(pulse, OUTPUT);
    pinMode(direction, OUTPUT);
    pinMode(enable, OUTPUT);
    digitalWrite(enable, 0);
    digitalWrite(direction, 0);

    int step_count = 0;

    while (step_count < 10000)
    {
        // gpio_write(pulse, PI_OUTPUT::HIGH);
        // usleep(10);
        // gpio_write(pulse, PI_OUTPUT::LOW);
        // usleep(10);
        digitalWrite(pulse, 1);
        usleep(10);
        digitalWrite(pulse, 0);
        usleep(10);

        step_count++;
        Log::log_info("step_count");
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
    //  2 - Piston Up - wiringGPIO 8
    //  3 - Piston Down - wiringGPIO 9

    // gpio_set_function(pin_direction, PI_FUNCTION::OUTPUT);
    pinMode(pin_direction, OUTPUT);

    usleep(100);

    // gpio_write(pin_direction, PI_OUTPUT::HIGH);
    digitalWrite(pin_direction, 1);
    // Log::log_info("Piston moving");
    if(pin_direction==8){
        sleep(1);
    }
    else{
        usleep(300000); // shortened time to prevent gripper damage
    }
    
    // gpio_write(pin_direction, PI_OUTPUT::LOW); // set low again ready for other solenoid to run
    digitalWrite(pin_direction, 0);
    Log::log_info("Piston moved");
    sleep(1);
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
    for (int i = 250; i > 50; i -= 1)
    {
        s.write_servo(i);
        usleep(10000);
        std::cout << "SERVO TO : " << i << std::endl;
    }
    usleep(10000);
    Log::log_info("Gripper closed");
}

void Auton::gripper_open(int pin)
{
    Servo s = Servo(pin);
    Log::log_info("Auton::run_auton - Setting servo to Max");
    for (int i = 50; i < 250; i += 1)
    {
        s.write_servo(i);
        usleep(10000);
        std::cout << "SERVO TO : " << i << std::endl;
    }
    s.write_servo(0); // When open want to turn pwm signal to zero so it doesn't continue to engage motor
    usleep(10000);
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