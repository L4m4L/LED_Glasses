# LED_Glasses
A hobby project aimed at making sound reactive LED glasses that you could take clubbing.

## Hardware

The hardware consists entirely of three PCBs without an enclosure with low profile hinges and a battery system that has yet to be worked out. The PCBs are designed in KiCAD and are as follows:

- The controller arm (`hardware/arm_mcu`) which houses the microcontroller (`STM32G071KBU6`), a microphone (`SPH0641LM4H-1`) and a 3v3 linear regulator.
- The power arm (`hardware/arm_power`) which houses input power protection, an on switch, 5v5 switching regulator and a power good indicator.
- The front display (`hardware/front_display`) which houses the LED matrix (`SK6812MINI`).

## Firmware

`firmware/arduino` contains an early revision of the firmware, with heavy modifications likely in future versions.

Hardware abstraction will be provided by either the STM HAL (pretty bloated and not very intuitive), STM LL (not very intuitive and a bit difficult to parse the many thousand line header files for insight), libopencm3 (haven't tried it but I've heard it's pretty good) or CMSIS (gives us the most control but literally no help haha) (i.e. we haven't decided yet but probably not Arduino).

## TODO

- Read https://github.com/EdwardHartmann/SK6812RGBW for inspiration
- Decide on copyright license
- Investigate smaller LED packages
- 