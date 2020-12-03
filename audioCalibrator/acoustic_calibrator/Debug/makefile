################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.8.LTS

GEN_OPTS__FLAG := --cmd_file="configPkg/compiler.opt"
GEN_CMDS__FLAG := -l"configPkg/linker.cmd"

ORDERED_OBJS += \
"./DelayUs.obj" \
"./DeviceInit_18Nov2018.obj" \
"./ELEX7820-DA-DacIsr template.obj" \
"./F2802x_GlobalVariableDefs.obj" \
"./FFT32_calc.obj" \
"./FFT32_init.obj" \
"./FFT32_izero.obj" \
"./RFFT32_brev.obj" \
"./RFFT32_brev_RT.obj" \
"./RFFT32_mag.obj" \
"./RFFT32_split.obj" \
"./RFFT32_win.obj" \
"./idle.obj" \
"./iir5biq16.obj" \
"./sel_q.obj" \
"../F2802x_Headers_BIOS.cmd" \
"../TMS320F28027.cmd" \
$(GEN_CMDS__FLAG) \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
acoustic_calibrator.out \

EXE_OUTPUTS__QUOTED += \
"acoustic_calibrator.out" \

BIN_OUTPUTS += \
acoustic_calibrator.hex \

BIN_OUTPUTS__QUOTED += \
"acoustic_calibrator.hex" \


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "acoustic_calibrator.out"

# Tool invocations
acoustic_calibrator.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: C2000 Linker'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.8.LTS/bin/cl2000" -v28 -ml -mt -g --diag_warning=225 --diag_wrap=off --display_error_number -z -m"acoustic_calibrator.map" --stack_size=0x300 --warn_sections -i"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.8.LTS/lib" -i"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.8.LTS/include" --reread_libs --diag_wrap=off --display_error_number --xml_link_info="acoustic_calibrator_linkInfo.xml" --rom_model -o "acoustic_calibrator.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

acoustic_calibrator.hex: $(EXE_OUTPUTS)
	@echo 'Building files: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo 'Invoking: C2000 Hex Utility'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.8.LTS/bin/hex2000"  -o "acoustic_calibrator.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(GEN_FILES__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "DelayUs.obj" "DeviceInit_18Nov2018.obj" "ELEX7820-DA-DacIsr template.obj" "F2802x_GlobalVariableDefs.obj" "FFT32_calc.obj" "FFT32_init.obj" "FFT32_izero.obj" "RFFT32_brev.obj" "RFFT32_brev_RT.obj" "RFFT32_mag.obj" "RFFT32_split.obj" "RFFT32_win.obj" "idle.obj" "iir5biq16.obj" "sel_q.obj" 
	-$(RM) "DeviceInit_18Nov2018.d" "ELEX7820-DA-DacIsr template.d" "F2802x_GlobalVariableDefs.d" "idle.d" 
	-$(RM) "DelayUs.d" "FFT32_calc.d" "FFT32_init.d" "FFT32_izero.d" "RFFT32_brev.d" "RFFT32_brev_RT.d" "RFFT32_mag.d" "RFFT32_split.d" "RFFT32_win.d" "iir5biq16.d" "sel_q.d" 
	-$(RMDIR) $(GEN_MISC_DIRS__QUOTED)
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

