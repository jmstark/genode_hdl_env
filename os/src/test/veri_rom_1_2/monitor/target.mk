#
# \brief  Emulator of Milkymist module "monitor"
# \author Martin Stein
# \date   2012-12-04
#

# set program name
TARGET = monitor

# add verilog sources
SRC_VLG = monitor.v

# add C++ sources
SRC_CC += wiring.cc

# add library dependencies
LIBS += verilator_env

