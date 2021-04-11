#include "mem/mjz/mjz_object.hh"

#include <iostream>

MjzObject::MjzObject(MjzObjectParams *params) :
    SimObject(params)
{
    std::cout << "Hello World! From a SimObject!" << std::endl;
}

MjzObject*
MjzObjectParams::create()
{
    return new MjzObject(this);
}
