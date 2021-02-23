# RasPi_L6470_Multi

RasPi_L6470_Multi is a library that makes L6470(STMicro) easy to use on Raspberry Pi.

* Daisy chain supported(tested 2 axes)

# Features
you can do with this library

* Execute L6470 command(Run, Goto, GoHome ... and so on.)
* Set initial parametor to All L6470.
* Get parametor from L6470.

# Requirement

* GNU Make (debug with ver4.2.1)
* GCC (debug with ver Raspbian 8.3.0-6+rpi1)
* WiringPi (debug with ver2.60)
    (For SPI control. Therefore, it may work with other SBCs, but I have not confirmed it. )

# Usage

First Test
```bash
git clone https://github.com/poemoep/RasPi_L6470_Multi.git
cd RasPi_L6470
make
./main
```

Usage this library in main function.

* Declare a variable as L6470_DATA_ARRAY type.
* Set L6470_DATA_T on each axis. (Use function "L6470_*" which return L6470_DATA_T)
* exec command with function "L6470_rw_multi(L6470_DATA_ARRAY data, const char* debug_message)".


# Note
if you don't want to get debug message like "[L6470 DEBUG]: ~",

Comment out "#define L6470_PRINT_MESSAGE" from "L6470_user.h".


If you want to change number of axes, edit "L6470_DEV_NUM" value in "L6470_user.h".

# Author

* Made by poemoep.

  (I made it within my hobby, so there may be bugs.)

# License
The source code is licensed MIT.
