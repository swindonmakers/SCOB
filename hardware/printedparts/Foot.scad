// Connectors

Foot_Con_Def = [[0,0,0], [0,0,-1], 0,0,0];


module Foot_STL() {

    printedPart("printedparts/Foot.scad", "Foot", "Foot_STL()") {

        view(t=[0,0,0],r=[72,0,130],d=400);

        if (DebugCoordinateFrames) frame();
        if (DebugConnectors) {
            connector(Foot_Con_Def);
        }

        color(Level3PlasticColor) {
            if (UseSTL) {
                import(str(STLPath, "Foot.stl"));
            } else {
                Foot_Model();
            }
        }
    }
}


module Foot_Model()
{
    // local vars

    // model
    difference() {
        union() {
            cube([10,10,10]);
        }


    }
}
