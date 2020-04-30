# Centronics Printer Emulator for Arduino Uno

## TODO: Adapting away from Arduino Mega
* [x] Move Everything to Serial
  * [x] Remove LCD Screen
  * [x] Remove SD Card
* [x] Remap Mega Pins to Uno Pins
* [ ] Rework Serial Interface


## Parallel interface

| Name      | DB25 connector | 26 IDC (TRS 80 Model 100) | Arduino pin | Arduino pin direction | Notes                                                                     |
|-----------|----------------|---------------------------|-------------|-----------------------|---------------------------------------------------------------------------|
| Strobe    | 1              | 1                         | 2           | Input                 | Pullup enabled. Attached to falling edge interrupt.                       |
| Error     | 15             |                           | A1 (15)     | Output                | Not used. Forced high.                                                    |
| Select    | 13             |                           | A2 (16)     | Output                | Not used. Forced high.                                                    |
| Paper Out | 12             |                           | A3 (17)     | Output                | Not used. Forced low.                                                     |
| Busy      | 11             | 21                        | A4 (18)     | Output                | Set high on the falling edge of Strobe. Set low after acknowledging data. |
| Ack       | 10             |                           | A5 (19)     | Output                | Generate falling edge to acknowledge data.                                |
| D0        | 2              | 3                         | 3           | Input                 | Parallel data.                                                            |
| D1        | 3              | 4                         | 4           | Input                 | Parallel data.                                                            |
| D2        | 4              | 5                         | 5           | Input                 | Parallel data.                                                            |
| D3        | 5              | 6                         | 6           | Input                 | Parallel data.                                                            |
| D4        | 6              | 7                         | 7           | Input                 | Parallel data.                                                            |
| D5        | 7              | 8                         | 8           | Input                 | Parallel data.                                                            |
| D6        | 8              | 9                         | 9           | Input                 | Parallel data.                                                            |
| D7        | 9              | 10                        | 10          | Input                 | Parallel data.                                                            |
