# Parking Sensor Atmega328P

## Description
This is a parking sensor made on the Arduino UNO that uses Atmega328P microcontroller. It uses 2 **ultrasonic sensors** to detect the object. They are designed to detect what is in front and in the back of the car. The sensor also has an **LCD** that displays the distances to the detected objects, an **LED** and a **passive buzzer** to warn the driver.

## Hardware design
Components:
 - Arduino UNO 16U2
 - Breadboard
 - 2 x Sensor ultrasonic HC-SR04
 - LED RGB
 - LCD 1602 IIC/I2C
 - Buzzer
 - Button

![alt text](HardwareDiagram.png "Hardware Design")

## Sftware Design
The software is available both in Arduino code and in AVR code written in C. The AVR code was made using *Platform IO*.

### **Ultrasonic Sensors**
The sensors detect the closesest object in front and back of the car. They both use the same pin as *Trigger pin* so they take turns in getting the signal.

### **LCD**
The distances are shown on the **LCD** but only if they are closer than 50cm. 

### **LED**
The **LED** has 3 states:
 - Red: The closest object is less than 30cm to one of the sensors
 - Green: The closest object is between 30 and 50 cm to one of the sensors
 - Off: Either no object is detected closer than 50cm or the driver turned off the Sensor (See Button)

### **Buzzer**
The **buzzer** will make noise when one of the sensors will detect an object hat is close enough to be dangerous.

### **Button**
Using the **Button**, the driver can turn off the **LED** and the **buzzer**. This will only work when the car is *parked*. If the car starts moving again, the parking Sensor will automatically turn on.

## Compiling and Running
There are 3 options to compile and run the code:
1. Use a simulator to create the hardware shown above. In that simulator, the arduino code can be used to test the Parking Sensor.
2. Use the Arduino IDE to upload the Arduino code directly to the microcontroller (if the hardware parts are available).
3. Use PlatformIO to compile and upload the C code directly to the microcontroller (if the hardware parts are available).


