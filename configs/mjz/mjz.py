import m5
from m5.objects import *

system = System()

# Set physical parameters of the system
system.clk_domain                = SrcClockDomain()
system.clk_domain.clock          = '1GHz'      # Can be whatever
system.clk_domain.voltage_domain = VoltageDomain()

system.mem_mode = 'timing'
system.mem_ranges = [AddrRange('512MB')]

# Choose CPU type
system.cpu = TimingSimpleCPU()

# Choose memory bus type
system.membus = SystemXBar()

# Connects the CPU to the memory bus
system.cpu.icache_port = system.membus.slave
system.cpu.dcache_port = system.membus.slave

# For X86:
# system.cpu.craeteInterruptController()
# system.cpu.interrupts[0].pio = system.membus.master
# system.cpu.interrupts[0].int_master = system.membus.slave
# system.cpu.interrupts[0].int_slave = system.membus.master


system.system_port = system.membus.slave

system.mem_ctrl       = DDR3_1600_8x8()
system.mem_ctrl.range = system.mem_ranges[0]
system.mem_ctrl.port  = system.membus.master

# Set up the program to execute
process             = Process()
process.cmd         = ['tests/tests-progs/hello/bin/riscv/linux/hello')
system.cpu.workload = processsystem.cpu.createThreads()

root = Root(full_system = False, system = system)
m5.instantiate()

print("Beginning simulation...")
exit_event = m5.simulate()
print("Exiting @ tick {} because {}".format(m5.curTick(), exit_event.getCause())


