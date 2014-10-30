# This file is a makefile included from the top level makefile which
# defines the sources built for the target.

# Define the prefix to this directory. 
# Note: The name must be unique within this build and should be
#       based on the root of the project
HAL_SRC_COREV2_PATH = $(TARGET_HAL_PATH)/src/core-v2

# if we are being compiled with platform as a dependency, then also include
# implementation headers.
ifneq (,$(findstring platform,$(DEPENDENCIES)))
INCLUDE_DIRS += $(HAL_SRC_COREV2_PATH)
INCLUDE_DIRS += $(HAL_SRC_COREV2_PATH)/wiced/include
endif

HAL_LIB_COREV2 = $(HAL_SRC_COREV2_PATH)/lib

LIB_DIRS += $(HAL_LIB_COREV2)

LIBS += $(basename $(call rwildcard,$(HAL_LIB_COREV2)/,*.a))

LDFLAGS += -T$(COMMON_BUILD)/arm/linker/linker_$(STM32_DEVICE_LC)_dfu.ld
LDFLAGS += --specs=nano.specs -lc -lnosys
LDFLAGS += -u _printf_float

# not using assembler startup script, but will use startup linked in with wiced


