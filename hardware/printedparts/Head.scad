// Connectors

Head_Con_Def = [[0,0,0], [0,0,-1], 0,0,0];

Head_Con_SensorTX = [[13,20,30], [0,1,0], 180,0,0];



module Head_STL() {

    printedPart("printedparts/Head.scad", "Head", "Head_STL()") {

        view(t=[0,0,0],r=[72,0,130],d=400);

        if (DebugCoordinateFrames) frame();
        if (DebugConnectors) {
            connector(Head_Con_Def);
            connector(Head_Con_SensorTX);
        }

        color(Level3PlasticColor) {
            if (UseSTL) {
                import(str(STLPath, "Head.stl"));
            } else {
                Head_Model();
            }
        }
    }
}


module Head_Model()
{
    // local vars
    w  = Head_Width;
    d = Head_Depth;
    h = Head_Height;

    // model
    difference() {
        union() {
            //base
            translate([-w/2, -d/2, 0])
                cube([w,d,dw]);

            // face
            translate([-w/2, d/2-dw, 0])
                cube([w,dw,h]);
        }


    }
}
