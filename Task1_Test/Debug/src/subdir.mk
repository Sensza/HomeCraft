################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/TXConverter.cpp \
../src/TXConverter_Test.cpp \
../src/Task1_Test.cpp 

OBJS += \
./src/TXConverter.o \
./src/TXConverter_Test.o \
./src/Task1_Test.o 

CPP_DEPS += \
./src/TXConverter.d \
./src/TXConverter_Test.d \
./src/Task1_Test.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


