include <../config/config.scad>
UseSTL=false;
UseVitaminSTL=false;
DebugConnectors=false;
DebugCoordinateFrames=false;

NeoPixelMouth_STL();

*translate([0, 0, dw])
	NeoPixel(NeoPixel_Strip8);

