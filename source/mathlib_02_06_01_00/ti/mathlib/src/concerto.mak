#******************************************************************************
#*      Copyright (C) 2016 Texas Instruments Incorporated.                    *
#*                      All Rights Reserved                                   *
#******************************************************************************

TEST_FILES := \
	TI_profile.c \
	TI_test.c \
	TI_memory.c \
	c71/TI_cache.c

# MATHLIB
MATHLIB_DEFS :=

# enable global MATHLIB check params if CHECKPARAMS is selected
ifeq ($(CHECKPARAMS),1)
MATHLIB_DEFS += MATHLIB_CHECK_PARAMS
endif

# enable MATHLIB_MESG debug messages whenever compiled in debug mode
ifeq ($(TARGET_BUILD),debug)
MATHLIB_DEFS += MATHLIB_MESSAGES
endif

ifneq ($(PERFORMANCE_TEST),)
MATHLIB_DEFS += PERFORMANCE_TEST=$(PERFORMANCE_TEST)
endif

ifneq ($(COMPILER_TEST),)
MATHLIB_DEFS += COMPILER_TEST=$(COMPILER_TEST)
endif

MATHLIB_DEFS += EXTENDED_TEST_CASE=$(EXTENDED_TEST_CASE)
ifneq ($(EXTENDED_TEST_CASE), -1)
MATHLIB_DEFS += TEST_CASE=$(EXTENDED_TEST_CASE)
else
MATHLIB_DEFS += TEST_CASE=$(TEST_CASE)
endif

MATHLIB_DEFS += TEST_CATEGORY=$(TEST_CATEGORY)

# if !defined(EXTENDED_TEST_CASE) && !defined(TEST_CASE) && !defined(TEST_CATEGORY) then set ALL_TEST_CASES
ifeq ($(EXTENDED_TEST_CASE), -1)
ifeq ($(TEST_CASE), -1)
ifeq ($(TEST_CATEGORY), -1)
MATHLIB_DEFS += ALL_TEST_CASES
endif
endif
endif

ifeq ($(NO_PRINTF), 1)
MATHLIB_DEFS += NO_PRINTF
endif

ifneq ($(FLAT_MEM),)
MATHLIB_DEFS += FLAT_MEM
endif

ifneq ($(MATHLIB_NUMBER_RANDOM_DIM_TESTS),)
MATHLIB_DEFS += MATHLIB_NUMBER_RANDOM_DIM_TESTS=$(MATHLIB_NUMBER_RANDOM_DIM_TESTS)
endif

ifneq ($(MATHLIB_DEBUGPRINT),)
MATHLIB_DEFS += MATHLIB_DEBUGPRINT=$(MATHLIB_DEBUGPRINT)
endif

ifneq ($(MATHLIB_SKIP_CI),)
MATHLIB_DEFS += MATHLIB_SKIP_CI=$(MATHLIB_SKIP_CI)
endif

ifneq ($(MATHLIB_SKIP_CN),)
MATHLIB_DEFS += MATHLIB_SKIP_CN=$(MATHLIB_SKIP_CN)
endif

ifneq ($(MATHLIB_SKIP_EXEC),)
MATHLIB_DEFS += MATHLIB_SKIP_CN=$(MATHLIB_SKIP_EXEC)
MATHLIB_DEFS += MATHLIB_SKIP_CI=$(MATHLIB_SKIP_EXEC)
endif

ifneq ($(FIXED_SEED),)
MATHLIB_DEFS += MATHLIB_FIXED_SEED=$(FIXED_SEED)
endif

# temporarily set compiler revision since compiler does not yet supply it for host emulation
# has the same format as __TI_COMPILER_VERSION__
# i.e. the 1.3.0 compiler is 001003000
ifneq ($(MATHLIB_COMPILER_VERSION),)
MATHLIB_DEFS += MATHLIB_COMPILER_VERSION=$(MATHLIB_COMPILER_VERSION)
else
MATHLIB_DEFS += MATHLIB_COMPILER_VERSION=0
endif

SUBMODULE_LIST :=

EXCLUSION_LIST := . common
SUBMODULE_LIST += math_c7x math_c7xmma

# things that shouldn't get built because there are known issues with this code
# EXCLUSION_LIST += linalg_c7xmma/MATHLIB_LINALG_matrixMatrixMultiply_ixX_ixX_oxX
# EXCLUSION_LIST += linalg_c7xmma/MATHLIB_LINALG_matrixMatrixMultiplyAccumulate_ixX_ixX_ixX_oxX
# EXCLUSION_LIST += linalg_c7xmma/MATHLIB_LINALG_matrixTranspose_ixX_oxX
# EXCLUSION_LIST += linalg_c7xmma/MATHLIB_LINALG_pointwiseMatrixMatrixMultiply_ixX_ixX_oxX
# EXCLUSION_LIST += fft_c7xmma/MATHLIB_FFT_dftLarge_ixX_cxX_oxX
# EXCLUSION_LIST += fft_c7xmma/MATHLIB_FFT_dftSmall_ixX_cxX_oxX
# EXCLUSION_LIST += fft_c7xmma/MATHLIB_FFT_fft_ixX_cxX_oxX
# EXCLUSION_LIST += fft_c7xmma/MATHLIB_FFT_highRadixDecompositions_ixX_cxX_oxX
# EXCLUSION_LIST += cnn_c7xmma/MATHLIB_CNN_convolve_row_ixX_ixX_oxX
# EXCLUSION_LIST += cnn_c7xmma/MATHLIB_CNN_deconvolve_row_ixX_ixX_oxX
# EXCLUSION_LIST += cnn_c7xmma/MATHLIB_CNN_fullyConnected_ixX_ixX_oxX
# EXCLUSION_LIST += cnn_c7xmma/MATHLIB_CNN_add_ixX_ixX_oxX
# EXCLUSION_LIST += dsp_c7xmma/MATHLIB_DSP_fir_ixX_ixX_oxX
# EXCLUSION_LIST += dsp_c7xmma/MATHLIB_DSP_firSmall_ixX_ixX_oxX
ifeq ($(TARGET_CPU),C7100)
EXCLUSION_LIST += cnn_c7xmma/MATHLIB_CNN_convolveBias_row_ixX_ixX_oxX
EXCLUSION_LIST += cnn_c7xmma/MATHLIB_CNN_fullyConnectedBias_ixX_ixX_oxX
EXCLUSION_LIST += cnn_c7xmma/MATHLIB_CNN_convolve_col_smallNo_legacy_ixX_ixX_oxX
EXCLUSION_LIST += cnn_c7xmma/MATHLIB_CNN_convolve_col_smallNo_highPrecision
EXCLUSION_LIST += cnn_c7xmma/MATHLIB_CNN_pixelShuffleBias_row_ixX_ixX_oxX

endif

ifeq ($(TARGET_SCPU),C7100)
EXCLUSION_LIST += cnn_c7xmma/MATHLIB_CNN_convolveBias_row_ixX_ixX_oxX
EXCLUSION_LIST += cnn_c7xmma/MATHLIB_CNN_fullyConnectedBias_ixX_ixX_oxX
EXCLUSION_LIST += cnn_c7xmma/MATHLIB_CNN_convolve_col_smallNo_legacy_ixX_ixX_oxX
EXCLUSION_LIST += cnn_c7xmma/MATHLIB_CNN_convolve_col_smallNo_highPrecision
EXCLUSION_LIST += cnn_c7xmma/MATHLIB_CNN_pixelShuffleBias_row_ixX_ixX_oxX
endif

# If compiling for C7x and C6X_MIGRATION is not defined
ifeq ($(TARGET_CPU)$(C6X_MIGRATION),C71)
TARGETPARENTFOLDER := c71
TARGETFOLDER := c71
else ifeq ($(TARGET_CPU)$(C6X_MIGRATION),C7100)
TARGETPARENTFOLDER := c71
TARGETFOLDER := c7100
else ifeq ($(TARGET_CPU)$(C6X_MIGRATION),C7120)
TARGETPARENTFOLDER := c71
TARGETFOLDER := c7120
else ifeq ($(TARGET_CPU)$(C6X_MIGRATION),C7502)
TARGETPARENTFOLDER := c71
TARGETFOLDER := c7502
else ifeq ($(TARGET_CPU)$(C6X_MIGRATION),C7504)
TARGETPARENTFOLDER := c71
TARGETFOLDER := c7504
else ifeq ($(TARGET_CPU)$(C6X_MIGRATION),C7508)
TARGETPARENTFOLDER := c71
TARGETFOLDER := c7508

# else if compiling host emulation mode for c7x
else ifeq ($(TARGET_SCPU),C71)
TARGETPARENTFOLDER := c71
TARGETFOLDER := c71
else ifeq ($(TARGET_SCPU),C7100)
TARGETPARENTFOLDER := c71
TARGETFOLDER := c7100
else ifeq ($(TARGET_SCPU),C7120)
TARGETPARENTFOLDER := c71
TARGETFOLDER := c7120
else ifeq ($(TARGET_SCPU),C7502)
TARGETPARENTFOLDER := c71
TARGETFOLDER := c7502
else ifeq ($(TARGET_SCPU),C7504)
TARGETPARENTFOLDER := c71
TARGETFOLDER := c7504
else ifeq ($(TARGET_SCPU),C7508)
TARGETPARENTFOLDER := c71
TARGETFOLDER := c7508


else

TARGETFOLDER := c66
EXCLUSION_LIST +=

endif
# end of ifeq ($(TARGET_CPU)$(C6X_MIGRATION),C71)
#$(info $$TARGETFOLDER was set to: [${TARGETFOLDER}])


ARCHIVE_LIST_CC := \

ARCHIVE_LIST_OPT := \

ARCHIVE_LIST_CN := \

# Common Utilities Library (needed for tests)
_MODULE=common
include $(PRELUDE)
TARGET = common_$(TARGET_CPU)
TARGETTYPE = library
CSOURCES := $(foreach file, $(COMMON_FILES), common/$(file))
#$(info TARGETFOLDER = $(TARGETFOLDER), TARGETPARENTFOLDER=$(TARGETPARENTFOLDER))
CSOURCES += $(call all-c-files-in,common/$(TARGETPARENTFOLDER))
CSOURCES += $(call all-c-files-in,common/$(TARGETFOLDER))
CPPSOURCES += $(call all-cpp-files-in,common/$(TARGETPARENTFOLDER))
CPPSOURCES += $(call all-cpp-files-in,common/$(TARGETFOLDER))
ASSEMBLY += $(call all-asm-files-in,common/$(TARGETPARENTFOLDER))
ASSEMBLY += $(call all-asm-files-in,common/$(TARGETFOLDER))
ifneq ($(TARGET_SCPU),)
EXCLUDE_FILE := common/c7100/MATHLIB_UTIL_mma_bist.asm
EXCLUDE_FILE += common/c7120/MATHLIB_UTIL_mma_bist.asm
ASSEMBLY := $(filter-out $(EXCLUDE_FILE), $(ASSEMBLY))
#ifdef SystemRoot
#CSOURCES := common/C71/MATHLIB_configurations.c
#endif
# Hack Alert:  SystemRoot is only defined in Windows build; not sure why this is necessary
ifdef SystemRoot
CSOURCES := common/$(TARGETPARENTFOLDER)/MATHLIB_configurations.c
CSOURCES += common/$(TARGETFOLDER)/MATHLIB_configurations.c
endif
endif
DEFS :=
ifneq ($(LINK_L2SRAM),)
DEFS += LINK_L2SRAM
endif
ifneq ($(HEAP_L2SRAM),)
DEFS += HEAP_L2SRAM
endif
ifneq ($(SIZE_L2CACHE),)
DEFS += SIZE_L2CACHE=$(SIZE_L2CACHE)
endif
#DEFS := DISABLE_STACK_PROFILING
DEFS += $(MATHLIB_DEFS)
include $(FINALE)


# Test Utilities Library (needed for tests)
_MODULE=test
include $(PRELUDE)
TARGET = test_$(TARGET_CPU)
TARGETTYPE = library
CSOURCES := $(foreach file, $(TEST_FILES), test/$(file))
CSOURCES += $(call all-c-files-in,test/$(TARGETPARENTFOLDER))
CPPSOURCES += $(call all-cpp-files-in,test/$(TARGETPARENTFOLDER))
ASSEMBLY += $(call all-asm-files-in,test/$(TARGETPARENTFOLDER))
CSOURCES += $(call all-c-files-in,test/$(TARGETFOLDER))
CPPSOURCES += $(call all-cpp-files-in,test/$(TARGETFOLDER))
ASSEMBLY += $(call all-asm-files-in,test/$(TARGETFOLDER))
ifneq ($(TARGET_SCPU),)
EXCLUDE_FILE := test/c71/MATHLIB_TEST_c7xecr.asm
EXCLUDE_FILE += test/c71/MATHLIB_TEST_c7xl2cc.asm
ASSEMBLY := $(filter-out $(EXCLUDE_FILE), $(ASSEMBLY))
# ifdef SystemRoot
# CSOURCES := common/C71/MATHLIB_configurations.c
# endif
# Hack Alert:  SystemRoot is only defined in Windows build; not sure why this is necessary
ifdef SystemRoot
CSOURCES := common/$(TARGETPARENTFOLDER)/MATHLIB_configurations.c
CSOURCES += common/$(TARGETFOLDER)/MATHLIB_configurations.c
endif
endif
DEFS :=
ifneq ($(LINK_L2SRAM),)
DEFS += LINK_L2SRAM
endif
ifneq ($(HEAP_L2SRAM),)
DEFS += HEAP_L2SRAM
endif
ifneq ($(SIZE_L2CACHE),)
DEFS += SIZE_L2CACHE=$(SIZE_L2CACHE)
endif
#DEFS := DISABLE_STACK_PROFILING
DEFS += $(MATHLIB_DEFS)
include $(FINALE)

#List all files and subdirectories and this level
KERNEL_DIRS := $(foreach submod, $(SUBMODULE_LIST), $(wildcard $(_MODPATH)/$(submod)/*/))
#Make the list relative
KERNEL_DIRS := $(subst $(_MODPATH)/,,$(KERNEL_DIRS))
#Only interested in directories
KERNEL_DIRS := $(dir $(KERNEL_DIRS))
#Remove trailing directory slashes
KERNEL_DIRS := $(patsubst %/,%,$(KERNEL_DIRS))
#Remove exclusion list directories
KERNEL_DIRS := $(filter-out $(EXCLUSION_LIST), $(KERNEL_DIRS))

$(foreach submod,$(SUBMODULE_LIST),$(eval $(call KERNEL_SUBMODULE_RULE,$(submod))))

#List all sub-categories of sources for building libraries (mathlib, mathlib_cn, mathlib_d)
CSOURCES_D  := $(foreach dir, $(KERNEL_DIRS), $(call all-type-files-in-this,*_d.c,$(dir)))
CSOURCES_D  += $(foreach dir, $(KERNEL_DIRS), $(call all-type-files-in-this,*_idat.c,$(dir)))
CSOURCES_CN := $(foreach dir, $(KERNEL_DIRS), $(call all-type-files-in-this,*_cn.c,$(dir)))
CPPSOURCES_D  := $(foreach dir, $(KERNEL_DIRS), $(call all-type-files-in-this,*_d.cpp,$(dir)))
CPPSOURCES_D  += $(foreach dir, $(KERNEL_DIRS), $(call all-type-files-in-this,*_idat.cpp,$(dir)))
CPPSOURCES_CN := $(foreach dir, $(KERNEL_DIRS), $(call all-type-files-in-this,*_cn.cpp,$(dir)))

#Populate LIBLIST which contains the kernel libraries which other .out files are dependent on
LIBLIST :=
$(foreach submod,$(SUBMODULE_LIST),$(eval include $(wildcard $(call PATH_CONV,$(SDIR)/$(submod)/concerto_inc.mak))))
LIBLIST := $(filter-out $(EXCLUSION_LIST), $(LIBLIST))

# OPTIMIZED Library
_MODULE=mathlib
include $(PRELUDE)
TARGET = mathlib_$(TARGET_CPU)
TARGETTYPE = library
# This generates library to match package.bld
# for every kernel directory, if the TARGETFOLDER is not empty, include files from the TARGETFOLDER else include the files from the TARGETPARENTFOLDER
CSOURCES := $(foreach dir, $(KERNEL_DIRS),  $(if $(wildcard $(call PATH_CONV,$(SDIR)/$(dir)/$(TARGETFOLDER))), $(call all-type-files-in-this,*.c,$(dir)/$(TARGETFOLDER)),  $(call all-type-files-in-this,*.c,$(dir)/$(TARGETPARENTFOLDER))))
CSOURCES += $(foreach dir, $(KERNEL_DIRS), $(call all-type-files-in-this,*.c,$(dir)))
CSOURCES := $(filter-out $(CSOURCES_D), $(CSOURCES))
CSOURCES := $(filter-out $(CSOURCES_CN), $(CSOURCES))
# for every kernel directory, if the TARGETFOLDER is not empty, include files from the TARGETFOLDER else include the files from the TARGETPARENTFOLDER
CPPSOURCES := $(foreach dir, $(KERNEL_DIRS),  $(if $(wildcard $(call PATH_CONV,$(SDIR)/$(dir)/$(TARGETFOLDER))), $(call all-type-files-in-this,*.cpp,$(dir)/$(TARGETFOLDER)),  $(call all-type-files-in-this,*.cpp,$(dir)/$(TARGETPARENTFOLDER))))
CPPSOURCES += $(foreach dir, $(KERNEL_DIRS), $(call all-type-files-in-this,*.cpp,$(dir)))
CPPSOURCES := $(filter-out $(CPPSOURCES_D), $(CPPSOURCES))
CPPSOURCES := $(filter-out $(CPPSOURCES_CN), $(CPPSOURCES))
DEFS := $(MATHLIB_DEFS)
#DEFS :=
#ifeq ($(TARGET_BUILD),debug)
# DEFS += MATHLIB_CHECK_PARAMS
#DEFS += MATHLIB_MESSAGES
#endif
#DEFS += $(MATHLIB_DEFS)
include $(FINALE)

# NATC Library
_MODULE=mathlib_cn
include $(PRELUDE)
TARGET = mathlib_cn_$(TARGET_CPU)
TARGETTYPE = library
# This generates library to match package.bld
CSOURCES := $(CSOURCES_CN)
#CPPSOURCES := $(CPPSOURCES_CN)
CPPSOURCES := $(foreach dir, $(KERNEL_DIRS), $(call all-type-files-in-this,*.cpp,$(dir)))
# This generates incomplete library for now to match package.bld
#CSOURCES := $(foreach dir, $(KERNEL_DIRS), $(dir)/$(dir)_cn.c)
#CSOURCES += $(ARCHIVE_LIST_CC) $(ARCHIVE_LIST_CN)
# This will generate full library after redundant object files are removed.
#CSOURCES := $(foreach dir, $(KERNEL_DIRS), $(call all-type-files-in-this,*_cn.c,$(dir)))
DEFS := $(MATHLIB_DEFS)
#DEFS :=
#ifeq ($(TARGET_BUILD),debug)
# DEFS += MATHLIB_CHECK_PARAMS
#DEFS += MATHLIB_MESSAGES
#endif
#DEFS += $(MATHLIB_DEFS)
include $(FINALE)

# Test Files Library (not used in any rule except itself)
_MODULE=mathlib_d
include $(PRELUDE)
TARGET = mathlib_d_$(TARGET_CPU)
TARGETTYPE = library
CSOURCES := $(CSOURCES_D)
CPPSOURCES := $(CPPSOURCES_D)
#CSOURCES := $(foreach dir, $(KERNEL_DIRS), $(call all-type-files-in-this,*_d.c,$(dir)))
#CSOURCES += $(foreach dir, $(KERNEL_DIRS), $(call all-type-files-in-this,*_idat.c,$(dir)))
DEFS := __ONESHOTTEST
DEFS += $(MATHLIB_DEFS)
include $(FINALE)

# The remainder of this file file is concerned with generating individual kernel
#   unit tests.

# TEST_MODULE_RULE macro for compiling the unit tests for each kernel
define TEST_MODULE_RULE
# create variable for checking whether the target folder exists
TARGETFOLDER_E :=$(wildcard $(call PATH_CONV,$(SDIR)/$(1)/$(TARGETFOLDER)))
# if the target folder does not exist, does a parent device folder exist
ifeq ($$(TARGETFOLDER_E),)
TARGETPARENTFOLDER_E :=$(wildcard $(call PATH_CONV,$(SDIR)/$(1)/$(TARGETPARENTFOLDER)))
endif
# woohoo!! tricky makefile logic to check to see if two variables are empty
ifneq ($$(TARGETFOLDER_E)$$(TARGETPARENTFOLDER_E),)
_MODULE := $(1)
include $(PRELUDE)
TARGET = $$(_MODULE)_$(TARGET_CPU)
TARGETTYPE = exe
STATIC_LIBS :=
include $(wildcard $(call PATH_CONV,$(SDIR)/$(1)/concerto_inc.mak))
STATIC_LIBS += common_$(TARGET_CPU)
STATIC_LIBS += test_$(TARGET_CPU)
ifeq ($$(TARGET_SCPU),C66)
STATIC_LIBS += c6xsim_$(TARGET_CPU)_$(TARGET_SCPU)
endif
ifeq ($(TARGET_SCPU), C7120)
SYS_STATIC_LIBS += C7120-host-emulation
else ifeq ($(TARGET_SCPU), C7502)
SYS_STATIC_LIBS += C7502-host-emulation
else ifeq ($(TARGET_SCPU), C7504)
SYS_STATIC_LIBS += C7504-host-emulation
else ifeq ($(TARGET_SCPU), C7508)
SYS_STATIC_LIBS += C7508-host-emulation
# need this backwards looking construct to prevent a true result when TARGET_SCPU is blank
else ifneq (,$(filter $(TARGET_SCPU),C7100))
SYS_STATIC_LIBS += C7100-host-emulation
endif

CSOURCES := $(call all-type-files-in-this,*.c,$(1))
CPPSOURCES := $(call all-type-files-in-this,*.cpp,$(1))
# if the target folder exists, use it
ifneq ($$(TARGETFOLDER_E),)
CSOURCES += $(call all-c-files-in,$(1)/$(TARGETFOLDER))
CPPSOURCES += $(call all-cpp-files-in,$(1)/$(TARGETFOLDER))
# if the target folder does not exist, use the target parent folder if it exists
else ifneq ($$(TARGETPARENTFOLDER_E),)
CSOURCES += $(call all-c-files-in,$(1)/$(TARGETPARENTFOLDER))
CPPSOURCES += $(call all-cpp-files-in,$(1)/$(TARGETPARENTFOLDER))
endif
LINKER_FILES := lnk.cmd
DIRECTORY := $(dir $(1))
LDIRS+=$(TARGET_OUT)/$(dir $(1))
DEFS := $(MATHLIB_DEFS)
#ifeq ($(TARGET_BUILD),debug)
# DEFS += MATHLIB_CHECK_PARAMS
#DEFS += MATHLIB_MESSAGES
#endif
include $(FINALE)
endif
endef

# TEST_MODULE_RULE_FROM_LIBS macro for compiling the unit tests for each kernel
define TEST_MODULE_RULE_FROM_LIBS
# create variable for checking whether the target folder exists
TARGETFOLDER_E :=$(wildcard $(call PATH_CONV,$(SDIR)/$(1)/$(TARGETFOLDER)))
# if the target folder does not exist, does a parent device folder exist
ifeq ($$(TARGETFOLDER_E),)
TARGETPARENTFOLDER_E :=$(wildcard $(call PATH_CONV,$(SDIR)/$(1)/$(TARGETPARENTFOLDER)))
endif
# woohoo!! tricky makefile logic to check to see if two variables are empty
ifneq ($$(TARGETFOLDER_E)$$(TARGETPARENTFOLDER_E),)
_MODULE := $(1)
include $(PRELUDE)
TARGET = $$(_MODULE)_$(TARGET_CPU)
TARGETTYPE = exe
STATIC_LIBS :=
STATIC_LIBS += mathlib_$(TARGET_CPU) mathlib_cn_$(TARGET_CPU)
# TEMPORARY DEFINITION TO BUILD OPTIMIZED LIBRARY WHEN NOT IN HOST EMULATION MODE
# STATIC_LIBS += mathlib_cn_$(TARGET_CPU)
ifeq ($$(TARGET_SCPU),)
STATIC_LIBS += mathlib_$(TARGET_CPU)
endif
STATIC_LIBS += mathlib_cn_$(TARGET_CPU)
ifeq ($(TARGET_SCPU), C7120)
SYS_STATIC_LIBS += C7120-host-emulation
else ifeq ($(TARGET_SCPU), C7502)
SYS_STATIC_LIBS += C7502-host-emulation
else ifeq ($(TARGET_SCPU), C7504)
SYS_STATIC_LIBS += C7504-host-emulation
else ifeq ($(TARGET_SCPU), C7508)
SYS_STATIC_LIBS += C7508-host-emulation
# need this backwards looking construct to prevent a true result when TARGET_SCPU is blank
else ifneq (,$(filter $(TARGET_SCPU),C7100))
SYS_STATIC_LIBS += C7100-host-emulation
endif
STATIC_LIBS += common_$(TARGET_CPU)
STATIC_LIBS += test_$(TARGET_CPU)
CSOURCES := $(call all-type-files-in-this,*_d.c,$(1))
CSOURCES += $(call all-type-files-in-this,*_idat.c,$(1))
CPPSOURCES := $(call all-type-files-in-this,*_d.cpp,$(1))
CPPSOURCES += $(call all-type-files-in-this,*_idat.cpp,$(1))
DIRECTORY := $(dir $(1))
LINKER_FILES := lnk.cmd
DIRECTORY := $(dir $(1))
DEFS := $(MATHLIB_DEFS)
ifneq ($$(LINK_L2SRAM),)
DEFS += LINK_L2SRAM
endif
ifneq ($$(AUTOTEST),)
DEFS += AUTOTEST
endif
include $(FINALE)
endif
endef


# TEST_MODULE_RULE_FROM_PREBUILT_LIBS macro for compiling the unit tests for each kernel
define TEST_MODULE_RULE_FROM_PREBUILT_LIBS
# create variable for checking whether the test driver (<kernel>_d.c) file exists
TESTDRIVER_E :=$(wildcard $(call PATH_CONV,$(SDIR)/$(1)/*_d.c))
ifneq ($$(TESTDRIVER_E),)
_MODULE := $(1)
include $(PRELUDE)
TARGET = $$(_MODULE)_$(TARGET_CPU)
TARGETTYPE = exe
STATIC_LIBS :=
# PUT THE BELOW LINE BACK IN WHEN HOST EMULATION MODE SUPPORTS MMA
# STATIC_LIBS += mathlib_$(TARGET_CPU) mathlib_cn_$(TARGET_CPU)

ifneq ($(TARGET_SCPU),)
SYSLDIRS += $(HOST_ROOT)/lib/$(TARGET_SCPU)/$(TARGET_BUILD)/
else
SYSLDIRS += $(HOST_ROOT)/lib/$(TARGET_CPU)/$(TARGET_BUILD)/
endif

ifneq (,$(filter $(TARGET_SCPU),C7100 C7120 C7502 C7504 C7508))
SYS_STATIC_LIBS += mathlib_$(TARGET_CPU) mathlib_cn_$(TARGET_CPU) common_$(TARGET_CPU) test_$(TARGET_CPU)
else
SYS_STATIC_LIBS += mathlib_$(TARGET_CPU).lib mathlib_cn_$(TARGET_CPU).lib common_$(TARGET_CPU).lib test_$(TARGET_CPU).lib
endif
# ifeq ($$(TARGET_SCPU),)
# STATIC_LIBS += mathlib_$(TARGET_CPU)
# endif
# STATIC_LIBS += mathlib_cn_$(TARGET_CPU)
ifeq ($(TARGET_SCPU), C7120)
SYS_STATIC_LIBS += C7120-host-emulation
else ifeq ($(TARGET_SCPU), C7502)
SYS_STATIC_LIBS += C7502-host-emulation
else ifeq ($(TARGET_SCPU), C7504)
SYS_STATIC_LIBS += C7504-host-emulation
else ifeq ($(TARGET_SCPU), C7508)
SYS_STATIC_LIBS += C7508-host-emulation
# need this backwards looking construct to prevent a true result when TARGET_SCPU is blank
else ifneq (,$(filter $(TARGET_SCPU),C7100))
SYS_STATIC_LIBS += C7100-host-emulation
endif
# STATIC_LIBS += common_$(TARGET_CPU)
# STATIC_LIBS += test_$(TARGET_CPU)
CSOURCES := $(call all-type-files-in-this,*_d.c,$(1))
CSOURCES += $(call all-type-files-in-this,*_idat.c,$(1))
CPPSOURCES := $(call all-type-files-in-this,*_d.cpp,$(1))
CPPSOURCES += $(call all-type-files-in-this,*_idat.cpp,$(1))
DIRECTORY := $(dir $(1))
LINKER_FILES := lnk.cmd
DEFS := $(MATHLIB_DEFS)
ifneq ($$(LINK_L2SRAM),)
DEFS += LINK_L2SRAM
endif
ifneq ($$(LINK_LIBS),)
DEFS += LINK_LIBS
endif
include $(FINALE)
endif
endef

# # LIB_MODULE_RULE macro for compiling the dependent libs for each kernel
define LIB_MODULE_RULE
TARGETFOLDER_E :=$(wildcard $(call PATH_CONV,$(SDIR)/$(1)/$(TARGETFOLDER)))
ifeq ($$(TARGETFOLDER_E),)
TARGETPARENTFOLDER_E :=$(wildcard $(call PATH_CONV,$(SDIR)/$(1)/$(TARGETPARENTFOLDER)))
endif
# woohoo!! tricky makefile logic to check to see if two variables are empty
ifneq ($$(TARGETFOLDER_E)$$(TARGETPARENTFOLDER_E),)
_MODULE := $(1)_lib
include $(PRELUDE)
TARGET = $$(_MODULE)_$(TARGET_CPU)
TARGETTYPE = library
CSOURCES := $(call all-type-files-in-this,*.c,$(1))
CSOURCES += $(call all-c-files-in,$(1)/$(TARGETPARENTFOLDER))
CSOURCES += $(call all-c-files-in,$(1)/$(TARGETFOLDER))
CPPSOURCES := $(call all-type-files-in-this,*.cpp,$(1))
CPPSOURCES += $(call all-cpp-files-in,$(1)/$(TARGETPARENTFOLDER))
CPPSOURCES += $(call all-cpp-files-in,$(1)/$(TARGETFOLDER))
DIRECTORY := $(dir $(1))
DEFS := $(MATHLIB_DEFS)
#DEFS :=
#ifeq ($(TARGET_BUILD),debug)
# DEFS += MATHLIB_CHECK_PARAMS
#DEFS += MATHLIB_MESSAGES
#endif
include $(FINALE)
endif
endef

# TEST_MODULE_OVERRIDE_RULE macro for excluding making rules for kernels that have
#   concerto.mak already (so that the individual concerto.mak overrides this makefile
EXCLUSION_LIST :=
define TEST_MODULE_OVERRIDE_RULE
ifneq ($(wildcard $(call PATH_CONV,$(SDIR)/$(1)/concerto.mak)),)
EXCLUSION_LIST += $(1)
endif
endef

# Filter out making rules for directories which already have concerto.mak files
$(foreach d,$(KERNEL_DIRS),$(eval $(call TEST_MODULE_OVERRIDE_RULE,$(d))))
KERNEL_DIRS := $(filter-out $(EXCLUSION_LIST), $(KERNEL_DIRS))

# Insert rules to build all the kernel executables, and dependent libraries
ifneq ($(LINK_L2SRAM),)
$(foreach d,$(KERNEL_DIRS),$(eval $(call TEST_MODULE_RULE_FROM_LIBS,$(d))))
else ifneq ($(AUTOTEST),)
$(foreach d,$(KERNEL_DIRS),$(eval $(call TEST_MODULE_RULE_FROM_LIBS,$(d))))
else ifneq ($(LINK_LIBS),)
$(foreach d,$(KERNEL_DIRS),$(eval $(call TEST_MODULE_RULE_FROM_PREBUILT_LIBS,$(d))))
else
$(foreach d,$(KERNEL_DIRS),$(eval $(call TEST_MODULE_RULE,$(d))))
$(foreach d,$(LIBLIST),$(eval $(call LIB_MODULE_RULE,$(d))))
endif

TEST_MODULE_RULE :=
LIB_MODULE_RULE :=
TEST_MODULE_OVERRIDE_RULE :=
