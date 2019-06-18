################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../UnitTests/Task2_Test.cpp 

OBJS += \
./UnitTests/Task2_Test.o 

CPP_DEPS += \
./UnitTests/Task2_Test.d 


# Each subdirectory must supply rules for building sources it contributes
UnitTests/%.o: ../UnitTests/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I"/home/tomasz/eclipse-workspace/Task2_Test/GoogleTestLib" -I"/home/tomasz/eclipse-workspace/Task2_Test/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


