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
../DelayUs.asm \
../FFT32_calc.asm \
../FFT32_init.asm \
../FFT32_izero.asm \
../RFFT32_brev.asm \
../RFFT32_brev_RT.asm \
../RFFT32_mag.asm \
../RFFT32_split.asm \
../RFFT32_win.asm \
../iir5biq16.asm \
../sel_q.asm 

C_SRCS += \
../DeviceInit_18Nov2018.c \
../ELEX7820-DA-DacIsr\ template.c \
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
./ELEX7820-DA-DacIsr\ template.d \
./F2802x_GlobalVariableDefs.d \
./idle.d 

GEN_OPTS += \
./configPkg/compiler.opt 

OBJS += \
./DelayUs.obj \
./DeviceInit_18Nov2018.obj \
./ELEX7820-DA-DacIsr\ template.obj \
./F2802x_GlobalVariableDefs.obj \
./FFT32_calc.obj \
./FFT32_init.obj \
./FFT32_izero.obj \
./RFFT32_brev.obj \
./RFFT32_brev_RT.obj \
./RFFT32_mag.obj \
./RFFT32_split.obj \
./RFFT32_win.obj \
./idle.obj \
./iir5biq16.obj \
./sel_q.obj 

ASM_DEPS += \
./DelayUs.d \
./FFT32_calc.d \
./FFT32_init.d \
./FFT32_izero.d \
./RFFT32_brev.d \
./RFFT32_brev_RT.d \
./RFFT32_mag.d \
./RFFT32_split.d \
./RFFT32_win.d \
./iir5biq16.d \
./sel_q.d 

GEN_MISC_DIRS__QUOTED += \
"configPkg\" 

OBJS__QUOTED += \
"DelayUs.obj" \
"DeviceInit_18Nov2018.obj" \
"ELEX7820-DA-DacIsr template.obj" \
"F2802x_GlobalVariableDefs.obj" \
"FFT32_calc.obj" \
"FFT32_init.obj" \
"FFT32_izero.obj" \
"RFFT32_brev.obj" \
"RFFT32_brev_RT.obj" \
"RFFT32_mag.obj" \
"RFFT32_split.obj" \
"RFFT32_win.obj" \
"idle.obj" \
"iir5biq16.obj" \
"sel_q.obj" 

C_DEPS__QUOTED += \
"DeviceInit_18Nov2018.d" \
"ELEX7820-DA-DacIsr template.d" \
"F2802x_GlobalVariableDefs.d" \
"idle.d" 

GEN_FILES__QUOTED += \
"configPkg\linker.cmd" \
"configPkg\compiler.opt" 

ASM_DEPS__QUOTED += \
"DelayUs.d" \
"FFT32_calc.d" \
"FFT32_init.d" \
"FFT32_izero.d" \
"RFFT32_brev.d" \
"RFFT32_brev_RT.d" \
"RFFT32_mag.d" \
"RFFT32_split.d" \
"RFFT32_win.d" \
"iir5biq16.d" \
"sel_q.d" 

ASM_SRCS__QUOTED += \
"../DelayUs.asm" \
"../FFT32_calc.asm" \
"../FFT32_init.asm" \
"../FFT32_izero.asm" \
"../RFFT32_brev.asm" \
"../RFFT32_brev_RT.asm" \
"../RFFT32_mag.asm" \
"../RFFT32_split.asm" \
"../RFFT32_win.asm" \
"../iir5biq16.asm" \
"../sel_q.asm" 

C_SRCS__QUOTED += \
"../DeviceInit_18Nov2018.c" \
"../ELEX7820-DA-DacIsr template.c" \
"../F2802x_GlobalVariableDefs.c" \
"../idle.c" 


