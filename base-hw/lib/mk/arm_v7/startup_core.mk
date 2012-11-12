#
# \brief  Essential platform specific sources for core
# \author Martin Stein
# \date   2012-04-16
#

# add C++ sources
SRC_CC += syscall.cc pager_support.cc

# add assembly sources
SRC_S += crt0.s mode_transition.s boot_modules.s

# add inlucde paths
INC_DIR += $(BASE_DIR)/src/core/include/

#
# Check if there are other images wich shall be linked to core.
# If not use a dummy boot-modules file wich includes only the symbols.
#
ifeq ($(wildcard $(BUILD_BASE_DIR)/boot_modules.s),)
vpath boot_modules.s $(REP_DIR)/src/core/arm
else
INC_DIR += $(BUILD_BASE_DIR)
vpath boot_modules.s $(BUILD_BASE_DIR)
endif

# declare remaining source paths
vpath syscall.cc        $(REP_DIR)/src/base/arm_v7a
vpath pager_support.cc  $(REP_DIR)/src/core/arm_v7
vpath mode_transition.s $(REP_DIR)/src/core/arm_v7
vpath crt0.s            $(REP_DIR)/src/core/arm
