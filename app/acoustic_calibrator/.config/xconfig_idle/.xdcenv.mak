#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = C:/ti/bios_6_76_04_02/packages;C:/Users/Alex/Documents/GitHub/acoustic-calibrator/audioCalibrator/acoustic_calibrator/.config
override XDCROOT = C:/ti/xdctools_3_51_01_18_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = C:/ti/bios_6_76_04_02/packages;C:/Users/Alex/Documents/GitHub/acoustic-calibrator/audioCalibrator/acoustic_calibrator/.config;C:/ti/xdctools_3_51_01_18_core/packages;..
HOSTOS = Windows
endif
