SRC_CC  += main.cc
LIBS    += base alarm
INC_DIR += $(REP_DIR)/src/ptc_drivers/timer/include

vpath main.cc $(REP_DIR)/src/drivers/ptc_timer
