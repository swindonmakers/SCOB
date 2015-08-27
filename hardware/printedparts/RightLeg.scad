// Connectors

RightLeg_Con_Def = [[0,0,0], [0,0,-1], 0,0,0];


module RightLeg_STL() {

    printedPart("printedparts/RightLeg.scad", "Right Leg", "RightLeg_STL()") {

        view(t=[0,0,0],r=[72,0,130],d=400);

        if (DebugCoordinateFrames) frame();
        if (DebugConnectors) {
            connector(RightLeg_Con_Def);
        }

        color(Level3PlasticColor) {
            if (UseSTL) {
                import(str(STLPath, "RightLeg.stl"));
            } else {
                RightLeg_Model();
            }
        }
    }
}


module RightLeg_Model()
{
    // local vars

    // model
    difference() {
        union() {
            cube([10,10,10]);
        }


    }
}
