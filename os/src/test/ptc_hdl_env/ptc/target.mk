#
# \brief  Verilog PTC
# \author Martin Stein
# \date   2012-12-04
#

# set program name
TARGET = test-ptc_hdl_env-ptc

# add verilog sources
SRC_VLG = ptc_top.v

# add C++ sources
SRC_CC = integration.cc

# add library dependencies
LIBS = verilator_env

