HTTPNotifier
============

Control an RGB LED via HTTP commands

This sketch uses the built-in Ethernet library for Arduino.  A MAC and IP address can be configured.  In the comments is an example of how to switch between DHCP and Static IP.  Example commands:

Turns on RED flashing LED
http://192.168.1.1/cmd=0

Turns on GREEN flashing LED
http://192.168.1.1/cmd=1

Turns on BLUE flashing LED
http://192.168.1.1/cmd=2

Turns on YELLOW flashing LED
http://192.168.1.1/cmd=3

Turns on WHITE flashing LED
http://192.168.1.1/cmd=4

Turns off LED
http://192.168.1.1/cmd=5

Turns on Buzzer for a certain interval (parameter is in the code)
http://192.168.1.1/cmd=6

