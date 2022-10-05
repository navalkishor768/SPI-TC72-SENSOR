# SPI-TC72-SENSOR
Interfacing TC72 sensor with Atmega32 using SPI communication

Here i have tried using SPI communication of Atmega32 with TC72 temperature sensor.
Following are some problems and their solution.

TC72 sensor have CS pin which is Acive High pin, while some chips have CS has Active low.

Hence i have used NOT gate inverter.

I was getting undesired results at some temperature readings which was due to CPHA set to 0 in master mode in Atmega32.

CPHA was set to 1, now results were correct.

This work is carried out on Microchip Studio.

Following is the schematic of Proteus Simulation and demo video.

https://drive.google.com/file/d/1Pq2NuR50hOBAehohzDUb80MoUh1_pF4K/view?usp=sharing

![tc72_at32](https://user-images.githubusercontent.com/111571035/194109737-5c14cec7-176d-45aa-bad5-815559a213fe.SVG)

https://drive.google.com/file/d/1Pq2NuR50hOBAehohzDUb80MoUh1_pF4K/view?usp=sharing
