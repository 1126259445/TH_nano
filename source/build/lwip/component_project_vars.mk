# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += /home/xc/ESP/TH_nano/components/lwip/include/apps /home/xc/ESP/TH_nano/components/lwip/include/apps/sntp /home/xc/ESP/TH_nano/components/lwip/lwip/src/include /home/xc/ESP/TH_nano/components/lwip/port/esp8266/include /home/xc/ESP/TH_nano/components/lwip/port/esp8266/include/arch
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/lwip -llwip
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += /home/xc/ESP/TH_nano/components/lwip/lwip
COMPONENT_LIBRARIES += lwip
COMPONENT_LDFRAGMENTS += /home/xc/ESP/TH_nano/components/lwip/linker.lf
component-lwip-build: 
