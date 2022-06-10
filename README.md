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

# Shopping list
My hardware is built up from what I had stashed.

The main controller is a [Seeeduino Xiao](https://shop.pimoroni.com/products/seeeduino-xiao?variant=32170131816531)

The sensors I'm using are breakout boards for a BME280 barometric pressure
sensor, an ADXL345 3-axis accelerometer, a NEO-6M gps receiver and a micro
SD card.  The micro SD breakout on mine has its voltage regulator removed
but you can just hook it up to the 5v instead if you like.  I have a small
OLED display and a couple of push buttons to control it.  I just used some
cheap PCB momentary switches.

[BME280 pressure sensor](https://smile.amazon.co.uk/Youmile-Digital-Barometric-Pressure-Temperature/dp/B0965843T6)

[ADXL345 accelerometer](https://smile.amazon.co.uk/XTVTX-ADXL345-Digital-Acceleration-Gravity/dp/B09NRFDH5J)

[NEO-6M gps](https://smile.amazon.co.uk/dp/B088LR3488)

It should be noted that this appears to be a fake u-Blox receiver as it
won't accept some UBX commands

[Micro SD breakout](https://smile.amazon.co.uk/Youmile-Storage-Expansion-Interface-Raspberry/dp/B07XLM23X2)

[OLED display](https://smile.amazon.co.uk/MakerHawk-Display-Module-SSD1306-Arduino/dp/B07BDFXFRK)

Construction is pretty much everything mounted on the back of the back of
the micro sd breakout using hot melt glue then point-to-point wired using
0.5mm enamelled copper wire.  The diplay and buttons are bonded into the 3d
printed housing then wired to the controller which is then screwed down
using the mounting holes of the SD breakout.

Nothing is too critical in all of this.  Mine is sized to fit in a 54mm
airframe just below the nosecone.  Lay it all out if you make one any way
you like.

