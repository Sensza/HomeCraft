################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../GoogleTestLib/gtest/gtest-all.cc 

CC_DEPS += \
./GoogleTestLib/gtest/gtest-all.d 

OBJS += \
./GoogleTestLib/gtest/gtest-all.o 


# Each subdirectory must supply rules for building sources it contributes
GoogleTestLib/gtest/%.o: ../GoogleTestLib/gtest/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I"/home/tomasz/eclipse-workspace/Task2_Test/GoogleTestLib" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


