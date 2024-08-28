# rally-buttons
Simple c++ code for digital road-books

# keypad matrix
|              | black (col1) | white  (col2) | brown  (col3) | 
|--------------|--------------|---------------|---------------|
| green (row1) | red-r        | yellow-f      | yellow-r      |
| red   (row1) | blue-f       | grey-f        | green-f       |


# Todo 
- [ ] Profile switch 
- [ ] Measure power usage
- [ ] order battery + charger
- [ ] 3d print case
- [ ] code cleanup 
- [ ] prototype of the circuit 
- [ ] experiment with long press / short press 
- [ ] experiment with modes
- [ ] sleep 


# Inspirations 
- https://www.circuitbasics.com/how-to-set-up-a-keypad-on-an-arduino/
- https://randomnerdtutorials.com/esp32-deep-sleep-arduino-ide-wake-up-sources/

# initial pre-modified keypad
| button   | cable 1 | cable 2 |
|----------|---------|---------|
| red-r    | green   | black   |
| yellow-r | green   | red     |
| red-f    | red     | blue    |
| grey-f   | red     | white   |
| green-f  | red     | brown   |
| yellow-f | white   | black   |
| blue-f   | red     | yellow  |