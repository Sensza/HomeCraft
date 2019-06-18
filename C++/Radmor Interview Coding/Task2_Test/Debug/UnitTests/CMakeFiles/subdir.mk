################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CXX_SRCS += \
../UnitTests/CMakeFiles/feature_tests.cxx 

C_SRCS += \
../UnitTests/CMakeFiles/feature_tests.c 

OBJS += \
./UnitTests/CMakeFiles/feature_tests.o 

CXX_DEPS += \
./UnitTests/CMakeFiles/feature_tests.d 

C_DEPS += \
./UnitTests/CMakeFiles/feature_tests.d 


# Each subdirectory must supply rules for building sources it contributes
UnitTests/CMakeFiles/%.o: ../UnitTests/CMakeFiles/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

UnitTests/CMakeFiles/%.o: ../UnitTests/CMakeFiles/%.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I"/home/tomasz/eclipse-workspace/Task2_Test/GoogleTestLib" -I"/home/tomasz/eclipse-workspace/Task2_Test/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


