5/3/2013

BERKELEY BOARD PROJECT

-San Diego California: checked in new slave sketch version 2C. This version scans and sends all sensor data per I2C transfer and uses two compressed longs to do so. Both of these longs use the the individual bits to determine whether the sensor is on or off.

The array:  long    leftSwitchOnOffArray[2] contains all of the data. All of the first long is used and 16 bits of the second long are used.

