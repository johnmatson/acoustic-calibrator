################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.asm $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.8.LTS/bin/cl2000" -v28 -ml -mt --include_path="C:/Users/Alex/Documents/GitHub/acoustic-calibrator/audioCalibrator/acoustic_calibrator" --include_path="C:/ti/bios_6_76_04_02/packages/ti/posix/ccs" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.8.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.8.LTS/bin/cl2000" -v28 -ml -mt --include_path="C:/Users/Alex/Documents/GitHub/acoustic-calibrator/audioCalibrator/acoustic_calibrator" --include_path="C:/ti/bios_6_76_04_02/packages/ti/posix/ccs" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.8.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-1018767295:
	@$(MAKE) --no-print-directory -Onone -f subdir_rules.mk build-1018767295-inproc

build-1018767295-inproc: ../idle.cfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: XDCtools'
	"C:/ti/xdctools_3_51_01_18_core/xs" --xdcpath="C:/ti/bios_6_76_04_02/packages;" xdc.tools.configuro -o configPkg -t ti.targets.C28_large -p ti.platforms.tms320x28:TMS320F28027 -r release -c "C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.8.LTS" --compileOptions "-g" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

configPkg/linker.cmd: build-1018767295 ../idle.cfg
configPkg/compiler.opt: build-1018767295
configPkg/: build-1018767295


