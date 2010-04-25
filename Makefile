#******************************************************************************
#
# Makefile - Rules for building the binary merger creator.
# Author:         Paul Archer
# Creation Date:  August, 2008
#
# THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
# OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
# LMI SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
# CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
#
#******************************************************************************

# VERBOSE = 1
DEBUG = 1

BUILD = build

include Makedefs
include Makefile.list

.PHONY: default_target
default_target: all

################################
# Compiler Flags
################################

PRJ_CFLAGS = -Wall -O

ifdef DEBUG
 TARGET_OPTS = -O0 -g
 PRJ_CFLAGS += -DDEBUG
else
 TARGET_OPTS = -O2
 PRJ_CFLAGS += -DNDEBUG
endif

INCLUDE_DIRS = -I include

CFLAGS += $(TARGET_OPTS) $(INCLUDE_DIRS) $(PRJ_CFLAGS)

################################
# The default build
# ################################
all: fitdecode

################################
# HW features creator
################################
fitdecode: ${BUILD}/fitdecode

${BUILD}/fitdecode: $(OBJS)

################################
# The rule to clean out all the build products
# ################################

.PHONY: clean
clean:
	@rm -rf ${BUILD}

.PHONY: distclean
distclean: clean
	@rm tags

################################
# Rules to remake the c/etags
################################

.PHONY: cfile
cfile:
	@ find src     -name \*.\[ch\] >  cfile.list
	@ find include -name \*.\[ch\] >> cfile.list

.PHONY: tags
tags: cfile
	ctags -L cfile.list
	@ rm cfile.list

################################
# Include the automatically generated dependency files.
# ################################
-include $(wildcard $(BUILD)/objs/*.d)
