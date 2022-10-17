# SPI-TC72-SENSOR
Interfacing TC72 sensor with Atmega32 using SPI communication

Here i have tried using SPI communication of Atmega32 with TC72 temperature sensor.
Following are some problems and their solution.

TC72 sensor have CS pin which is Acive High pin, while some chips have CS has Active low.

Hence i have used NOT gate inverter.

I was getting undesired results at some temperature readings which was due to CPHA set to 0 in master mode in Atmega32.

CPHA was set to 1, now results were correct.

This work is carried out on Microchip Studio.

Update1.....
Improved  Code for reading and printing temperature.

Reading temperature at 0.5 sec and printing temperature at 1 sec as selected Celsius or Fahrenheit.

Following is the schematic of Proteus Simulation and demo video.

https://drive.google.com/file/d/1H1pz7EZGXOTSyRFqThRCWEs7T1VQYk-p/view?usp=sharing

![tc72_at32_update1](https://user-images.githubusercontent.com/111571035/196241268-99f76c04-1145-402e-88b1-e8d16572008a.SVG)

https://drive.google.com/file/d/1H1pz7EZGXOTSyRFqThRCWEs7T1VQYk-p/view?usp=sharing
