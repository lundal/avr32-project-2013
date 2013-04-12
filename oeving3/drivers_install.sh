#!/bin/sh


# LEDS
driver_leds="driver_leds.ko"
module_leds="LEDS"
device_leds="leds"

# Install driver
insmod ${driver_leds}

# Remove old device file
rm -f /dev/${device_leds}

# Find major number
major=$(awk "\$2==\"${module_leds}\" {print \$1}" /proc/devices)

# Create new device file
mknod /dev/${device_leds} c ${major} 0


# BUTTONS
driver_buttons="driver_buttons.ko"
module_buttons="BUTTONS"
device_buttons="buttons"

# Install driver
insmod ${driver_buttons}

# Remove old device file
rm -f /dev/${device_buttons}

# Find major number
major=$(awk "\$2==\"${module_buttons}\" {print \$1}" /proc/devices)

# Create new device file
mknod /dev/${device_buttons} c ${major} 0
