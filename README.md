# Splitting Squares

Simple SDL app that starts with 1 square moving along the screen, collisions with the edge of the window causing splitting and "respawning" of 2 new squares in the center. New squares are smaller and move faster (configurable).

## Running
Builds for both Windows and Linux included in [Releases](https://github.com/t-honk/SplittingSquares/releases)

### Windows
Windows release includes compiled binary and SDL2.dll and should work out of the box

### Linux
Only compiled binary, SDL2 can be installed with package manager

## Building

**SDL2 development headers required to build**

Install varies by system
- Windows using MinGW:
    ```bash
    pacman -S mingw-w64-x86_64-SDL2
    ```
- Linux:
  - pacman:
    ```sh
    pacman -S SDL2
    ```
  - apt:
    ```sh
    apt-get install libsdl2-dev
    ```
  - dnf:
    ```sh
    dnf install SDL2-devel
    ```

Project includes a Makefile with build rules for both Windows (using mingw) and Linux systems.
```bash
make windows
```
or 
```bash
make linux
```
Will build the correct binary for the platform in the "bin" folder.

Program Behavior can be modified using the consts in the top of "main.c" (explained below)

## Configuration Options

- SCREEN_WIDTH
- SCREEN_HEIGHT
- MAX_SQUARES
  - Default: 40,000 | Program exits, prints that MAX_SQUARES was reached ~~Currently crashes after this is exceeded [(TODO)](#TODO)~~
- FPS
  - Default: 144 | FPS Limit, delays main SDL event loop (will affect square speed)
- ANGLE_INCREMENT
  - Default: 10 | How much to increment the angle of the spiral pattern the squares move in
- SHRINK_MODIFIER
  - Default: 0.6 | How large the resulting squares should be after collision
- SPEED_MULTIPLIER
  - Default: 1.1 | What percentage speed increase each new square(s) should get (1.1 = 10%)

## TODO

- [x] ~~Implement error checking for square count to prevent crash~~

- MAYBE: 
  - Add UI elements for config (sliders?)
  - Display FPS
  - Display Square Count