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
    void move_to(MoveData _move_pos);
    void move_to_sync(MoveData _move_pos);
    void home(double _move_accel);

private:
    TaskManager tm;

}; // class Auton
