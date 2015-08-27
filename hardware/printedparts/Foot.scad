// Connectors

Foot_Con_Def = [[0,0,0], [0,0,-1], 0,0,0];

Foot_Con_ServoFixing1a = [[tw/2, 35 + 2, dw + MicroServo_Width/2], [0,-1,0], 0,0,0];
Foot_Con_ServoFixing1b = [[tw/2, 25 - 2, dw + MicroServo_Width/2], [0,1,0], 0,0,0];


module Foot_STL() {
    printedPart("printedparts/Foot.scad", "Foot", "Foot_STL()") {

        view(t=[0,0,0],r=[72,0,130],d=400);

        if (DebugCoordinateFrames) frame();
        if (DebugConnectors) {
            connector(Foot_Con_Def);
            connector(Foot_Con_ServoFixing1a);
            connector(Foot_Con_ServoFixing1b);
        }

        color(Level3PlasticColor) {
            if (UseSTL) {
                import(str(STLPath, "Foot.stl"));
            } else {
                Foot_Model();
            }
        }
    }

    // ref
    *color([0,0,0,0.2])
        import("../ref/Foot__repaired_.stl");
}


module Foot_Model()
{

    // local vars
    w = 45;
    d = 60;
    h = dw;


    // model
    difference() {
        union() {
            // foot plate
            roundedRect([w,d,h], 10);

            // servo holder
            translate([0, d/2 - 5, dw-eta])
                cube([tw, 10, MicroServo_Width]);

            translate([tw + MicroServo_Depth, d/2 - 5, dw-eta])
                cube([tw, 10, MicroServo_Width]);
        }


    }
}
