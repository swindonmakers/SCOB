// Connectors

LeftLeg_Con_Def = [[0,0,0], [0,0,-1], 0,0,0];


module LeftLeg_STL() {

    printedPart("printedparts/LeftLeg.scad", "Left Leg", "LeftLeg_STL()") {

        view(t=[0,0,0],r=[72,0,130],d=400);

        if (DebugCoordinateFrames) frame();
        if (DebugConnectors) {
            connector(LeftLeg_Con_Def);
        }

        color(Level3PlasticColor) {
            if (UseSTL) {
                import(str(STLPath, "LeftLeg.stl"));
            } else {
                LeftLeg_Model();
            }
        }
    }
}


module LeftLeg_Model()
{
    // local vars

    import("../ref/Leg_LH__repaired_.stl");

    // model
    difference() {
        union() {
            cube([10,10,10]);
        }


    }
}
