#pragma once

#include <iostream>
#include <algorithm>
#include <set>
#include <stdio.h>
#include <unistd.h>
#include <cmath>
#include <cstdlib>

const int size_x = 1440, size_y = 900;
double coef = 2;

class pixel
{
public:
    pixel():
    b_(255), g_(255), r_(255), t_(255)
    {};

    pixel(unsigned char b, unsigned char g, unsigned char r, unsigned char t):
    t_(t), r_(r), g_(g), b_(b)
    {};

    pixel operator*(double val) const
    {
        pixel result (b_ * val, g_ * val, r_ * val, 255);
        return result;
    }

    pixel operator+(const pixel& rhs)
    {
        pixel result (b_ + rhs.b_, g_ + rhs.g_, r_ + rhs.r_, 255);
        return result;
    }


    unsigned char b_, g_, r_, t_;
};

const pixel white;

class point
{
public:
    point():
    x(0.0), y(0.0), z(0.0)
    {};

    explicit point(double x_, double y_, double z_, pixel color = white):
    x(x_), y(y_), z(z_), colour(color)
    {};

    explicit point(const point& value):
    x(value.x), y(value.y), z(value.z), colour(value.colour)
    {};
    
    void norm()
    {
        if (z >= coef || coef == 0)
        {
            x = 0;
            y = 0;
        }
        else
        {
            x = x / (1 - z / coef);
            y = y / (1 - z / coef);
        }

        x = (x + 1) * size_x / 2;
        y = (y + 1) * size_y / 2;

        /*if (x >= size_x || x <= 0)
        {
            x = size_x / 2;
        }

        if (y >= size_y || y <= 0)
        {
            y = size_y / 2;
        }*/
    }

    void rotate(double tetta)
    {
        double oldx = x, oldy = y;
        x = oldx * std::cos(tetta) - oldy * std::sin(tetta);
        y = oldx * std::sin(tetta) + oldy * std::cos(tetta);
    }


    double x, y, z;
    pixel colour;
};

std::ostream& operator<<(std::ostream& stream, const point& value)
{
    stream << '{' << value.x << ' ' << value.y << ' ' << value.z << '}';
    return stream;
}

class line
{
public:
    line(const point& first, const point& second, pixel colour = white):
    first_(first),
    second_(second),
    color_(colour)
    {};

    void draw(pixel* buffer, double tetta)
    {
        point first_temp(first_), second_temp(second_);
        first_temp.norm();
        second_temp.norm();
        first_temp.rotate(tetta);
        second_temp.rotate(tetta);

        int x0 = first_temp.x, x1 = second_temp.x, y0 = first_temp.y, y1 = second_temp.y;
        bool steep = false;
        if (std::abs(x0-x1) < std::abs(y0-y1)) 
        {
            std::swap(x0, y0);
            std::swap(x1, y1);
            steep = true;
        }
        if (x0 > x1) 
        {
            std::swap(x0, x1);
            std::swap(y0, y1);
        }
        int dx = x1 - x0;
        int dy = y1 - y0;
        int derror2 = std::abs(dy) * 2;
        int error2 = 0;
        int y = y0;
        for (int x = x0; x <= x1; x++) 
        {
            if (steep) 
            {
                int k = 1440 * x + y;
                if (k < 0 || k >= size_y * size_x)
                    k = 0;

                buffer[k] = color_;
            } 
            else 
            {
                int k = 1440 * y + x;
                if (k < 0 || k >= size_y * size_x)
                    k = 0;

                buffer[k] = color_;
            }
            error2 += derror2;

            if (error2 > dx) 
            {
                y += (y1 > y0 ? 1: -1);
                error2 -= dx * 2;
            }
        }
    }

private:
    point first_, second_;
    pixel color_;
};

class triangle
{
public:
    triangle()
    {};

    explicit triangle (const point& first, const point& second, const point& third):
    first_(first),
    second_(second),
    third_(third)
    {};

    void drawlines (pixel* buffer, double tetta)
    {
        line line1(first_, second_);
        line line2(first_, third_);
        line line3(second_, third_);
        line1.draw(buffer, tetta);
        line2.draw(buffer, tetta);
        line3.draw(buffer, tetta);
    }

    void draw (pixel* buffer)
    {
        double x1 = first_.x, x2 = second_.x, x3 = third_.x, xmin = 0, xmax = 0;
        double y1 = first_.y, y2 = second_.y, y3 = third_.y, ymin = 0, ymax = 0;

        std::set<double> set1 {x1, x2, x3};
        std::set<double> set2 {y1, y2, y3};

        xmin = *set1.begin();
        auto tmp = set1.end();
        tmp--;
        xmax = *tmp;

        ymin = *set2.begin();
        tmp = set2.end();
        tmp--;
        ymax = *tmp;

        for (int i = ymin; i < ymax; i++)
        {
            for (int j = xmin; j < xmax; j++)
            {
                double dx1 = x2 - x1, dx2 = x3 - x1;
                double dy1 = y2 - y1, dy2 = y3 - y1;
                double dx = j - x1, dy = i - y1;

                double maindet = dx1 * dy2 - dy1 * dx2, det1 = dx* dy2 - dy * dx2, det2 = dx1 * dy - dy1 * dx;

                double b = det1 / maindet, c = det2 / maindet, a = (maindet - det1 - det2) / maindet;

                if (a > 0 && b > 0 && c > 0)
                {
                    int k = 1440 * i + j;
                    pixel mixed = first_.colour * a + second_.colour * b + third_.colour * c;
                    buffer[k] = mixed;
                }
            }
        }
    }

    point first_, second_, third_;
};

std::ostream& operator<<(std::ostream& stream, const triangle& value)
{
    stream << '{' << value.first_  << ' ' << value.second_  << ' ' << value.third_ << '}';
    return stream;
}