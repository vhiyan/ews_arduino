# ews_arduino
## use arduino nano for this program 
## rtc not implemented yet

## Serial Port
GPS neo6m use software serial on pin 3 (RX) and 4 (TX) 
9600 bps

## Serial communication
* send "DAFTAR" to arduino when input device id / no txt file 
* wait for "CHANGE" from arduino 
-->arduino will send DAFTAR#<ID>#<LAT>#<LON>
* send "FETCH" to arduino to get data lux and **RTC**
-->arduino will send DATA#<lux>#**<RTC>**

## Keypad
Keypad on pin 
rowPins[ROWS] = {12, 11, 10, 9}; //connect to the row pinouts of the keypad
colPins[COLS] = {8, 7, 6}; //connect to the column pinouts of the keypad

