// Connectors

Hat_Con_Def = [[0,0,0], [0,0,-1], 0,0,0];


module Hat_STL() {

    printedPart("printedparts/Hat.scad", "Hat", "Hat_STL()") {

        view(t=[0,0,0],r=[72,0,130],d=400);

        if (DebugCoordinateFrames) frame();
        if (DebugConnectors) {
            connector(Hat_Con_Def);
        }

        color(Level2PlasticColor) {
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
    w  = Head_Width;
    d = Head_Depth;
    h = Head_Height;

    $fn=32;

    tol = 0.3; // fitting tolerance

    // model
    render()
    difference() {
        union() {
            // top
            hull () {
                translate([-w/2+5, -d/2+5, 10])
                    roundedCube([w-10,d-10,dw], 5);

                translate([-w/2, -d/2, 0])
                    roundedCube([w,d,dw], 5);
            }


            // upper back wall
            translate([-w/2+5 + tol, -d/2, -h/2 + dw])
                cube([w-10 - 2*tol,dw,h/2]);

            // lower back wall
            translate([-w/2+5 + tol, -d/2, -h + 30 + dw])
                roundedRectY([w-10 - 2*tol,dw,h-30], 5);
        }
    }
}
