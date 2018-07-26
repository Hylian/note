# Firmware

## Build Instructions

Update git submodules and run make. 

## Design

### Debounce

Debounce is done in the debounce.c file. It uses the Timer0 peripheral on the 32U4 to count how long a pin has been at the same level, and only exposes the level change if enough time has passed. The timer counts required to trigger a level change are configurable per-pin. Currently, there are different trigger counts configured for encoder pins and button pins.

The trigger counts are so low right now that they don't really matter. I've found that the hardware debounce on the encoders is enough.

### USB

The firmware is based off the LUFA library by Dean Camera. It instantiates three USB descriptors: an HID mouse, HID keyboard, and CDC serial for debug/configuration.

The two VOL knobs control the x/y movement of the mouse, while the buttons send keyboard button presses.
