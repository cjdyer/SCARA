#pragma once

#include "task_manager.h"
#include "gpio.h"
#include "servo.h"

struct MoveData
{
    int16_t x, y;
};

class Auton
{
public:
    Auton() {}
    ~Auton() {}

    void run_auton();

private:
    void gripper_close(int pin);
    void gripper_open(int pin);
    void use_belt(int pulse, int direction, int enable);
    void use_pneumatics(int pin_direction);
    void move_to(MoveData _move_pos);
    void move_to_sync(MoveData _move_pos);
    void home(double _move_accel);

private:
    TaskManager tm;
    int pulse = 21;
    int direction = 20;
    int enable = 16;

}; // class Auton
