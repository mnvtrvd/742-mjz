#include "mem/mjz/mjz_object.hh"
#include "debug/MjzDebug.hh"

MjzObject::MjzObject(MjzObjectParams *params) :
    SimObject(params)
{
    // std::cout << "Hello World! From a SimObject!" << std::endl;
    DPRINTF(MjzDebug, "See this debug output with --debug-flags=MjzDebug")
}

MjzObject*
MjzObjectParams::create()
{
    return new MjzObject(this);
}
