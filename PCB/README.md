# PCB

![PCB Render](../Images/pcb-render.png)

## Build Instructions

Install [PCBmodE](https://github.com/boldport/pcbmode). Clone this repo into the "boards" directory within your PCBmodE installation.

### Generate the Board
```
pcbmode -b sdvx --fab -m
```
You should now be able to view the board SVG in /sdvx/build/sdvx.svg.

### Generating Gerbers
```
pcbmode -b sdvx --fab dirtypcb
```
Board files will be output to: /sdvx/build/production/

### Modifying the Design

In PCBmodE, there are two different ways to modify the design: the sdvx.json file itself, or the sdvx.svg output file in the /build/ directory.

The json can be used to define/place components, while the svg can be used to place traces and move components.

After modifying the json, regenerate the board with the -m flag. 

After modifying the svg, extract the changes with the -e flag, and then generate the board with -m.

For more info, check the [PCBmodE Wiki](http://pcbmode.readthedocs.io/en/latest/introduction.html).