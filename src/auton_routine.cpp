#include "auton_routine.h"

// Autonomous routine laid out below
const int down_pin = 9;
const int up_pin = 8;

void Auton::run_auton()
{
    wiringPiSetup(); // Setup the WiringPi library to manipulate GPIO
    pinMode(down_pin, OUTPUT);
    pinMode(up_pin, OUTPUT);
    usleep(100);
    // To begin we must move the belt to under the camera

    Log::log_info("run belt");
    use_belt(pulse,direction,enable); // moves 10,000 steps forward

    home(0.0001);    // Move the arm to a home position to start the process
    Log::log_info("Auton::run_auton - Homed");

    // then position the arm above the left hand box
    move_to_sync({566,60}); // Bottom left
    Log::log_info("Move to left box");

    // Pneumatics
    //  WiringGPIO 8 - GPIO2 - Piston Up
    //  WiringGPIO 9 - GPIO3 - Piston Down
    Log::log_info("P Up");
    use_pneumatics(true, 0);
    usleep(200000);  // Initially always ensure to set pneumatics high

    // lower the pneumatics
    Log::log_info("P Down");
    use_pneumatics(false, 255000);
    sleep(1);

    // close the gripper
    gripper_close(23, 180, 80);
    Log::log_info("Gripper closed");
    sleep(1);

    // Move the pneumatics up again
    Log::log_info("P Up");
    use_pneumatics(true, 0);
    sleep(1);

    // // Move the arm over to the side
    move_to_sync({375,360}); // to the side for stacking
    Log::log_info("Move to side");

    // // Move the pneumatics down
    Log::log_info("P Down");
    use_pneumatics(false, 1000000);
    sleep(1);

    // release the gripper to place the box
    gripper_open(23, 180, 90);
    Log::log_info("Gripper release");

    // // Move the pneumatics up again
    Log::log_info("P Up");
    use_pneumatics(true, 0);
    sleep(1);

    // // move to second box position
    move_to_sync({564,150}); // Bottom left
    Log::log_info("Move to left box");

    // // lower pneumatics
    Log::log_info("P Down");
    use_pneumatics(false, 265000);
    sleep(1);

    // // close gripper
    gripper_close(23, 180, 70);
    Log::log_info("Gripper closed");

    // // lift pneumatics
    Log::log_info("P Up");
    use_pneumatics(true, 0);
    sleep(1);

    // // move box to same position on the side
    move_to_sync({375,360}); // to the side for stacking
    Log::log_info("Move to side");

    // // lower pneumatics
    Log::log_info("P down");
    use_pneumatics(false, 350000);
    sleep(1);

    // // release gripper
    gripper_open(23, 180, 70);
    Log::log_info("Gripper release");

    // // raise pneumatics
    Log::log_info("P Up");
    use_pneumatics(true, 0);
    sleep(1);

    // Move to middle position before homing to avoid wall
    move_to_sync({350,-100});
    Log::log_info("Move to side");

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
    digitalWrite(direction, 1);

    int step_count = 0;

    while (step_count < 11000)
    {
        digitalWrite(pulse, 1);
        usleep(20);
        digitalWrite(pulse, 0);
        usleep(20);
        step_count++;
    }
    Log::log_info("Belt moved forward");
}

// Move the arm function

void Auton::use_pneumatics(bool dir, int downtime)
{
    if (dir)
    {
        digitalWrite(down_pin, 0);
        digitalWrite(up_pin, 1);
        sleep(1);
    }
    else
    {
        digitalWrite(down_pin, 1);
        digitalWrite(up_pin, 0);
        usleep(downtime); // shortened time to prevent gripper damage
    }
    digitalWrite(up_pin, 1);
    digitalWrite(down_pin, 1);
}

// Gripper close function

void Auton::gripper_close(int pin, int max, int min)
{
    Servo s = Servo(pin);
    Log::log_info("Auton::run_auton - Setting servo to Min");
    for (int i = max; i > min; i -= 1)
    {
        s.write_servo(i);
        usleep(10000);
        std::cout << "SERVO TO : " << i << std::endl;
    }
    usleep(10000);
    Log::log_info("Gripper closed");
}

// Gripper open function
void Auton::gripper_open(int pin, int max, int min)
{
    Servo s = Servo(pin);
    Log::log_info("Auton::run_auton - Setting servo to Max");
    for (int i = min; i < max; i += 1)
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
    tm.send_terminal("HOME,0.0001"); //"HOME," + std::to_string(_move_accel));
    sleep(2);
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
    while (tm.get_machine_state() == MachineState::RUNNING)
        ;
    Log::log_info("Arm repositioned");
}
