#pragma once

#include "task_manager.h"

struct MoveData
{
    uint16_t x, y, w;
};


class Auton
{
public:
    static void run_auton();

private:
    explicit Auton();
    ~Auton();

}; // class Auton
