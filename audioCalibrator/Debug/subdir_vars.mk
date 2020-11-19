################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
CFG_SRCS += \
../idle.cfg 

CMD_SRCS += \
../F2802x_Headers_BIOS.cmd \
../TMS320F28027.cmd 

ASM_SRCS += \
../DelayUs.asm 

C_SRCS += \
../DeviceInit_18Nov2018.c \
../F2802x_GlobalVariableDefs.c \
../idle.c 

GEN_CMDS += \
./configPkg/linker.cmd 

GEN_FILES += \
./configPkg/linker.cmd \
./configPkg/compiler.opt 

GEN_MISC_DIRS += \
./configPkg/ 

C_DEPS += \
./DeviceInit_18Nov2018.d \
./F2802x_GlobalVariableDefs.d \
./idle.d 

GEN_OPTS += \
./configPkg/compiler.opt 

OBJS += \
./DelayUs.obj \
./DeviceInit_18Nov2018.obj \
./F2802x_GlobalVariableDefs.obj \
./idle.obj 

ASM_DEPS += \
./DelayUs.d 

GEN_MISC_DIRS__QUOTED += \
"configPkg\" 

OBJS__QUOTED += \
"DelayUs.obj" \
"DeviceInit_18Nov2018.obj" \
"F2802x_GlobalVariableDefs.obj" \
"idle.obj" 

C_DEPS__QUOTED += \
"DeviceInit_18Nov2018.d" \
"F2802x_GlobalVariableDefs.d" \
"idle.d" 

GEN_FILES__QUOTED += \
"configPkg\linker.cmd" \
"configPkg\compiler.opt" 

ASM_DEPS__QUOTED += \
"DelayUs.d" 

ASM_SRCS__QUOTED += \
"../DelayUs.asm" 

C_SRCS__QUOTED += \
"../DeviceInit_18Nov2018.c" \
"../F2802x_GlobalVariableDefs.c" \
"../idle.c" 


