# XiaoLogger
Simple gps, accelerometer and pressure sensor logger onto an SD Card using a Seeediuno Xiao as the controller

![Alt text](XiaoLogger.jpg?raw=true "Logger in 3d printed housing")

# Operation
The logger just needs power and will start appending data to LOGGER.TXT in
the root directory of the SD card.  In order to shut it down without
trashing the filesystem, you press the button attached to pin 0 for more
than a second and it will stop operation.  At this point, you can safely
remove power and all will be well.

# Hardware wiring
The pin allocations for my hardware are as below.  I do not have the buzzer
fitted as yet and may not do but I will add support for it.

	0	-	Push button, pull-up with connection to ground when pressed
	1	-	Push button, pull-up with connection to ground when pressed
	2	-	Beeper with integral oscillator, active high
	3	-	CS to SD breakout
	4	-	SDA to sensors
	5	-	SCL to sensors
	6	-	TX to GPS
	7	-	RX from GPS
	8	-	SCK to SD breakout
	9	-	MISO to SD breakout
	10	-	MOSI to SD breakout
	3v3	-	Power to SD breakout with its own regulator removed
	GND	-	General
	5v	-	Power to sensors with own regulators and GPS

My hardware runs from either the 5v port on the Seeeuino Xiao or I have a
650mAh lipo connected to 5v through a Schottky diode and a power switch.