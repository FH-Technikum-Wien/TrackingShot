In development, needs A LOT of restructuring and clean-up.

# TrackingShot

A small project for that implementes a tracking shot using OpenGL.

Uses https://github.com/FH-Technikum-Wien/kd-tree for generating the kd-tree and raycasting.

## Controls

### In Free-Fly
|  |  |
| --- | --- |
| `WASD` | Moving around |
| `SPACE` | Moving up |
| `LEFT-CTRL` | Moving down |

### General
|  |  |
| --- | --- |
| `ESC` | Close program |
| `1,2,3,4` | Change movement speed (1 = slow, 2 = normal, 3 = fast, 4 = sonic) |
| `TAB` | Switch between "Free-Fly" and tracking shot |
| `E` | Add custom path point |
| `R` | Delete last added custom path point |
| `DELETE` | Deletes the custom path and restores the default path |

### Path controls
|  |  |
| --- | --- |
| `F1` | Go to first path point |
| `Left-Arrow` | Go to previous path point |
| `Right-Arror` | Go to next path point |
| `F` | Focus (jump to) current path point |

### Special
|  |  |
| --- | --- |
| `F5` | Enable/Disable anti-aliasing |
| `F6` | Switch anti-aliasing mode (Will close and reopen the window) |
| `K` | Show kd-tree wireframe |
| `MOUSE LEFT` | Shoot intersecting ray into scene |
| `MOUSE RIGHT` | Remove intersection information from scene |
