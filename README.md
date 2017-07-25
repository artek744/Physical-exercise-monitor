# Physical Exercise Monitor
 
The project has been created to serve the purpose as an engineering thesis.

- - -
1. [Descritpion](#descritpion)  
2. [Construction](#construction)  
3. [Electronic schematic](#electronic-schematic)  
4. [Demonstration (movie)](#demonstration)  
5. [Engineering dissertation topic](#engineering-dissertation-topic)
- - -

<a name="description"></a>

## 1\. Description

The aim of the engineering thesis was to create a device and an application, which would monitor the correct posture of the body during physical exercises. The thesis includes a description of the components on which the project was based. Using the Arduino platform as the main control unit, allows for an improvement and further development of the device for monitoring one's correct posture.

<a name="construction"></a>
## 2\. Construction

The picture below shows the location of the  accelerometers and Arduino Uno.

<p align="center">
  <img src="/img/construction.jpg" width="450"/>
</p>

- ![red-color](https://placehold.it/15/e20001/000000?text=+) Location of accelerometers
- ![green-color](https://placehold.it/15/27c45b/000000?text=+) Location of Arduino Uno

<a name="Electronic schematic"></a>
## 3\. Electronic schematic

The device consists of:

 - Arduino Uno - as the control unit.
 - Adxl345 accelerometers - for calculating the inclination angles.
 - TCA9548A multiplexer - enables using more than two accelerometers.
 - Green LEDs - to notify if the accelerometer are properly powered.
 - Blue LED & buzzer - to notify users about their incorrect posture during physical exercise.

<p align="center">
  <img src="/img/electronic-schematic.jpg" width="450"/>
</p>


<a name="demonstration"></a>
## 4\. Demonstration

Movie demonstrates the way in which the Physical Exercise Monitor works.

<p align="center">
<a href="https://www.youtube.com/watch?v=nIm1RtE0_s0)">
	<img src="img/demo.jpg" width="600">
</a>
</p>

<a name="engineering-dissertation-topic"></a>
## 5\. Engineering dissertation topic

**Developing an application on a device in order to monitor the correct posture during physical exercises.**
