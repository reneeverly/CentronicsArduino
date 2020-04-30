# Centronics Printer Emulator for Arduino Uno

## TODO: Adapting away from Arduino Mega
* [ ] Move Everything to Serial
  * [ ] Remove LCD Screen
  * [ ] Remove SD Card
 
## SD card pinout

| SD card pin name | Arduino pin |
|------------------|-------------|
| MOSI             | pin 11      |
| MISO             | pin 12      |
| CLK              | pin 13      |
| CS               | pin 4       |
| GND              | GND         |


## LCD pinout

| LCD pin name | Arduino pin number |
|--------------|--------------------|
| RS           | pin 8              |
| EN           | pin 9              |
| D4           | pin 4              |
| D5           | pin 5              |
| D6           | pin 6              |
| D7           | pin 7              |


## Parallel interface
Centronics parallel interface is pretty old and it was well documented in nineties, so it was easy to figure out how to connect Arduino to it.

| Name      | Centronics pin. DB25 connector | Arduino pin | Arduino pin direction | Notes                                                                     |
|-----------|--------------------------------|-------------|-----------------------|---------------------------------------------------------------------------|
| Strobe    | 1                              | pin 18      | Input                 | Pullup enabled. Attached to falling edge interrupt.                       |
| Error     | 15                             | pin 22      | Output                | Not used. Forced high.                                                    |
| Select    | 13                             | pin 24      | Output                | Not used. Forced high.                                                    |
| Paper Out | 12                             | pin 26      | Output                | Not used. Forced low.                                                    |
| Busy      | 11                             | pin 28      | Output                | Set high on the falling edge of Strobe. Set low after acknowledging data. |
| Ack       | 10                             | pin 30      | Output                | Generate falling edge to acknowledge data.                                |
| D0        | 2                              | pin 39      | Input                 | Parallel data.                                                            |
| D1        | 3                              | pin 41      | Input                 | Parallel data.                                                            |
| D2        | 4                              | pin 43      | Input                 | Parallel data.                                                            |
| D3        | 5                              | pin 45      | Input                 | Parallel data.                                                            |
| D4        | 6                              | pin 47      | Input                 | Parallel data.                                                            |
| D5        | 7                              | pin 49      | Input                 | Parallel data.                                                            |
| D6        | 8                              | pin 46      | Input                 | Parallel data.                                                            |
| D7        | 9                              | pin 48      | Input                 | Parallel data.                                                            |
