Robotic project, running in Linux platform using mini2440 Dev board

# Introduction #

This is a robotics project, platform Linux loaded in Arm9 2440c (FriendlyArm development board). The application is written in C++.


# Details #

**Current Status**

**Sensors and actuators**

**Programing environment**

**Files**

**Current Status**

Recently I finished the first level of my project, it can just move around by its own, no control over it, if it find some obstacle it will go back a little and takes a turn. To find obstacle it has IR sensor.


**Sensors and actuators**

> There are two IR sensor to detect obstacles, one PIR sensor and two gear motors to drive the Robot. All peripheral interfaced to Arm Dev board using I2C, so there is no much connection between FriendlyArm and additional circuitries. To interface peripherals using I2C protocol I am using PCF8574 I2C bus expander.I am using this for safety of Arm development board.