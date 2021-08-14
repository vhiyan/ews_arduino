# ews_arduino

## use arduino nano for this program

## rtc not implemented yet

## Serial Port

GPS neo7m use software serial on pin 3 (RX) and 2 (TX)

- green on pin 3
- yellow on pin 2
- 9600 bps

## Serial communication

- send "DAFTAR" to arduino when input device id / no txt file
- wait for "CHANGE" from arduino
  -->arduino will send DAFTAR#<ID>#<LAT>#<LON>
- send "FETCH" to arduino to get data lux and **RTC**
  -->arduino will send DATA#<lux>#**<RTC>**

## Keypad

Keypad on pin
rowPins[ROWS] = {12, 11, 10, 9}; //connect to the row pinouts of the keypad
colPins[COLS] = {8, 7, 6}; //connect to the column pinouts of the keypad
  
## Pendaftaran Device
 - `* untuk backspace`
 - `# untuk yes/next`
  1. Hidupkan Device
  2. Input device id (default id = 0000)
  3. OK (wait for gps get location)
