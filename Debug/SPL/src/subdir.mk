################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SPL/src/stm32f30x_adc.c \
../SPL/src/stm32f30x_can.c \
../SPL/src/stm32f30x_comp.c \
../SPL/src/stm32f30x_crc.c \
../SPL/src/stm32f30x_dac.c \
../SPL/src/stm32f30x_dbgmcu.c \
../SPL/src/stm32f30x_dma.c \
../SPL/src/stm32f30x_exti.c \
../SPL/src/stm32f30x_flash.c \
../SPL/src/stm32f30x_gpio.c \
../SPL/src/stm32f30x_i2c.c \
../SPL/src/stm32f30x_iwdg.c \
../SPL/src/stm32f30x_misc.c \
../SPL/src/stm32f30x_opamp.c \
../SPL/src/stm32f30x_pwr.c \
../SPL/src/stm32f30x_rcc.c \
../SPL/src/stm32f30x_rtc.c \
../SPL/src/stm32f30x_spi.c \
../SPL/src/stm32f30x_syscfg.c \
../SPL/src/stm32f30x_tim.c \
../SPL/src/stm32f30x_usart.c \
../SPL/src/stm32f30x_wwdg.c 

OBJS += \
./SPL/src/stm32f30x_adc.o \
./SPL/src/stm32f30x_can.o \
./SPL/src/stm32f30x_comp.o \
./SPL/src/stm32f30x_crc.o \
./SPL/src/stm32f30x_dac.o \
./SPL/src/stm32f30x_dbgmcu.o \
./SPL/src/stm32f30x_dma.o \
./SPL/src/stm32f30x_exti.o \
./SPL/src/stm32f30x_flash.o \
./SPL/src/stm32f30x_gpio.o \
./SPL/src/stm32f30x_i2c.o \
./SPL/src/stm32f30x_iwdg.o \
./SPL/src/stm32f30x_misc.o \
./SPL/src/stm32f30x_opamp.o \
./SPL/src/stm32f30x_pwr.o \
./SPL/src/stm32f30x_rcc.o \
./SPL/src/stm32f30x_rtc.o \
./SPL/src/stm32f30x_spi.o \
./SPL/src/stm32f30x_syscfg.o \
./SPL/src/stm32f30x_tim.o \
./SPL/src/stm32f30x_usart.o \
./SPL/src/stm32f30x_wwdg.o 

C_DEPS += \
./SPL/src/stm32f30x_adc.d \
./SPL/src/stm32f30x_can.d \
./SPL/src/stm32f30x_comp.d \
./SPL/src/stm32f30x_crc.d \
./SPL/src/stm32f30x_dac.d \
./SPL/src/stm32f30x_dbgmcu.d \
./SPL/src/stm32f30x_dma.d \
./SPL/src/stm32f30x_exti.d \
./SPL/src/stm32f30x_flash.d \
./SPL/src/stm32f30x_gpio.d \
./SPL/src/stm32f30x_i2c.d \
./SPL/src/stm32f30x_iwdg.d \
./SPL/src/stm32f30x_misc.d \
./SPL/src/stm32f30x_opamp.d \
./SPL/src/stm32f30x_pwr.d \
./SPL/src/stm32f30x_rcc.d \
./SPL/src/stm32f30x_rtc.d \
./SPL/src/stm32f30x_spi.d \
./SPL/src/stm32f30x_syscfg.d \
./SPL/src/stm32f30x_tim.d \
./SPL/src/stm32f30x_usart.d \
./SPL/src/stm32f30x_wwdg.d 


# Each subdirectory must supply rules for building sources it contributes
SPL/src/%.o SPL/src/%.su SPL/src/%.cyclo: ../SPL/src/%.c SPL/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32 -DSTM32F302R8Tx -DSTM32F3 -DNUCLEO_F302R8 -DDEBUG -DUSE_STDPERIPH_DRIVER=1 -c -I"C:/Users/th/src/stm/ex0102/SPL/inc" -I"C:/Users/th/src/stm/ex0102/Inc" -I"C:/Users/th/src/stm/ex0102/cmsis" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-SPL-2f-src

clean-SPL-2f-src:
	-$(RM) ./SPL/src/stm32f30x_adc.cyclo ./SPL/src/stm32f30x_adc.d ./SPL/src/stm32f30x_adc.o ./SPL/src/stm32f30x_adc.su ./SPL/src/stm32f30x_can.cyclo ./SPL/src/stm32f30x_can.d ./SPL/src/stm32f30x_can.o ./SPL/src/stm32f30x_can.su ./SPL/src/stm32f30x_comp.cyclo ./SPL/src/stm32f30x_comp.d ./SPL/src/stm32f30x_comp.o ./SPL/src/stm32f30x_comp.su ./SPL/src/stm32f30x_crc.cyclo ./SPL/src/stm32f30x_crc.d ./SPL/src/stm32f30x_crc.o ./SPL/src/stm32f30x_crc.su ./SPL/src/stm32f30x_dac.cyclo ./SPL/src/stm32f30x_dac.d ./SPL/src/stm32f30x_dac.o ./SPL/src/stm32f30x_dac.su ./SPL/src/stm32f30x_dbgmcu.cyclo ./SPL/src/stm32f30x_dbgmcu.d ./SPL/src/stm32f30x_dbgmcu.o ./SPL/src/stm32f30x_dbgmcu.su ./SPL/src/stm32f30x_dma.cyclo ./SPL/src/stm32f30x_dma.d ./SPL/src/stm32f30x_dma.o ./SPL/src/stm32f30x_dma.su ./SPL/src/stm32f30x_exti.cyclo ./SPL/src/stm32f30x_exti.d ./SPL/src/stm32f30x_exti.o ./SPL/src/stm32f30x_exti.su ./SPL/src/stm32f30x_flash.cyclo ./SPL/src/stm32f30x_flash.d ./SPL/src/stm32f30x_flash.o ./SPL/src/stm32f30x_flash.su ./SPL/src/stm32f30x_gpio.cyclo ./SPL/src/stm32f30x_gpio.d ./SPL/src/stm32f30x_gpio.o ./SPL/src/stm32f30x_gpio.su ./SPL/src/stm32f30x_i2c.cyclo ./SPL/src/stm32f30x_i2c.d ./SPL/src/stm32f30x_i2c.o ./SPL/src/stm32f30x_i2c.su ./SPL/src/stm32f30x_iwdg.cyclo ./SPL/src/stm32f30x_iwdg.d ./SPL/src/stm32f30x_iwdg.o ./SPL/src/stm32f30x_iwdg.su ./SPL/src/stm32f30x_misc.cyclo ./SPL/src/stm32f30x_misc.d ./SPL/src/stm32f30x_misc.o ./SPL/src/stm32f30x_misc.su ./SPL/src/stm32f30x_opamp.cyclo ./SPL/src/stm32f30x_opamp.d ./SPL/src/stm32f30x_opamp.o ./SPL/src/stm32f30x_opamp.su ./SPL/src/stm32f30x_pwr.cyclo ./SPL/src/stm32f30x_pwr.d ./SPL/src/stm32f30x_pwr.o ./SPL/src/stm32f30x_pwr.su ./SPL/src/stm32f30x_rcc.cyclo ./SPL/src/stm32f30x_rcc.d ./SPL/src/stm32f30x_rcc.o ./SPL/src/stm32f30x_rcc.su ./SPL/src/stm32f30x_rtc.cyclo ./SPL/src/stm32f30x_rtc.d ./SPL/src/stm32f30x_rtc.o ./SPL/src/stm32f30x_rtc.su ./SPL/src/stm32f30x_spi.cyclo ./SPL/src/stm32f30x_spi.d ./SPL/src/stm32f30x_spi.o ./SPL/src/stm32f30x_spi.su ./SPL/src/stm32f30x_syscfg.cyclo ./SPL/src/stm32f30x_syscfg.d ./SPL/src/stm32f30x_syscfg.o ./SPL/src/stm32f30x_syscfg.su ./SPL/src/stm32f30x_tim.cyclo ./SPL/src/stm32f30x_tim.d ./SPL/src/stm32f30x_tim.o ./SPL/src/stm32f30x_tim.su ./SPL/src/stm32f30x_usart.cyclo ./SPL/src/stm32f30x_usart.d ./SPL/src/stm32f30x_usart.o ./SPL/src/stm32f30x_usart.su ./SPL/src/stm32f30x_wwdg.cyclo ./SPL/src/stm32f30x_wwdg.d ./SPL/src/stm32f30x_wwdg.o ./SPL/src/stm32f30x_wwdg.su

.PHONY: clean-SPL-2f-src

