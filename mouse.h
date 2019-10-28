#pragma once

#include <iostream>
#include <algorithm>
#include <set>
#include <stdio.h>
#include <unistd.h>
#include <cmath>
#include <cstdlib>

struct pair
{
    signed char x, y;
};


class mouse
{
public:
    pair getmovement()
    {
        FILE * mice = fopen("/dev/input/mice", "rb");
        signed char temp[3] = {0, 0, 0};
        pair result;
        fread(temp, 1, 3, mice);

        result.x = temp[1];
        result.y = temp[2];

        fclose(mice);
        return result;
    }
    
    bool left_button_pressed()
    {
        FILE * mice = fopen("/dev/input/mice", "rb");
        signed char temp[3] = {0, 0, 0};
        fread(temp, 1, 3, mice);

        fclose(mice);
        return temp[0] & 0x1;
    }

    bool right_button_pressed()
    {
        FILE * mice = fopen("/dev/input/mice", "rb");
        signed char temp[3] = {0, 0, 0};
        fread(temp, 1, 3, mice);

        fclose(mice);
        return temp[0] & 0x2;
    }
};