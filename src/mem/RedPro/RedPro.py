from m5.params import *
from MemObject import MemObject

class RedPro(MemObject):

    type = 'RedProObject'
    cxx_header = "mem/RedPro/redpro.hh"

    latency = Param.Latency("Time before firing the event")

    inst_port = SlavePort("CPU-side port")
    data_port = SlavePort("CPU-side port")

    mem_side = MasterPort("Mem side port")



