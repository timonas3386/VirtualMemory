#include "Line.h"

Line::Line() : count(0) {}

void Line::write()
{
    count += 1;
}

unsigned long Line::read()
{
    return count;
}