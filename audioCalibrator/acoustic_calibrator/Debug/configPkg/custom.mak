## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: .libraries,28L linker.cmd package/cfg/idle_p28L.o28L

# To simplify configuro usage in makefiles:
#     o create a generic linker command file name 
#     o set modification times of compiler.opt* files to be greater than
#       or equal to the generated config header
#
linker.cmd: package/cfg/idle_p28L.xdl
	$(SED) 's"^\"\(package/cfg/idle_p28Lcfg.cmd\)\"$""\"C:/Users/Alex/Documents/GitHub/acoustic-calibrator/audioCalibrator/acoustic_calibrator/Debug/configPkg/\1\""' package/cfg/idle_p28L.xdl > $@
	-$(SETDATE) -r:max package/cfg/idle_p28L.h compiler.opt compiler.opt.defs
