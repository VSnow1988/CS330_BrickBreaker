# CS330_BrickBreaker
CS330 SNHU week 8 project: Brick-Breaker with OpenGL.
Downlod these files to edit or run as a Visual Studio project.

A 2D brick-breaker arcade style game.
Uses vectors to hold balls and bricks and draw them on-screen.
Colors are random shades of purple. I sure love purple.

How to Play:
Run program and use space bar to launch a ball in a random direction.
Balls will collide with bricks or each other and cause colors to change or bricks to disappear and rebound.

## Setting up:
Drop all files into a Visual Studio project and extract the .zip files as directories of the same name.

This code uses the GLFW library. https://www.glfw.org/

## Issues to be fixed:
- The collision behavior does not work as expected.
- Balls sometimes bounce when no brick apparently exists.
- Physics result in balls being stuck sometimes moving in a straight line.
- Code documentation is very lacking.
- Security features like assertions, automated tests, and error logging would be very useful.
- Some variables names don't make sense and should change to be more intuitive.
- An ArrayList may be more efficient and secure than a vector for holding world objects.
