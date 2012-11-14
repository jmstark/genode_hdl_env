#
# \brief  Test driver for a freehdl emulator
# \author Martin Stein
# \date   2012-05-04
#

# Set program name
TARGET = test-veri_rom_1_2

# Add C++ sources
SRC_CC += main.cc

# Add libraries
LIBS += cxx env

# decalre source paths
vpath % $(GEN_DIR)

