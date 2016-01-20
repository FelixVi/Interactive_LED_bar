# Interactive LED bar
Interactive LED bar with WS2811 LEDs and home-build IR proximity sensors

Using PSoC4 dev boards to read out infrared sensor nodes. Setup is so that each controller has 8 nodes attached and compares their analog output voltage to a threshold that is set by software. Controllers are setup as I2C slaves so that a master (Arduino or Raspberry Pi) can read a register (SENS_OUT) to find out which sensors have crossed the threshold since the last reset. After a read, the master can reset SENS_OUT by writing a “1” to the RESET register. Thresholds are set by writing their value to THR0 – THR16 registers. Hereby, THR0 is the high byte and THR1 is the low byte for the first threshold, THR2 is the high byte and THR3 is the low byte for the second threshold, etc.
I2C is setup to operate at 100 kHz (Arduino default). The PSoC4 are 5V devices, so BSS138 transistors are used as level shifters to allow for 5V pull-ups on the PSoc4 side while 3.3V pull-ups (all resistors are 10k) are used on the Arduino (or Raspberry Pi) side.

Slave address is 0x08 for the first controller and addresses increment by one for each additional node.

R/W registers:
THR0 – THR16 registers have addresses from 0-15 (0x00 – 0x0F).
Unused R/W registers have addresses from 16-31 (0x10 - 0x1F)
EEPROM_WRITE (currently not working) has the address 32 (0x20)
RESET has the address 33 (0x21).
SENS_OUT has the address 34 (0x22)

Read only registers:
Unused registers has address 35 (0x23 - 0x1F)
Most recent ADC values are stored in registers 36 - 51 (0x24 - 0x33)
Remaining registers are unused
