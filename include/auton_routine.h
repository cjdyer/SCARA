#pragma once

#include "task_manager.h"

struct MoveData
{
    int16_t x, y;
};

class Auton
{
public:
    void run_auton();
    Auton() {}
    ~Auton() {}

private:
    void move_to(MoveData _move_pos);
    void move_to_sync(MoveData _move_pos);
    void home(double _move_accel);

private:
    TaskManager tm;

}; // class Auton
