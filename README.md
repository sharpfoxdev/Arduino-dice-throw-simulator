# Arduino dice throw simulator
Random number generator which simulates throws of polyhedral dices on arduino, utilizing 7 segment display and buttons of arduino funshield. http://kabinet.fyzika.net/dilna/ARDUINO/funduino-popis.php

Code simulates dice throws in Advanced Dungeons & Dragons game. The game uses various types of polyhedral dices with different numbers of sides (d4, d6, d8, ...). Furthermore, a generated random number represents a sum of multiple throws. E.g., 3d6 means that player should throw 3 times using a dice with 6 sides (cube) and take the sum of these three throws (which is a number between 3 and 18).

The dice is controlled by 3 buttons and display the results on the 4-digit LED display. It operates in two modes. In normal mode it displays last generated number on the LED display. In configuration mode it displays the type (and repetition) of dice being simulated. First digit (1-9) displays number of throws, second digit displays symbol 'd', and the remaining two digits display the type of dice (d4, d6, d8, d10, d12, d20, and d100 should be supported; d100 is denoted as '00' on the LED display).

Button 1
- switches the dice to normal mode
- whilst pressed down, the random data are gathered (result is being generated)
- when the button is released, a new random result is displayed

Button 2
- switches the dice to configuration mode
- increments the number of throws (if 9 is exceeded, the number returns to 1)

Button 3
- switches the dice to configuration mode
- changes the dice type (dices d4-d100 from the list above are cycled)
- It shows small animation on the display


