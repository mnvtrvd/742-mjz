#ifndef __MJZ_OBJECT_HH__
#define __MJZ_OBJECT_HH__

#include "params/HelloObject.hh"
#include "sim/sim_object.hh"

class MjzObject : public SimObject
{
  public:
    MjzObject(MjzObjectParams *p);
};

#endif // __MJZ_OBJECT_HH__
