#pragma once

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "geom.h"
#include <cstdlib>

using std::vector;

struct object
{
    vector<point> points;
    vector<triangle> triangles;
};

object parse(const char* filename)
{
    object result;
    std::ifstream in(filename);
    if (!in.is_open())
        std::cout << "Can't open file " << filename << '\n';

    std::string line;
    while (std::getline(in, line))
    {
        std::istringstream iss(line);
        std::string word;
        iss >> word;

        if (word == "v")
        {
            double a = 0, b = 0, c = 0;
            if(!(iss >> a >> b >> c))
                throw std::runtime_error("could not parse line: " + line);

            point temp(a, b, c);
            result.points.push_back(temp);
        }
        else if (word == "f")
        {
            int a[3];
            std::string temp;
            for (int i = 0; i < 3; i++)
            {
                iss >> temp;
                std::istringstream helper(temp);
                helper >> a[i];
                temp.clear();
            }
            vector<point> pt = result.points;
            triangle tr(pt[a[0] - 1], pt[a[1] - 1], pt[a[2] - 1]);
            result.triangles.push_back(tr);
        }
    }

    return result;
}