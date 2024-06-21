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

# Add the paths from the makefile
$(_MODULE)_IDIRS += $(SYSIDIRS) $(IDIRS)
$(_MODULE)_LDIRS += $(SYSLDIRS) $(LDIRS)

# Add any additional libraries which are in this build system
$(_MODULE)_STATIC_LIBS += $(STATIC_LIBS)
$(_MODULE)_SHARED_LIBS += $(SHARED_LIBS)
$(_MODULE)_SYS_STATIC_LIBS += $(SYS_STATIC_LIBS)
$(_MODULE)_SYS_SHARED_LIBS += $(SYS_SHARED_LIBS)
$(_MODULE)_JAVA_LIBS += $(JAVA_LIBS)
$(_MODULE)_BINS += $(BINS)
$(_MODULE)_INCS += $(INCS)
ifdef INC_SUBPATH
$(_MODULE)_INC_SUBPATH := $(INC_SUBPATH)
else
$(_MODULE)_INC_SUBPATH := $(TARGET)
endif
$(_MODULE)_HEADERS := $(HEADERS)

# Copy over the rest of the variables
$(_MODULE)_TYPE := $(TARGETTYPE)
$(_MODULE)_DEFS := $(SYSDEFS) $(DEFS)
$(_MODULE)_TEST := $(TESTCASE)

# Set the Install Path
$(_MODULE)_INSTALL_PATH = $(INSTALL_PATH)

# For debugging the build system
$(_MODULE)_SRCS := $(CSOURCES) $(CPPSOURCES) $(ASSEMBLY) $(JSOURCES) $(KCSOURCES)

ifneq ($(SKIPBUILD),1)

NEEDS_COMPILER:=
ifeq ($($(_MODULE)_TYPE),library)
	NEEDS_COMPILER=1
else ifeq ($($(_MODULE)_TYPE),dsmo)
	NEEDS_COMPILER=1
else ifeq ($($(_MODULE)_TYPE),exe)
	NEEDS_COMPILER=1
else ifeq ($($(_MODULE)_TYPE),jar)
	NEEDS_COMPILER=1
endif

ifeq ($(NEEDS_COMPILER),1)

ifeq ($(HOST_COMPILER),GCC)
	include $(CONCERTO_ROOT)/gcc.mak
else ifeq ($(HOST_COMPILER),GPP)
	include $(CONCERTO_ROOT)/gpp5.mak
else ifeq ($(HOST_COMPILER),CLANG)
	include $(CONCERTO_ROOT)/clang.mak
else ifeq ($(HOST_COMPILER),CL)
	include $(CONCERTO_ROOT)/cl.mak
else ifeq ($(HOST_COMPILER),CGT6X)
	include $(CONCERTO_ROOT)/cgt6x.mak
else ifeq ($(HOST_COMPILER),CGT7X)
	include $(CONCERTO_ROOT)/cgt7x.mak
else ifeq ($(HOST_COMPILER),QCC)
	include $(CONCERTO_ROOT)/qcc.mak
else ifeq ($(HOST_COMPILER),TMS470)
	include $(CONCERTO_ROOT)/tms470.mak
else ifeq ($(HOST_COMPILER),TIARMCGT)
	include $(CONCERTO_ROOT)/tiarmcgt.mak
else ifeq ($(HOST_COMPILER),ARP32)
	include $(CONCERTO_ROOT)/arp32.mak
endif

include $(CONCERTO_ROOT)/java.mak

endif

include $(CONCERTO_ROOT)/opencl.mak
include $(CONCERTO_ROOT)/dpkg.mak
include $(CONCERTO_ROOT)/doxygen.mak

else

$(info Build Skipped for $(_MODULE):$(TARGET))

endif  # ifneq SKIPBUILD

$(_MODULE)_DEPS += $(foreach subdir,$(DIRECTORY),$(TDIR)/$(subdir).gitignore)

# Reset Skipbuild
SKIPBUILD:=

###################################################
# RULES
###################################################

$(_MODULE): $($(_MODULE)_BIN)

.PHONY: $(_MODULE)_test
$(_MODULE)_test: $($(_MODULE)_TEST) install
	$(Q)$($(_MODULE)_TEST)

ifeq ($(strip $($(_MODULE)_TYPE)),library)

define $(_MODULE)_BUILD_LIB
$($(_MODULE)_BIN): $($(_MODULE)_OBJS) $($(_MODULE)_STATIC_LIBS) $($(_MODULE)_DEPS)
	$(PRINT) Linking $$@
	-$(Q)$(call $(_MODULE)_LINK_LIB) $(LOGGING)
endef

$(eval $(call $(_MODULE)_BUILD_LIB))
$(eval $(call $(_MODULE)_INSTALL))
$(eval $(call $(_MODULE)_BUILD))
$(eval $(call $(_MODULE)_UNINSTALL))

else ifeq ($(strip $($(_MODULE)_TYPE)),exe)

define $(_MODULE)_BUILD_EXE
$($(_MODULE)_BIN): $($(_MODULE)_OBJS) $($(_MODULE)_STATIC_LIBS) $($(_MODULE)_SHARED_LIBS) $($(_MODULE)_DEPS)
	$(PRINT) Linking $$@
	-$(Q)$(call $(_MODULE)_LINK_EXE) $(LOGGING)
endef

$(eval $(call $(_MODULE)_BUILD_EXE))
$(eval $(call $(_MODULE)_INSTALL))
$(eval $(call $(_MODULE)_BUILD))
$(eval $(call $(_MODULE)_UNINSTALL))

else ifeq ($(strip $($(_MODULE)_TYPE)),dsmo)

define $(_MODULE)_BUILD_DSO
$($(_MODULE)_BIN): $($(_MODULE)_OBJS) $($(_MODULE)_STATIC_LIBS) $($(_MODULE)_SHARED_LIBS) $($(_MODULE)_DEPS)
	$(PRINT) Linking $$@
	$(Q)$(call $(_MODULE)_LINK_DSO) $(LOGGING)
	-$(Q)$(call $(_MODULE)_LN_DSO)
endef

$(eval $(call $(_MODULE)_BUILD_DSO))
$(eval $(call $(_MODULE)_INSTALL))
$(eval $(call $(_MODULE)_BUILD))
$(eval $(call $(_MODULE)_UNINSTALL))

else ifeq ($(strip $($(_MODULE)_TYPE)),objects)

$($(_MODULE)_BIN): $($(_MODULE)_OBJS)

else ifeq ($(strip $($(_MODULE)_TYPE)),prebuilt)

$(_MODULE)_BIN := $($(_MODULE)_TDIR)/$(TARGET)$(suffix $(PREBUILT))
build:: $($(_MODULE)_BIN)

#In Windows, copy does not update the timestamp, so we have to do an extra step below to update the timestamp
define $(_MODULE)_PREBUILT
$($(_MODULE)_BIN): $($(_MODULE)_SDIR)/$(1) $($(_MODULE)_TDIR)/.gitignore
	@echo Copying Prebuilt binary $($(_MODULE)_SDIR)/$(1) to $($(_MODULE)_BIN)
	-$(Q)$(COPY) $(call PATH_CONV,$($(_MODULE)_SDIR)/$(1) $($(_MODULE)_BIN))
ifeq ($(HOST_OS),Windows_NT)
	-$(Q)cd $($(_MODULE)_TDIR) && $(COPY) $(call PATH_CONV,$($(_MODULE)_BIN))+,,
endif

$(_MODULE)_CLEAN_LNK =

endef

$(eval $(call $(_MODULE)_PREBUILT,$(PREBUILT)))

else ifeq ($(strip $($(_MODULE)_TYPE)),jar)

$(eval $(call $(_MODULE)_DEPEND_JAR))

else ifeq ($(strip $($(_MODULE)_TYPE)),deb)

$(eval $(call $(_MODULE)_PACKAGE))

else ifeq ($(strip $($(_MODULE)_TYPE)),doxygen)

$(eval $(call $(_MODULE)_DOCUMENTS))

endif

define $(_MODULE)_CLEAN
.PHONY: clean_target clean $(_MODULE)_clean $(_MODULE)_clean_target
clean_target:: $(_MODULE)_clean_target
clean:: $(_MODULE)_clean

$(_MODULE)_clean_target:
	$(PRINT) Cleaning $(_MODULE) target $($(_MODULE)_BIN)
	-$(Q)$(CLEAN) $(call PATH_CONV,$($(_MODULE)_BIN))
ifneq ($($(_MODULE)_MAP),)
	$(PRINT) Cleaning $(_MODULE) target mapfile $($(_MODULE)_MAP)
	-$(Q)$(CLEAN) $(call PATH_CONV,$($(_MODULE)_MAP))
endif

$(_MODULE)_clean: $(_MODULE)_clean_target
	$(PRINT) Cleaning $($(_MODULE)_ODIR)
	-$(Q)$(CLEANDIR) $(call PATH_CONV,$($(_MODULE)_ODIR))

endef

# Include compiler generated dependency rules
# Second rule is to support module source sub-directories
define $(_MODULE)_DEPEND
-include $(1).dep
$(1)$(2): $(dir $(1)).gitignore
endef

$(foreach obj,$($(_MODULE)_OBJS),$(eval $(call $(_MODULE)_DEPEND,$(basename $(obj)),$(suffix $(obj)))))
$(eval $(call $(_MODULE)_CLEAN))
$(eval $(call $(_MODULE)_CLEAN_LNK))
$(eval $(call $(_MODULE)_COMPILE_TOOLS))

define $(_MODULE)_VARDEF
$(_MODULE)_vars::
	$(PRINT) =============================================
	$(PRINT) _MODULE=$(_MODULE)
	$(PRINT) $(_MODULE)_BIN =$($(_MODULE)_BIN)
	$(PRINT) $(_MODULE)_TYPE=$($(_MODULE)_TYPE)
	$(PRINT) $(_MODULE)_OBJS=$($(_MODULE)_OBJS)
	$(PRINT) $(_MODULE)_SDIR=$($(_MODULE)_SDIR)
	$(PRINT) $(_MODULE)_ODIR=$($(_MODULE)_ODIR)
	$(PRINT) $(_MODULE)_TDIR=$($(_MODULE)_TDIR)
	$(PRINT) $(_MODULE)_SRCS=$($(_MODULE)_SRCS)
	$(PRINT) $(_MODULE)_STATIC_LIBS=$($(_MODULE)_STATIC_LIBS)
	$(PRINT) $(_MODULE)_SHARED_LIBS=$($(_MODULE)_SHARED_LIBS)
	$(PRINT) $(_MODULE)_SYS_SHARED_LIBS=$($(_MODULE)_SYS_SHARED_LIBS)
	$(PRINT) $(_MODULE)_CLASSES=$($(_MODULES)_CLASSES)
	$(PRINT) =============================================
endef

$(eval $(call $(_MODULE)_VARDEF))

# Now clear out the module variable for repeat definitions
_MODULE :=
