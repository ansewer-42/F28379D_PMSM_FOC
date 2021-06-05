################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
common/F2837xD_Adc.obj: E:/2020/4\ TMS320F28379D\ &\ TI/examples/device_support/f2837xd/common/source/F2837xD_Adc.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccs930/ccs/tools/compiler/ti-cgt-c2000_18.12.5.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --fp_mode=relaxed --include_path="D:/ti/ccs930/ccs/tools/compiler/ti-cgt-c2000_18.12.5.LTS/include" --include_path="E:/2020/0 Graduation Project/0 Experiments/Code/TMS320F28379D/legacy mode/PMSM_FOC_F28379D_LEGACY_DELAY/include_user" --include_path="E:/2020/4 TMS320F28379D & TI/examples/device_support/f2837xd/common/include" --include_path="E:/2020/4 TMS320F28379D & TI/examples/device_support/f2837xd/headers/include" --include_path="E:/2020/4 TMS320F28379D & TI/examples/libraries/math/IQmath/c28/include" --include_path="E:/2020/4 TMS320F28379D & TI/examples/libraries/math/CLAmath/c28/include" --include_path="E:/2020/4 TMS320F28379D & TI/examples/libraries/math/FPUfastRTS/c28/include" --advice:performance=all --define=CPU1 --define=_LAUNCHXL_F28379D -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="common/F2837xD_Adc.d_raw" --obj_directory="common" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

common/F2837xD_CodeStartBranch.obj: E:/2020/4\ TMS320F28379D\ &\ TI/examples/device_support/f2837xd/common/source/F2837xD_CodeStartBranch.asm $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccs930/ccs/tools/compiler/ti-cgt-c2000_18.12.5.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --fp_mode=relaxed --include_path="D:/ti/ccs930/ccs/tools/compiler/ti-cgt-c2000_18.12.5.LTS/include" --include_path="E:/2020/0 Graduation Project/0 Experiments/Code/TMS320F28379D/legacy mode/PMSM_FOC_F28379D_LEGACY_DELAY/include_user" --include_path="E:/2020/4 TMS320F28379D & TI/examples/device_support/f2837xd/common/include" --include_path="E:/2020/4 TMS320F28379D & TI/examples/device_support/f2837xd/headers/include" --include_path="E:/2020/4 TMS320F28379D & TI/examples/libraries/math/IQmath/c28/include" --include_path="E:/2020/4 TMS320F28379D & TI/examples/libraries/math/CLAmath/c28/include" --include_path="E:/2020/4 TMS320F28379D & TI/examples/libraries/math/FPUfastRTS/c28/include" --advice:performance=all --define=CPU1 --define=_LAUNCHXL_F28379D -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="common/F2837xD_CodeStartBranch.d_raw" --obj_directory="common" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

common/F2837xD_CpuTimers.obj: E:/2020/4\ TMS320F28379D\ &\ TI/examples/device_support/f2837xd/common/source/F2837xD_CpuTimers.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccs930/ccs/tools/compiler/ti-cgt-c2000_18.12.5.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --fp_mode=relaxed --include_path="D:/ti/ccs930/ccs/tools/compiler/ti-cgt-c2000_18.12.5.LTS/include" --include_path="E:/2020/0 Graduation Project/0 Experiments/Code/TMS320F28379D/legacy mode/PMSM_FOC_F28379D_LEGACY_DELAY/include_user" --include_path="E:/2020/4 TMS320F28379D & TI/examples/device_support/f2837xd/common/include" --include_path="E:/2020/4 TMS320F28379D & TI/examples/device_support/f2837xd/headers/include" --include_path="E:/2020/4 TMS320F28379D & TI/examples/libraries/math/IQmath/c28/include" --include_path="E:/2020/4 TMS320F28379D & TI/examples/libraries/math/CLAmath/c28/include" --include_path="E:/2020/4 TMS320F28379D & TI/examples/libraries/math/FPUfastRTS/c28/include" --advice:performance=all --define=CPU1 --define=_LAUNCHXL_F28379D -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="common/F2837xD_CpuTimers.d_raw" --obj_directory="common" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

common/F2837xD_DefaultISR.obj: E:/2020/4\ TMS320F28379D\ &\ TI/examples/device_support/f2837xd/common/source/F2837xD_DefaultISR.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccs930/ccs/tools/compiler/ti-cgt-c2000_18.12.5.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --fp_mode=relaxed --include_path="D:/ti/ccs930/ccs/tools/compiler/ti-cgt-c2000_18.12.5.LTS/include" --include_path="E:/2020/0 Graduation Project/0 Experiments/Code/TMS320F28379D/legacy mode/PMSM_FOC_F28379D_LEGACY_DELAY/include_user" --include_path="E:/2020/4 TMS320F28379D & TI/examples/device_support/f2837xd/common/include" --include_path="E:/2020/4 TMS320F28379D & TI/examples/device_support/f2837xd/headers/include" --include_path="E:/2020/4 TMS320F28379D & TI/examples/libraries/math/IQmath/c28/include" --include_path="E:/2020/4 TMS320F28379D & TI/examples/libraries/math/CLAmath/c28/include" --include_path="E:/2020/4 TMS320F28379D & TI/examples/libraries/math/FPUfastRTS/c28/include" --advice:performance=all --define=CPU1 --define=_LAUNCHXL_F28379D -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="common/F2837xD_DefaultISR.d_raw" --obj_directory="common" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

common/F2837xD_EPwm.obj: E:/2020/4\ TMS320F28379D\ &\ TI/examples/device_support/f2837xd/common/source/F2837xD_EPwm.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccs930/ccs/tools/compiler/ti-cgt-c2000_18.12.5.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --fp_mode=relaxed --include_path="D:/ti/ccs930/ccs/tools/compiler/ti-cgt-c2000_18.12.5.LTS/include" --include_path="E:/2020/0 Graduation Project/0 Experiments/Code/TMS320F28379D/legacy mode/PMSM_FOC_F28379D_LEGACY_DELAY/include_user" --include_path="E:/2020/4 TMS320F28379D & TI/examples/device_support/f2837xd/common/include" --include_path="E:/2020/4 TMS320F28379D & TI/examples/device_support/f2837xd/headers/include" --include_path="E:/2020/4 TMS320F28379D & TI/examples/libraries/math/IQmath/c28/include" --include_path="E:/2020/4 TMS320F28379D & TI/examples/libraries/math/CLAmath/c28/include" --include_path="E:/2020/4 TMS320F28379D & TI/examples/libraries/math/FPUfastRTS/c28/include" --advice:performance=all --define=CPU1 --define=_LAUNCHXL_F28379D -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="common/F2837xD_EPwm.d_raw" --obj_directory="common" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

common/F2837xD_GlobalVariableDefs.obj: E:/2020/4\ TMS320F28379D\ &\ TI/examples/device_support/f2837xd/headers/source/F2837xD_GlobalVariableDefs.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccs930/ccs/tools/compiler/ti-cgt-c2000_18.12.5.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --fp_mode=relaxed --include_path="D:/ti/ccs930/ccs/tools/compiler/ti-cgt-c2000_18.12.5.LTS/include" --include_path="E:/2020/0 Graduation Project/0 Experiments/Code/TMS320F28379D/legacy mode/PMSM_FOC_F28379D_LEGACY_DELAY/include_user" --include_path="E:/2020/4 TMS320F28379D & TI/examples/device_support/f2837xd/common/include" --include_path="E:/2020/4 TMS320F28379D & TI/examples/device_support/f2837xd/headers/include" --include_path="E:/2020/4 TMS320F28379D & TI/examples/libraries/math/IQmath/c28/include" --include_path="E:/2020/4 TMS320F28379D & TI/examples/libraries/math/CLAmath/c28/include" --include_path="E:/2020/4 TMS320F28379D & TI/examples/libraries/math/FPUfastRTS/c28/include" --advice:performance=all --define=CPU1 --define=_LAUNCHXL_F28379D -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="common/F2837xD_GlobalVariableDefs.d_raw" --obj_directory="common" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

common/F2837xD_Gpio.obj: E:/2020/4\ TMS320F28379D\ &\ TI/examples/device_support/f2837xd/common/source/F2837xD_Gpio.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccs930/ccs/tools/compiler/ti-cgt-c2000_18.12.5.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --fp_mode=relaxed --include_path="D:/ti/ccs930/ccs/tools/compiler/ti-cgt-c2000_18.12.5.LTS/include" --include_path="E:/2020/0 Graduation Project/0 Experiments/Code/TMS320F28379D/legacy mode/PMSM_FOC_F28379D_LEGACY_DELAY/include_user" --include_path="E:/2020/4 TMS320F28379D & TI/examples/device_support/f2837xd/common/include" --include_path="E:/2020/4 TMS320F28379D & TI/examples/device_support/f2837xd/headers/include" --include_path="E:/2020/4 TMS320F28379D & TI/examples/libraries/math/IQmath/c28/include" --include_path="E:/2020/4 TMS320F28379D & TI/examples/libraries/math/CLAmath/c28/include" --include_path="E:/2020/4 TMS320F28379D & TI/examples/libraries/math/FPUfastRTS/c28/include" --advice:performance=all --define=CPU1 --define=_LAUNCHXL_F28379D -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="common/F2837xD_Gpio.d_raw" --obj_directory="common" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

common/F2837xD_PieCtrl.obj: E:/2020/4\ TMS320F28379D\ &\ TI/examples/device_support/f2837xd/common/source/F2837xD_PieCtrl.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccs930/ccs/tools/compiler/ti-cgt-c2000_18.12.5.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --fp_mode=relaxed --include_path="D:/ti/ccs930/ccs/tools/compiler/ti-cgt-c2000_18.12.5.LTS/include" --include_path="E:/2020/0 Graduation Project/0 Experiments/Code/TMS320F28379D/legacy mode/PMSM_FOC_F28379D_LEGACY_DELAY/include_user" --include_path="E:/2020/4 TMS320F28379D & TI/examples/device_support/f2837xd/common/include" --include_path="E:/2020/4 TMS320F28379D & TI/examples/device_support/f2837xd/headers/include" --include_path="E:/2020/4 TMS320F28379D & TI/examples/libraries/math/IQmath/c28/include" --include_path="E:/2020/4 TMS320F28379D & TI/examples/libraries/math/CLAmath/c28/include" --include_path="E:/2020/4 TMS320F28379D & TI/examples/libraries/math/FPUfastRTS/c28/include" --advice:performance=all --define=CPU1 --define=_LAUNCHXL_F28379D -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="common/F2837xD_PieCtrl.d_raw" --obj_directory="common" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

common/F2837xD_PieVect.obj: E:/2020/4\ TMS320F28379D\ &\ TI/examples/device_support/f2837xd/common/source/F2837xD_PieVect.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccs930/ccs/tools/compiler/ti-cgt-c2000_18.12.5.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --fp_mode=relaxed --include_path="D:/ti/ccs930/ccs/tools/compiler/ti-cgt-c2000_18.12.5.LTS/include" --include_path="E:/2020/0 Graduation Project/0 Experiments/Code/TMS320F28379D/legacy mode/PMSM_FOC_F28379D_LEGACY_DELAY/include_user" --include_path="E:/2020/4 TMS320F28379D & TI/examples/device_support/f2837xd/common/include" --include_path="E:/2020/4 TMS320F28379D & TI/examples/device_support/f2837xd/headers/include" --include_path="E:/2020/4 TMS320F28379D & TI/examples/libraries/math/IQmath/c28/include" --include_path="E:/2020/4 TMS320F28379D & TI/examples/libraries/math/CLAmath/c28/include" --include_path="E:/2020/4 TMS320F28379D & TI/examples/libraries/math/FPUfastRTS/c28/include" --advice:performance=all --define=CPU1 --define=_LAUNCHXL_F28379D -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="common/F2837xD_PieVect.d_raw" --obj_directory="common" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

common/F2837xD_SysCtrl.obj: E:/2020/4\ TMS320F28379D\ &\ TI/examples/device_support/f2837xd/common/source/F2837xD_SysCtrl.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccs930/ccs/tools/compiler/ti-cgt-c2000_18.12.5.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --fp_mode=relaxed --include_path="D:/ti/ccs930/ccs/tools/compiler/ti-cgt-c2000_18.12.5.LTS/include" --include_path="E:/2020/0 Graduation Project/0 Experiments/Code/TMS320F28379D/legacy mode/PMSM_FOC_F28379D_LEGACY_DELAY/include_user" --include_path="E:/2020/4 TMS320F28379D & TI/examples/device_support/f2837xd/common/include" --include_path="E:/2020/4 TMS320F28379D & TI/examples/device_support/f2837xd/headers/include" --include_path="E:/2020/4 TMS320F28379D & TI/examples/libraries/math/IQmath/c28/include" --include_path="E:/2020/4 TMS320F28379D & TI/examples/libraries/math/CLAmath/c28/include" --include_path="E:/2020/4 TMS320F28379D & TI/examples/libraries/math/FPUfastRTS/c28/include" --advice:performance=all --define=CPU1 --define=_LAUNCHXL_F28379D -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="common/F2837xD_SysCtrl.d_raw" --obj_directory="common" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

common/F2837xD_usDelay.obj: E:/2020/4\ TMS320F28379D\ &\ TI/examples/device_support/f2837xd/common/source/F2837xD_usDelay.asm $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccs930/ccs/tools/compiler/ti-cgt-c2000_18.12.5.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --fp_mode=relaxed --include_path="D:/ti/ccs930/ccs/tools/compiler/ti-cgt-c2000_18.12.5.LTS/include" --include_path="E:/2020/0 Graduation Project/0 Experiments/Code/TMS320F28379D/legacy mode/PMSM_FOC_F28379D_LEGACY_DELAY/include_user" --include_path="E:/2020/4 TMS320F28379D & TI/examples/device_support/f2837xd/common/include" --include_path="E:/2020/4 TMS320F28379D & TI/examples/device_support/f2837xd/headers/include" --include_path="E:/2020/4 TMS320F28379D & TI/examples/libraries/math/IQmath/c28/include" --include_path="E:/2020/4 TMS320F28379D & TI/examples/libraries/math/CLAmath/c28/include" --include_path="E:/2020/4 TMS320F28379D & TI/examples/libraries/math/FPUfastRTS/c28/include" --advice:performance=all --define=CPU1 --define=_LAUNCHXL_F28379D -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="common/F2837xD_usDelay.d_raw" --obj_directory="common" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


