################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
modules/Proximitry/%.obj: ../modules/Proximitry/%.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=none -me --include_path="C:/Users/Prajeeth/OneDrive - Airzai/Airzai Firmware/Airzai_Application_V6 (+TOF)/airzai_prototype_CC3220SF_LAUNCHXL_tirtos_ccs" --include_path="C:/Users/Prajeeth/OneDrive - Airzai/Airzai Firmware/Airzai_Application_V6 (+TOF)/airzai_prototype_CC3220SF_LAUNCHXL_tirtos_ccs/modules/" --include_path="C:/Users/Prajeeth/OneDrive - Airzai/Airzai Firmware/Airzai_Application_V6 (+TOF)/airzai_prototype_CC3220SF_LAUNCHXL_tirtos_ccs/modules/PWM" --include_path="C:/Users/Prajeeth/OneDrive - Airzai/Airzai Firmware/Airzai_Application_V6 (+TOF)/airzai_prototype_CC3220SF_LAUNCHXL_tirtos_ccs/modules/Humidity" --include_path="C:/Users/Prajeeth/OneDrive - Airzai/Airzai Firmware/Airzai_Application_V6 (+TOF)/airzai_prototype_CC3220SF_LAUNCHXL_tirtos_ccs/modules/Proximitry" --include_path="C:/Users/Prajeeth/OneDrive - Airzai/Airzai Firmware/Airzai_Application_V6 (+TOF)/airzai_prototype_CC3220SF_LAUNCHXL_tirtos_ccs/modules/Proximitry/VL53L0x/core/inc" --include_path="C:/Users/Prajeeth/OneDrive - Airzai/Airzai Firmware/Airzai_Application_V6 (+TOF)/airzai_prototype_CC3220SF_LAUNCHXL_tirtos_ccs/modules/Proximitry/VL53L0x/core/src" --include_path="C:/Users/Prajeeth/OneDrive - Airzai/Airzai Firmware/Airzai_Application_V6 (+TOF)/airzai_prototype_CC3220SF_LAUNCHXL_tirtos_ccs/modules/Proximitry/VL53L0x/platform/src" --include_path="C:/Users/Prajeeth/OneDrive - Airzai/Airzai Firmware/Airzai_Application_V6 (+TOF)/airzai_prototype_CC3220SF_LAUNCHXL_tirtos_ccs/modules/Proximitry/VL53L0x/platform/inc" --include_path="C:/Users/Prajeeth/OneDrive - Airzai/Airzai Firmware/Airzai_Application_V6 (+TOF)/airzai_prototype_CC3220SF_LAUNCHXL_tirtos_ccs/modules/TOUCH" --include_path="C:/Users/Prajeeth/OneDrive - Airzai/Airzai Firmware/Airzai_Application_V6 (+TOF)/airzai_prototype_CC3220SF_LAUNCHXL_tirtos_ccs/modules/LED" --include_path="C:/ti/simplelink_cc32xx_sdk_2_30_00_05/source/ti/posix/ccs" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/include" --include_path="C:/Users/Prajeeth/Downloads/Airzai_Application_V6 (+TOF)/airzai_prototype_CC3220SF_LAUNCHXL_tirtos_ccs/modules/TOUCH" -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="modules/Proximitry/$(basename $(<F)).d_raw" --obj_directory="modules/Proximitry" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


