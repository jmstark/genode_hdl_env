#
# \brief  Test driver for a freehdl emulator
# \author Martin Stein
# \date   2012-05-04
#

# Set program name
TARGET = test-hdl_env_8

# Add C++ sources
SRC_CC += main.cc

# Add libraries
LIBS += cxx env

# decalre source paths
vpath % $(GEN_DIR)

