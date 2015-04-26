#ifndef FIGURE_H
#define FIGURE_H


class Figure
{
public:
    Figure();
    virtual ~Figure();

    virtual double getPerimeter() = 0;
};

#endif // FIGURE_H
