# Copyright (C) 2017 Texas Instruments
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

ifeq ($(TARGET_CPU),$(HOST_CPU))
	CROSS_COMPILE:=
endif

ifeq ($(TARGET_CPU),X86)
	CROSS_COMPILE:=
endif

CC = $(CROSS_COMPILE)gcc-5
CP = $(CROSS_COMPILE)g++-5
AS = $(CROSS_COMPILE)as-5
AR = $(CROSS_COMPILE)gcc-ar-5
LD = $(CROSS_COMPILE)g++-5

ifdef LOGFILE
LOGGING:=&>$(LOGFILE)
else
LOGGING:=
endif

ifeq ($(TARGET_OS),DARWIN)
DSO_EXT := .dylib
else ifeq ($(TARGET_OS),CYGWIN)
DSO_EXT := .dll.a
else
DSO_EXT := .so
endif

ifeq ($(strip $($(_MODULE)_TYPE)),library)
	BIN_PRE=lib
	BIN_EXT=.a
else ifeq ($(strip $($(_MODULE)_TYPE)),dsmo)
	BIN_PRE=lib
	BIN_EXT=$(DSO_EXT)
else
	BIN_PRE=
	BIN_EXT=
endif

$(_MODULE)_DEFS += J7ES=1
$(_MODULE)_DEFS += J7AEP=2
$(_MODULE)_DEFS += J7AHP=3
$(_MODULE)_DEFS += AM62A=4
$(_MODULE)_DEFS += AM67A=5
ifeq ($(SOC),J7ES)
$(_MODULE)_DEFS += SOC=$(SOC)
else ifeq ($(SOC),J7AEP)
$(_MODULE)_DEFS += SOC=$(SOC)
else ifeq ($(SOC),J7AHP)
$(_MODULE)_DEFS += SOC=$(SOC)
else ifeq ($(SOC),AM62A)
$(_MODULE)_DEFS += SOC=$(SOC)
else ifeq ($(SOC),AM67A)
$(_MODULE)_DEFS += SOC=$(SOC)
else
$(_MODULE)_DEFS += SOC=J7ES
endif


$(_MODULE)_OUT  := $(dir $(TARGET))$(BIN_PRE)$(notdir $(TARGET))$(BIN_EXT)
$(_MODULE)_BIN  := $($(_MODULE)_TDIR)/$($(_MODULE)_OUT)
$(_MODULE)_OBJS := $(ASSEMBLY:%.S=$($(_MODULE)_ODIR)/%.o) $(CPPSOURCES:%.cpp=$($(_MODULE)_ODIR)/%.o) $(CSOURCES:%.c=$($(_MODULE)_ODIR)/%.o)
#$(_MODULE)_DEPS := $(foreach subdir,$(DIRECTORY),$($(_MODULE)_TDIR)/$(subdir))
$(_MODULE)_DEPS :=
# Redefine the local static libs and shared libs with REAL paths and pre/post-fixes
$(_MODULE)_STATIC_LIBS := $(foreach lib,$(STATIC_LIBS),$($(_MODULE)_TDIR)/$(dir $(lib))lib$(notdir $(lib)).a)
$(_MODULE)_SHARED_LIBS := $(foreach lib,$(SHARED_LIBS),$($(_MODULE)_TDIR)/lib$(lib)$(DSO_EXT))

#$(_MODULE)_COPT := $(CFLAGS)
#$(_MODULE)_LOPT := $(LDFLAGS)
ifneq ($(TARGET_OS),CYGWIN)
$(_MODULE)_COPT += -fPIC
endif
$(_MODULE)_COPT += -Wall -fms-extensions -Wno-write-strings -Wno-unknown-pragmas -fno-strict-aliasing

ifeq ($(TARGET_BUILD),debug)
$(_MODULE)_COPT += -O0 -g
$(_MODULE)_COPT += -D__C7X_UNSTABLE_API
#$(_MODULE)_LOPT += -g
else ifeq ($(TARGET_BUILD),release)
$(_MODULE)_COPT += -O3
$(_MODULE)_COPT += -D__C7X_UNSTABLE_API
$(_MODULE)_COPT += -g
endif

ifeq ($(TARGET_GCOV),1)
$(_MODULE)_COPT += -fprofile-arcs -ftest-coverage
endif

ifeq ($(TARGET_CPU),ARM)
$(_MODULE)_COPT += -mapcs -mno-sched-prolog -mno-thumb-interwork
ifeq ($(TARGET_OS),LINUX)
$(_MODULE)_COPT += -mabi=aapcs-linux
endif
endif

ifeq ($(TARGET_ARCH),32)
ifneq ($(TARGET_CPU),ARM)
$(_MODULE)_COPT += -m32 -fno-stack-protector
endif
endif

$(_MODULE)_MAP      := $($(_MODULE)_BIN).map
$(_MODULE)_INCLUDES := $(foreach inc,$($(_MODULE)_IDIRS),-I$(inc))
$(_MODULE)_DEFINES  := $(foreach def,$($(_MODULE)_DEFS),-D$(def))
$(_MODULE)_LIBRARIES:= $(foreach ldir,$($(_MODULE)_LDIRS),-L$(ldir)) $(foreach lib,$(STATIC_LIBS),-l$(notdir $(lib))) $(foreach lib,$(SYS_STATIC_LIBS),-l$(lib)) $(foreach lib,$(SHARED_LIBS),-l$(lib)) $(foreach lib,$(SYS_SHARED_LIBS),-l$(lib))
$(_MODULE)_AFLAGS   := $($(_MODULE)_INCLUDES)
ifeq ($(HOST_OS),DARWIN)
$(_MODULE)_LDFLAGS  := -arch $(TARGET_CPU) $(LDFLAGS)
endif
$(_MODULE)_LDFLAGS  += $($(_MODULE)_LOPT)
$(_MODULE)_CPLDFLAGS := $(foreach ldf,$($(_MODULE)_LDFLAGS),-Wl,$(ldf)) $($(_MODULE)_COPT)
$(_MODULE)_CFLAGS   := -c $($(_MODULE)_INCLUDES) $($(_MODULE)_DEFINES) $($(_MODULE)_COPT) $(CFLAGS)

ifdef DEBUG
$(_MODULE)_AFLAGS += --gdwarf-2
endif

###################################################
# COMMANDS
###################################################
ifneq ($(TARGET_OS),CYGWIN)
EXPORT_FLAG:=--export-dynamic
EXPORTER   :=-rdynamic
else
EXPORT_FLAG:=--export-all-symbols
EXPORTER   :=
endif

$(_MODULE)_ATTRIB_EXE := $(SET_EXEC) $($(_MODULE)_BIN)
$(_MODULE)_LN_DSO     := $(LINK) $($(_MODULE)_BIN).1.0 $($(_MODULE)_BIN)
$(_MODULE)_LN_INST_DSO:= $(LINK) $($(_MODULE)_INSTALL_LIB)/$($(_MODULE)_OUT).1.0 $($(_MODULE)_INSTALL_LIB)/$($(_MODULE)_OUT)
$(_MODULE)_UNLN_DSO   := $(CLEAN) $($(_MODULE)_BIN).1.0
$(_MODULE)_UNLN_INST_DSO:= $(CLEAN) $($(_MODULE)_INSTALL_LIB)/$($(_MODULE)_OUT).1.0
$(_MODULE)_INSTALL_DSO:= $(INSTALL) $($(_MODULE)_BIN).1.0 $($(_MODULE)_INSTALL_LIB)
$(_MODULE)_UNINSTALL_DSO:=$(CLEAN) $($(_MODULE)_INSTALL_LIB)/$($(_MODULE)_OUT)
$(_MODULE)_INSTALL_EXE:= $(INSTALL) $($(_MODULE)_BIN) $($(_MODULE)_INSTALL_BIN)
$(_MODULE)_UNINSTALL_EXE:=$(CLEAN) $($(_MODULE)_INSTALL_BIN)/$($(_MODULE)_OUT)
$(_MODULE)_LINK_LIB   := $(AR) -rscu $($(_MODULE)_BIN) $($(_MODULE)_OBJS) #$($(_MODULE)_STATIC_LIBS)
ifeq ($(TARGET_OS),DARWIN)
$(_MODULE)_LINK_DSO   := $(LD) -shared $($(_MODULE)_LDFLAGS) -all_load $($(_MODULE)_LIBRARIES) -lm -o $($(_MODULE)_BIN).1.0 $($(_MODULE)_OBJS)
$(_MODULE)_LINK_EXE   := $(LD) -rdynamic $($(_MODULE)_CPLDFLAGS) $($(_MODULE)_OBJS) $($(_MODULE)_LIBRARIES) -o $($(_MODULE)_BIN)
else
$(_MODULE)_LINK_DSO   := $(LD) $($(_MODULE)_LDFLAGS) -shared -Wl,$(EXPORT_FLAG) -Wl,-soname,$(notdir $($(_MODULE)_BIN)).1.0 $($(_MODULE)_OBJS) -Wl,--whole-archive $($(_MODULE)_LIBRARIES) -lm -Wl,--no-whole-archive -o $($(_MODULE)_BIN).1.0 -Wl,-Map=$($(_MODULE)_MAP)
$(_MODULE)_LINK_EXE   := $(LD) $(EXPORTER) -Wl,--cref $($(_MODULE)_CPLDFLAGS) $($(_MODULE)_OBJS) $($(_MODULE)_LIBRARIES) -o $($(_MODULE)_BIN) -Wl,-Map=$($(_MODULE)_MAP)
endif

###################################################
# MACROS FOR COMPILING
###################################################

ifeq ($(strip $($(_MODULE)_TYPE)),library)

define $(_MODULE)_UNINSTALL
uninstall::
	@echo No uninstall step for static libraries
endef

define $(_MODULE)_INSTALL
install:: $($(_MODULE)_BIN)
	@echo No install step for static libraries
endef

define $(_MODULE)_BUILD
build:: $($(_MODULE)_BIN)
endef

define $(_MODULE)_CLEAN_LNK
clean::
endef

else ifeq ($(strip $($(_MODULE)_TYPE)),dsmo)

define $(_MODULE)_UNINSTALL
uninstall::
	@echo Uninstalling $($(_MODULE)_BIN) from $($(_MODULE)_INSTALL_LIB)
	-$(Q)$(call $(_MODULE)_UNLN_INST_DSO)
	-$(Q)$(call $(_MODULE)_UNINSTALL_DSO)
endef

define $(_MODULE)_INSTALL
install:: $($(_MODULE)_BIN)
	@echo Installing $($(_MODULE)_BIN) to $($(_MODULE)_INSTALL_LIB)
	-$(Q)$(call $(_MODULE)_INSTALL_DSO)
	-$(Q)$(call $(_MODULE)_LN_INST_DSO)
endef

define $(_MODULE)_BUILD
build:: $($(_MODULE)_BIN)
endef

define $(_MODULE)_CLEAN_LNK
clean::
	@echo Removing Link for Shared Object $($(_MODULE)_BIN).1.0
	-$(Q)$(CLEAN) $($(_MODULE)_BIN).1.0
endef

else ifeq ($(strip $($(_MODULE)_TYPE)),exe)

define $(_MODULE)_UNINSTALL
uninstall::
	@echo Uninstalling $($(_MODULE)_BIN) from $($(_MODULE)_INSTALL_BIN)
	-$(Q)$(call $(_MODULE)_UNINSTALL_EXE)
endef

define $(_MODULE)_INSTALL
install:: $($(_MODULE)_BIN)
	@echo Installing $($(_MODULE)_BIN) to $($(_MODULE)_INSTALL_BIN)
	-$(Q)$(call $(_MODULE)_INSTALL_EXE)
	-$(Q)$(call $(_MODULE)_ATTRIB_EXE)
endef

define $(_MODULE)_BUILD
build:: $($(_MODULE)_BIN)
endef

define $(_MODULE)_CLEAN_LNK
clean::
endef

endif

define $(_MODULE)_COMPILE_TOOLS
$(ODIR)/%.o: $(SDIR)/%.c
	@echo [GCC] Compiling C++14 $$(notdir $$<)
	$(Q)$(CP) --std=c++14 -g $($(_MODULE)_CFLAGS) -MMD -MF $(ODIR)/$$*.dep -MT '$(ODIR)/$$*.o' $$< -o $$@ $(LOGGING)

$(ODIR)/%.o: $(SDIR)/%.cpp
	@echo [GCC] Compiling C++14 $$(notdir $$<)
	$(Q)$(CP) --std=c++14 -g $($(_MODULE)_CFLAGS) -MMD -MF $(ODIR)/$$*.dep -MT '$(ODIR)/$$*.o' $$< -o $$@ $(LOGGING)

$(ODIR)/%.o: $(SDIR)/%.S
	@echo [GCC] Assembling $$(notdir $$<)
	$(Q)$(AS) $($(_MODULE)_AFLAGS) -MD $(ODIR)/$$*.dep $$< -o $$@ $(LOGGING)
endef
