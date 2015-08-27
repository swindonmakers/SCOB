// Connectors

Hat_Con_Def = [[0,0,0], [0,0,-1], 0,0,0];


module Hat_STL() {

    printedPart("printedparts/Hat.scad", "Hat", "Hat_STL()") {

        view(t=[0,0,0],r=[72,0,130],d=400);

        if (DebugCoordinateFrames) frame();
        if (DebugConnectors) {
            connector(Hat_Con_Def);
        }

        color(Level3PlasticColor) {
            if (UseSTL) {
                import(str(STLPath, "Hat.stl"));
            } else {
                Hat_Model();
            }
        }
    }
}


module Hat_Model()
{
    // local vars

    // model
    difference() {
        union() {
            cube([10,10,10]);
        }


    }
}
