# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += /home/xc/ESP/TH_nano/components/newlib/include /home/xc/ESP/TH_nano/components/newlib/newlib/port/include /home/xc/ESP/TH_nano/components/newlib/newlib/include
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/newlib /home/xc/ESP/TH_nano/components/newlib/newlib/lib/libc_fnano.a /home/xc/ESP/TH_nano/components/newlib/newlib/lib/libm.a -lnewlib -u _printf_float -u _scanf_float
COMPONENT_LINKER_DEPS += /home/xc/ESP/TH_nano/components/newlib/newlib/lib/libc_fnano.a /home/xc/ESP/TH_nano/components/newlib/newlib/lib/libm.a
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += newlib
COMPONENT_LDFRAGMENTS += 
component-newlib-build: 
