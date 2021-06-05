################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
source_user/%.obj: ../source_user/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccs930/ccs/tools/compiler/ti-cgt-c2000_18.12.5.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --fp_mode=relaxed --include_path="D:/ti/ccs930/ccs/tools/compiler/ti-cgt-c2000_18.12.5.LTS/include" --include_path="E:/2020/0 Graduation Project/0 Experiments/Code/TMS320F28379D/legacy mode/PMSM_FOC_F28379D_LEGACY_DELAY/include_user" --include_path="E:/2020/4 TMS320F28379D & TI/examples/device_support/f2837xd/common/include" --include_path="E:/2020/4 TMS320F28379D & TI/examples/device_support/f2837xd/headers/include" --include_path="E:/2020/4 TMS320F28379D & TI/examples/libraries/math/IQmath/c28/include" --include_path="E:/2020/4 TMS320F28379D & TI/examples/libraries/math/CLAmath/c28/include" --include_path="E:/2020/4 TMS320F28379D & TI/examples/libraries/math/FPUfastRTS/c28/include" --advice:performance=all --define=CPU1 --define=_LAUNCHXL_F28379D -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="source_user/$(basename $(<F)).d_raw" --obj_directory="source_user" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


