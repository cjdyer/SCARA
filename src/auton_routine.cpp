#include "auton_routine.h"

// Autonomous routine laid out below

void Auton::run_auton()
{
    wiringPiSetup();    // Setup the WiringPi library to manipulate GPIO 

    // To begin we must home the arm

    home(0.0001);    // Move the arm to a home position to start the process
    Log::log_info("Auton::run_auton - Homed");

    // // then move the belt forward with the two blocks
    Log::log_info("run belt");
    use_belt(pulse,direction,enable); // moves 10,000 steps forward
    
    // then position the arm above the left hand box
    move_to_sync({575,60}); // Bottom left
    Log::log_info("Move to left box");
    
    // // Pneumatics
    // //  WiringGPIO 8 - GPIO2 - Piston Up
    // //  WiringGPIO 9 - GPIO3 - Piston Down 
    use_pneumatics(8);  // Initially always ensure to set pneumatics high

    // // lower the pneumatics 
    Log::log_info("Piston down");
    use_pneumatics(9);

    // // close the gripper
    gripper_close(23);
    Log::log_info("Gripper closed");

    // Move the pneumatics up again
    Log::log_info("Piston up");
    use_pneumatics(8);

    // // Move the arm over to the side
    move_to_sync({375,360}); // to the side for stacking
    Log::log_info("Move to side");

    // // Move the pneumatics down
    Log::log_info("Piston down");
    use_pneumatics(9);

    // release the gripper to place the box
    gripper_open(23);
    Log::log_info("Gripper release");

    // Move the pneumatics up again
    Log::log_info("Piston up");
    use_pneumatics(8);

    // // move to second box position
    move_to_sync({575,190}); // second box position
    Log::log_info("Move to right box");

    // lower pneumatics
    Log::log_info("Piston down");
    use_pneumatics(9);

    // close gripper
    gripper_close(23);
    Log::log_info("Gripper closed");

    // lift pneumatics
    Log::log_info("Piston up");
    use_pneumatics(8);

    // // move box to same position on the side
    move_to_sync({375,360}); // to the side for stacking
    Log::log_info("Move to side");

    // lower pneumatics
    Log::log_info("Piston down");
    use_pneumatics(9);

    // release gripper
    gripper_open(23);
    Log::log_info("Gripper release");

    // raise pneumatics
    Log::log_info("Piston up");
    use_pneumatics(8);

    // return arm to home and finish
    home(0.0001); 
    Log::log_info("Routine complete"); 
}

////////////////////////////////////////////////////////////////////////
// SCARA Action Functions
////////////////////////////////////////////////////////////////////////
    
// Run conveyor belt function
void Auton::use_belt(int pulse, int direction, int enable)
{
    pinMode(pulse, OUTPUT);
    pinMode(direction, OUTPUT);
    pinMode(enable, OUTPUT);
    digitalWrite(enable, 0);
    digitalWrite(direction, 0);

    int step_count = 0;

    while (step_count < 10000)
    {
        digitalWrite(pulse, 1);
        usleep(10);
        digitalWrite(pulse, 0);
        usleep(10);
        step_count++;
    }
    Log::log_info("Belt moved forward");
}

// Move the arm function
void Auton::use_pneumatics(int pin_direction)
{
    // Pneumatics
    //  2 - Piston Up - wiringGPIO 8
    //  3 - Piston Down - wiringGPIO 9
    pinMode(pin_direction, OUTPUT);
    usleep(100);
    digitalWrite(pin_direction, 1);
    if(pin_direction==8){
        sleep(1);
    }
    else{
        usleep(250000); // shortened time to prevent gripper damage
    }
    digitalWrite(pin_direction, 0);
    Log::log_info("Piston moved");
    sleep(1);
}

// Gripper close function
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

// Gripper open function
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
    tm.send_terminal("HOME,0.0001");//"HOME," + std::to_string(_move_accel));
    sleep(5); 
    Log::log_info("Home");
}

// Move arm functions, including a syncronous function which ensures arm finished moving before any subsequent actions are available
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
}