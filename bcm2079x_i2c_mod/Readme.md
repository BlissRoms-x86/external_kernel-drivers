This a glue driver for Boradcom NFC chips BCM2079x connected via I2C.
The original driver was developed by Broadcom for Android devices, using the
Android NFC HAL library implementing the NCI and some additional BCM realted
functions.

Unfortunately there is no public documentation for this kind of NFC chips so
we need to do educated guessing and learning from source.

The driver in this repo is modified to be configurable via ACPI DSDT. The
hard requirement for a wakeup GPIO has been removed since devices like the
Lenovo Thinkpad Tablet 10 do not provide such GPIO, just one GPIO which is
presumably the ENABLE GPIO.

@Broadcom: Please provide the specs to this family of chips. I will sign an
NDA if necessary. Please contact me.
