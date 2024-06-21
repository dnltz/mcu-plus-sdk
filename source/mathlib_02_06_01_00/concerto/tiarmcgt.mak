# Copyright (C) 2013 Texas Instruments
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

ifndef TIARMCGT_ROOT
$(error You must define TIARMCGT_ROOT!)
endif

CC=$(TIARMCGT_ROOT)/bin/armcl
CP=$(TIARMCGT_ROOT)/bin/armcl
AS=$(TIARMCGT_ROOT)/bin/armcl
AR=$(TIARMCGT_ROOT)/bin/armar
LD=$(TIARMCGT_ROOT)/bin/armcl

ifdef LOGFILE
LOGGING:=&>$(LOGFILE)
else
LOGGING:=
endif

OBJ_EXT=obj
LIB_PRE=
LIB_EXT=lib
# if an object is encountered as dsmo, make it a library.
ifeq ($(strip $($(_MODULE)_TYPE)),dsmo)
$(_MODULE)_TYPE := library
endif
ifeq ($(strip $($(_MODULE)_TYPE)),library)
	BIN_PRE=
	BIN_EXT=.$(LIB_EXT)
else ifeq ($(strip $($(_MODULE)_TYPE)),exe)
	BIN_PRE=
	BIN_EXT=.out
endif

$(_MODULE)_BIN  := $(TDIR)/$(BIN_PRE)$(TARGET)$(BIN_EXT)
#If linker file does not exist inside the module directory
#then expect it to be generated by some other module in the output target directory
$(_MODULE)_DEPS := $(foreach linkf,$(LINKER_FILES),$(if $(wildcard $(SDIR)/$(linkf)),$(SDIR)/$(linkf),$(TDIR)/$(linkf)))
$(_MODULE)_OBJS := $(ASSEMBLY:%.S=$(ODIR)/%.$(OBJ_EXT)) $(CPPSOURCES:%.cpp=$(ODIR)/%.$(OBJ_EXT)) $(CSOURCES:%.c=$(ODIR)/%.$(OBJ_EXT))
# Redefine the local static libs and shared libs with REAL paths and pre/post-fixes
$(_MODULE)_STATIC_LIBS := $(foreach lib,$(STATIC_LIBS),$(TDIR)/$(LIB_PRE)$(lib).$(LIB_EXT))
$(_MODULE)_SHARED_LIBS := $(foreach lib,$(SHARED_LIBS),$(TDIR)/$(LIB_PRE)$(lib).$(LIB_EXT))
$(_MODULE)_COPT := --gcc
$(_MODULE)_IDIRS += $(TIARMCGT_ROOT)/include
$(_MODULE)_LDIRS += $(TIARMCGT_ROOT)/lib

# Ensure the compilation rules syntactically match the rules generated by the compiler
$(_MODULE)_OBJS := $(call PATH_CONV,$($(_MODULE)_OBJS))

ifeq ($(TARGET_BUILD),debug)
$(_MODULE)_COPT += --opt_level=0 -g
else ifeq ($(TARGET_BUILD),release)
$(_MODULE)_COPT += --opt_level=3 --gen_opt_info=2
endif

$(_MODULE)_COPT +=--endian=little --abi=eabi
ifeq ($(TARGET_CPU),ARM)
$(_MODULE)_COPT +=-mv=7A8 --float_support=vfpv3
else ifeq ($(TARGET_CPU),ARM_M3)
$(_MODULE)_COPT +=-mv=7M3
else ifeq ($(TARGET_CPU),ARM_M4)
$(_MODULE)_COPT +=-mv=7M4
endif

ifneq ($(MISRA_RULES),) # If module specifies rules,
$(_MODULE)_MISRA := --check_misra=$(MISRA_RULES)
else ifeq ($(CHECK_MISRA),1) # else, check the environment variable
$(_MODULE)_MISRA := --check_misra=required
endif

ifeq ($(KEEP_ASM),1)
$(_MODULE)_COPT += --keep_asm
endif

$(_MODULE)_MAP      := $($(_MODULE)_BIN).map
$(_MODULE)_INCLUDES := $(foreach inc,$($(_MODULE)_IDIRS),-I="$(basename $(inc))") $(foreach inc,$($(_MODULE)_SYSIDIRS),-I="$(basename $(inc))")
$(_MODULE)_DEFINES  := $(foreach def,$($(_MODULE)_DEFS),-D=$(def))
$(_MODULE)_LIBRARIES:= $(foreach ldir,$($(_MODULE)_LDIRS),--search_path="$(ldir)") $(foreach ldir,$($(_MODULE)_SYSLDIRS),--search_path="$(ldir)") $(foreach lib,$(STATIC_LIBS),--library=$(LIB_PRE)$(lib).$(LIB_EXT)) $(foreach lib,$(SYS_STATIC_LIBS),--library=$(LIB_PRE)$(lib).$(LIB_EXT)) $(foreach linkerf,$(LINKER_FILES),--library=$(linkerf))
$(_MODULE)_AFLAGS   := $($(_MODULE)_INCLUDES)
$(_MODULE)_CFLAGS   := $($(_MODULE)_INCLUDES) $($(_MODULE)_DEFINES) $($(_MODULE)_COPT) $(CFLAGS)
$(_MODULE)_LDFLAGS  := $($(_MODULE)_CFLAGS) -z --warn_sections --reread_libs --rom_model

###################################################
# COMMANDS
###################################################

$(_MODULE)_LINK_LIB   := $(call PATH_CONV,$(AR) ru2 $($(_MODULE)_BIN) $($(_MODULE)_OBJS) $($(_MODULE)_STATIC_LIBS))
$(_MODULE)_LINK_EXE   := $(call PATH_CONV,$(LD) $($(_MODULE)_LDFLAGS) $($(_MODULE)_OBJS) $($(_MODULE)_LIBRARIES) --output_file=$($(_MODULE)_BIN) --map_file=$($(_MODULE)_MAP))

###################################################
# MACROS FOR COMPILING
###################################################

define $(_MODULE)_COMPILE_TOOLS
$(ODIR)$(PATH_SEP)%.$(OBJ_EXT): $(SDIR)/%.c
	@echo [TIARM] Compiling C $$(notdir $$<)
	$(Q)$$(call PATH_CONV,$(CC) $($(_MODULE)_CFLAGS) --preproc_with_compile --preproc_dependency=$(ODIR)/$$*.dep -fr=$$(dir $$@) -fs=$$(dir $$@) -ft=$$(dir $$@) -eo=.$(OBJ_EXT) -fc=$$< $(LOGGING)) $($(_MODULE)_MISRA)

$(ODIR)$(PATH_SEP)%.$(OBJ_EXT): $(SDIR)/%.cpp
	@echo [TIARM] Compiling C++ $$(notdir $$<)
	$(Q)$$(call PATH_CONV,$(CP) $($(_MODULE)_CFLAGS) --preproc_with_compile --preproc_dependency=$(ODIR)/$$*.dep -fr=$$(dir $$@) -fs=$$(dir $$@) -ft=$$(dir $$@) -eo=.$(OBJ_EXT) -fp=$$< $(LOGGING))

$(ODIR)$(PATH_SEP)%.$(OBJ_EXT): $(SDIR)/%.asm
	@echo [TIARM] Assembling $$(notdir $$<)
	$(Q)$$(call PATH_CONV,$(AS) $($(_MODULE)_AFLAGS) --preproc_with_compile --preproc_dependency=$(ODIR)/$$*.dep -fr=$$(dir $$@) -ft=$$(dir $$@) -eo=.$(OBJ_EXT) -fa=$$< $(LOGGING))

endef

ifeq ($(strip $($(_MODULE)_TYPE)),library)

define $(_MODULE)_UNINSTALL
uninstall::
	@echo No uninstall step for static libraries
endef

define $(_MODULE)_INSTALL
install::
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
	@echo No dynamic objects are supported!
endef

define $(_MODULE)_INSTALL
install::
	@echo No dynamic objects are supported!
endef

define $(_MODULE)_BUILD
$($(_MODULE)_BIN): $($(_MODULE)_OBJS) $($(_MODULE)_STATIC_LIBS) $($(_MODULE)_SHARED_LIBS)
	@echo Linking $$@
	-$(Q)$(call $(_MODULE)_LINK_LIB) $(LOGGING)

build:: $($(_MODULE)_BIN)
endef

define $(_MODULE)_CLEAN_LNK
clean::
	@echo No dynamic objects are supported!
endef

else ifeq ($(strip $($(_MODULE)_TYPE)),exe)

define $(_MODULE)_UNINSTALL
uninstall::
	@echo No uninstall step for $(TARGET_CPU)
endef

define $(_MODULE)_INSTALL
install::
	@echo No install step for $(TARGET_CPU)
endef

define $(_MODULE)_BUILD
build:: $($(_MODULE)_BIN)
endef

define $(_MODULE)_CLEAN_LNK
clean::
endef

endif

