################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := D:/ti/ccs930/ccs/tools/compiler/ti-cgt-c2000_18.12.5.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./common/F2837xD_Adc.obj" \
"./common/F2837xD_CodeStartBranch.obj" \
"./common/F2837xD_CpuTimers.obj" \
"./common/F2837xD_DefaultISR.obj" \
"./common/F2837xD_EPwm.obj" \
"./common/F2837xD_GlobalVariableDefs.obj" \
"./common/F2837xD_Gpio.obj" \
"./common/F2837xD_PieCtrl.obj" \
"./common/F2837xD_PieVect.obj" \
"./common/F2837xD_SysCtrl.obj" \
"./common/F2837xD_usDelay.obj" \
"./source_user/ADC_Init.obj" \
"./source_user/Coordinate_Trans.obj" \
"./source_user/DAC_Init.obj" \
"./source_user/Device_Init.obj" \
"./source_user/Discrete_Domain_Controller.obj" \
"./source_user/EPWM_Init.obj" \
"./source_user/EQEP_Init.obj" \
"./source_user/Global_Functions_Variables_Call.obj" \
"./source_user/PID_Controller.obj" \
"./source_user/PMSM_FOC_Main.obj" \
"./source_user/Ramp_Function.obj" \
"./source_user/SVPWM_Gen.obj" \
"../cmd/2837xD_RAM_lnk_cpu1.cmd" \
"../cmd/F2837xD_Headers_nonBIOS_cpu1.cmd" \
$(GEN_CMDS__FLAG) \
-lrts2800_fpu32_fast_supplement.lib \
-lIQmath_fpu32.lib \
-lcla1_math_library_fpu32.lib \
-lfast_current_loop.lib \
-llibc.a \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include cmd/subdir_vars.mk
-include common/subdir_vars.mk
-include source_user/subdir_vars.mk
-include cmd/subdir_rules.mk
-include common/subdir_rules.mk
-include source_user/subdir_rules.mk
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
PMSM_FOC_F28379D_LEGACY_DELAY.out \

EXE_OUTPUTS__QUOTED += \
"PMSM_FOC_F28379D_LEGACY_DELAY.out" \

BIN_OUTPUTS += \
PMSM_FOC_F28379D_LEGACY_DELAY.hex \

BIN_OUTPUTS__QUOTED += \
"PMSM_FOC_F28379D_LEGACY_DELAY.hex" \


# All Target
all: PMSM_FOC_F28379D_LEGACY_DELAY.out

# Tool invocations
PMSM_FOC_F28379D_LEGACY_DELAY.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: C2000 Linker'
	"D:/ti/ccs930/ccs/tools/compiler/ti-cgt-c2000_18.12.5.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --fp_mode=relaxed --advice:performance=all --define=CPU1 --define=_LAUNCHXL_F28379D -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi -z -m"PMSM_FOC_F28379D_LEGACY_DELAY.map" --stack_size=0x200 --warn_sections -i"D:/ti/ccs930/ccs/tools/compiler/ti-cgt-c2000_18.12.5.LTS/lib" -i"D:/ti/ccs930/ccs/tools/compiler/ti-cgt-c2000_18.12.5.LTS/include" -i"E:/2020/4 TMS320F28379D & TI/examples/libraries/math/FPUfastRTS/c28/lib" -i"E:/2020/4 TMS320F28379D & TI/examples/libraries/math/IQmath/c28/lib" -i"E:/2020/4 TMS320F28379D & TI/examples/libraries/math/CLAmath/c28/lib" -i"E:/2020/4 TMS320F28379D & TI/examples/controlSUITE_app_libs/motor_control/libs/FCL/v02_00_00_00/lib" --reread_libs --diag_wrap=off --display_error_number --xml_link_info="PMSM_FOC_F28379D_LEGACY_DELAY_linkInfo.xml" --entry_point=_c_int00 --rom_model -o "PMSM_FOC_F28379D_LEGACY_DELAY.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

PMSM_FOC_F28379D_LEGACY_DELAY.hex: $(EXE_OUTPUTS)
	@echo 'Building files: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo 'Invoking: C2000 Hex Utility'
	"D:/ti/ccs930/ccs/tools/compiler/ti-cgt-c2000_18.12.5.LTS/bin/hex2000"  -o "PMSM_FOC_F28379D_LEGACY_DELAY.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "common\F2837xD_Adc.obj" "common\F2837xD_CodeStartBranch.obj" "common\F2837xD_CpuTimers.obj" "common\F2837xD_DefaultISR.obj" "common\F2837xD_EPwm.obj" "common\F2837xD_GlobalVariableDefs.obj" "common\F2837xD_Gpio.obj" "common\F2837xD_PieCtrl.obj" "common\F2837xD_PieVect.obj" "common\F2837xD_SysCtrl.obj" "common\F2837xD_usDelay.obj" "source_user\ADC_Init.obj" "source_user\Coordinate_Trans.obj" "source_user\DAC_Init.obj" "source_user\Device_Init.obj" "source_user\Discrete_Domain_Controller.obj" "source_user\EPWM_Init.obj" "source_user\EQEP_Init.obj" "source_user\Global_Functions_Variables_Call.obj" "source_user\PID_Controller.obj" "source_user\PMSM_FOC_Main.obj" "source_user\Ramp_Function.obj" "source_user\SVPWM_Gen.obj" 
	-$(RM) "common\F2837xD_Adc.d" "common\F2837xD_CpuTimers.d" "common\F2837xD_DefaultISR.d" "common\F2837xD_EPwm.d" "common\F2837xD_GlobalVariableDefs.d" "common\F2837xD_Gpio.d" "common\F2837xD_PieCtrl.d" "common\F2837xD_PieVect.d" "common\F2837xD_SysCtrl.d" "source_user\ADC_Init.d" "source_user\Coordinate_Trans.d" "source_user\DAC_Init.d" "source_user\Device_Init.d" "source_user\Discrete_Domain_Controller.d" "source_user\EPWM_Init.d" "source_user\EQEP_Init.d" "source_user\Global_Functions_Variables_Call.d" "source_user\PID_Controller.d" "source_user\PMSM_FOC_Main.d" "source_user\Ramp_Function.d" "source_user\SVPWM_Gen.d" 
	-$(RM) "common\F2837xD_CodeStartBranch.d" "common\F2837xD_usDelay.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

