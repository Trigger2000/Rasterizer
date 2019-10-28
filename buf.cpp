#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <algorithm>
#include <set>
#include "parser.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "mouse.h"

int main()
{   
    object cube = parse("cube.obj");
    
    pixel pix1 {255, 0, 0, 255};
    pixel pix2 {0, 255, 0, 255};
    /*pixel pix3 {0, 0, 255, 255};

    point first(300.0, 523.0, 0.0, pix1);
    point second(500.0, 200.0, 0.0, pix2);
    point third(1200.0, 600.0, 0.0, pix3);
    triangle temp(first, second, third);
    temp.draw(buffer); 

    line lfirst(first, second, pix1);
    line lsecond(first, third, pix2);
    line lthird(second, third, pix3); 

    lfirst.draw(buffer);
    lsecond.draw(buffer);
    lthird.draw(buffer);  */

    double x1 = 0.5;
    double x2 = 0.3;
    double tetta = 0;
    while (1)
    {      
        pixel* buffer = (pixel*)calloc(size_y * size_x, sizeof(pixel));

        int fb0 = open("/dev/fb0", O_WRONLY);

        for (auto item: cube.triangles)
        {
            item.drawlines(buffer, tetta);
        }

        mouse mice;
        pair mouse = mice.getmovement();
        tetta += static_cast<double> (mouse.x) / 360.0;
        //std::cout << coef << '\n';

        write(fb0, (void*)(buffer), size_x * size_y * sizeof(pixel));
        close(fb0);
    }
    
    return 0;
}