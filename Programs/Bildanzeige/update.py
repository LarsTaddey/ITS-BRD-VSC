import re
with open('Bildanzeige.cproject.yml', 'r') as f:
    content = f.read()

# Add USB middleware include paths
old_add = '        - ../../ITS_BRD_LIB/Utilities/AddFonts'
new_add = '        - ../../ITS_BRD_LIB/Utilities/AddFonts\n        - ../../stm32cubef4/Middlewares/ST/STM32_USB_Device_Library/Core/Inc\n        - ../../stm32cubef4/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc'
content = content.replace(old_add, new_add)

# Add HAL_PCD_MODULE_ENABLED to define
old_def = '        - __MICROLIB'
new_def = '        - __MICROLIB\n        - HAL_PCD_MODULE_ENABLED'
content = content.replace(old_def, new_def)

# Add USB middleware group after HAL group
old_hal = '    # HAL-Treiber für STM32F4xx\n    - group: Drivers/STM32F4xx_HAL_Driver\n      files:\n        - file: ../../stm32cubef4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c\n        - file: ../../stm32cubef4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c\n        - file: ../../stm32cubef4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c\n        - file: ../../stm32cubef4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c\n        - file: ../../stm32cubef4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c\n        - file: ../../stm32cubef4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c\n        - file: ../../stm32cubef4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c\n        - file: ../../stm32cubef4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c\n        - file: ../../stm32cubef4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c\n        - file: ../../stm32cubef4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c\n        - file: ../../stm32cubef4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_eth.c'
new_hal = '    # HAL-Treiber für STM32F4xx\n    - group: Drivers/STM32F4xx_HAL_Driver\n      files:\n        - file: ../../stm32cubef4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c\n        - file: ../../stm32cubef4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c\n        - file: ../../stm32cubef4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c\n        - file: ../../stm32cubef4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c\n        - file: ../../stm32cubef4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c\n        - file: ../../stm32cubef4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c\n        - file: ../../stm32cubef4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c\n        - file: ../../stm32cubef4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c\n        - file: ../../stm32cubef4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c\n        - file: ../../stm32cubef4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c\n        - file: ../../stm32cubef4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_eth.c\n\n    # USB-Middleware für USB-Device-Mode\n    - group: Middleware/USB\n      files:\n        - file: ../../stm32cubef4/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c\n        - file: ../../stm32cubef4/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c\n        - file: ../../stm32cubef4/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c\n        - file: ../../stm32cubef4/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/usbd_cdc.c\n        - file: ../../stm32cubef4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pcd.c\n        - file: ../../stm32cubef4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pcd_ex.c\n        - file: ../../stm32cubef4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usb.c'
content = content.replace(old_hal, new_hal)

# Add user source files
old_user = '        - file: Src/main.c'
new_user = '        - file: Src/main.c\n        - file: Src/input.c\n        - file: Src/byte_io.c\n        - file: Src/byte_io_usb.c\n        - file: Src/byte_io_uart.c\n        - file: Src/ringbuf.c\n        - file: Src/headers.c\n        - file: Src/fileHeader.c\n        - file: Src/errorhandler.c\n        - file: Src/usb_device.c\n        - file: Src/usb_clock_config.c\n        - file: Src/usbd_cdc_if.c\n        - file: Src/usbd_conf.c\n        - file: Src/usbd_desc.c'
content = content.replace(old_user, new_user)

with open('Bildanzeige.cproject.yml', 'w') as f:
    f.write(content)
print('Done')
