include <config/config.scad>

STLPath = "printedparts/stl/";
VitaminSTL = "vitamins/stl/";

DebugCoordinateFrames = 0;
DebugConnectors = false;

UseSTL = true;

machine("SCOB.scad","SCOB") {

    view(size=[1024,768], t=[0,-8,15], r=[96, 0, 212], d=400);

    markup(section="introduction", markup="BOB-inspired mobile robot designed in OpenSCAD");

    //Top level assembly
    FinalAssembly();
}
