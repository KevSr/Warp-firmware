# Alert system to aid blind pedestrians
**Junyeob Kim**

**Trinity College**

**jk680**

## Summary
This project aims to develop a alert system that can possibly help people with visual disability.
The system consists of two different methods to alert the user.
1. Piezo Buzzer.
2. Small vibrating motor.
   
This system uses a *Ultrasonic proximity sensor* to detect the objects in front of the user. The system triggers piezo buzzer to ring when an object is within 1.5m. If the object comes closer than 50cm of the user, vibrating motor gets triggered as well, providing additional alert.

Check [diff.patch](../diff.patch) file to see detailed changes I have made for this project. This file also includes changes in srec and hex files as well.

## Repository Layout
This project uses Warp-firmware as its framework. Therefore, all project files are located in
```
 Warp-firmware/src/boot/ksdk1.1.0
```
folder.

The main system file, *warp-kl03-ksdk1.1-boot.c*, is only used to link the main system to use the Warp-firmware.

Following list describes each sets of files I have created for this project.

### HC-SR04 Ultrasonic Proximity Sensor
Files included: devHCSR04.c and devHCSR04.h
* This is where the main system is located. It checks the sensor readings, and selects which sensor(s) to run.
* Also includes codes to run **vibrating mini motor** described before, as this module only requires a voltage source to run the motor. Output of a GPIO pin was enough.
### HPA-17F Piezo Buzzer
Files included: devHPA17F.c and devHPA17F.h
* Piezo buzzer requires PWM, so TPM has been initialised in these files.
### Microsecond Delay
Files included: delay.c and delay.h
* HC-SR04 module requires delays in microseconds, in order to trigger the sensor, and to read the values from sensor more accurately. However, *OSA_TimeDelay* only supports up to 1 ms delay, so new function is built.
* It uses assembly codes to create the delay.
  ```C
  asm volatile("nop");
  ```
### SSD-1331 OLED Display
Files included: devSSD1331.c and devSSD1331.h
* Initial plan was to use OLED display to also show the distance, for debugging and to provide further information.
* It has been found out that the *KL03 freescale platform* cannot provide enough power to all different components used above, so I decided not to use it. 