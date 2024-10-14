# rally-buttons
Simple c++ code for digital road-books

# keypad matrix v1
|              | black (col1) | white  (col2) | brown  (col3) | 
|--------------|--------------|---------------|---------------|
| green (row1) | red-r        | yellow-f      | yellow-r      |
| red   (row1) | blue-f       | grey-f        | green-f       |

# keypad matrix v2
|                   | black (col1, p3) | white  (col2, p2) | brown  (col3, p1) | yellow (col4, p4) | 
|-------------------|------------------|-------------------|-------------------|-------------------|
| green (row1, p10) | red-r            | yellow-f          | yellow-r          | red-f             |
| red   (row2, p8)  | blue-f           | grey-f            | green-f           |                   |

# keypad matrix v3
|                   | grey (col1, p3) | white  (col2, p2) | brown  (col3, p1) | yellow (col4, p4) | 
|-------------------|-----------------|-------------------|-------------------|-------------------|
| green (row1, p10) | red-r           | yellow-f          | yellow-r          | red-f             |
| red   (row2, p8)  | blue-f          | grey-f            | green-f           |                   |


# keypad matrix v4
|                  | grey (col1, p2) | white  (col2, p1) | brown  (col3, p0) | yellow (col4, p4) | 
|------------------|-----------------|-------------------|-------------------|-------------------|
| green (row1, p8) | red-r           | yellow-f          | yellow-r          | red-f             |
| red   (row2, p6) | blue-f          | grey-f            | green-f           |                   |


# keypad matrix v5
|                  | grey (col1, p1) | white  (col2, p0) | brown  (col3, p4) | yellow (col4, p5) | 
|------------------|-----------------|-------------------|-------------------|-------------------|
| green (row1, p2) | red-r           | yellow-f          | yellow-r          | red-f             |
| red   (row2, p3) | blue-f          | grey-f            | green-f           |                   |

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
| red-r    | black   | green   |
| yellow-f | black   | white   |
| yellow-r | red     | green   |
| red-f    | red     | blue    |
| grey-f   | red     | white   |
| green-f  | red     | brown   |
| blue-f   | red     | yellow  |

# initial pre-modified keypad keypad matrix proposal
|                   | green (col1, p3) | blue  (col2, p2) | white  (col3, p1) | brown (col4, p0) | yellow (col5, p4) | 
|-------------------|------------------|------------------|-------------------|------------------|-------------------|
| red   (row2, p8)  | blue-f           | grey-f           | green-f           |                  |                   |
| black (row1, p10) | red-r            | yellow-f         | yellow-r          | red-f            | red-f             |