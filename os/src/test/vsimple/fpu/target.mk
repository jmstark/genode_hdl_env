#
# \brief  FPU emulator
# \author Martin Stein
# \date   2012-05-04
#

# set program name
TARGET = test-vsimple-fpu

# add C++ sources
SRC_CC += main.cc

# add include paths
INC_DIR += $(PRG_DIR)

# add library dependencies
LIBS += base

# declare source paths
vpath main.cc $(PRG_DIR)

