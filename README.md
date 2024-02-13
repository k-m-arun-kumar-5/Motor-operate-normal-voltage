Description :
=============
in simulation, single phase motor is indicated as lamp and R supply(ie as single phase supply). VAR_POWER_POT is adjusted to vary the voltage across VAR_POWER_POT to simulate R supply voltage.  When voltage across VAR_POWER_POT is within range of >= MIN_R_RMS_VOLTAGE and <= MAX_R_RMS_VOLTAGE, then trigger TRIAC gate ON to start or continue run the motor and MOTOR_ON LED is used as an indicator. When voltage across VAR_POWER_POT is either < MIN_R_RMS_VOLTAGE, indicated by R_VOLTAGE_MIN LED or > MAX_R_RMS_VOLTAGE, indicated by R_VOLTAGE_MAX LED, then stop the motor by trigger TRIAC gate OFF, as R supply voltage is abnormal, which will damage the motor, others machines and circuits connected to R supply. 

CAUTION:
========
Schematics and simulation is done by using Proteus CAD. NOT EXPERIMENTED IN REAL TIME ENVIRONMENT.

Purpose :
=========
In all my respective repositories, I just shared my works that I worked as the learning path and practiced, with designed, developed, implemented, simulated and tested, including some projects, assignments, documentations and all other related files and some programming that might not being implement, not being completed, lacks some features or have some bugs. Purpose of all my repositories, if used, can be used for EDUCATIONAL PURPOSE ONLY. It can be used as the open source and freeware. Kindly read the LICENSE.txt for license, terms and conditions about the use of source codes, binaries, documentation and all other files, located in all my repositories. 

