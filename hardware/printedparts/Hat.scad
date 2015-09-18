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

    inset = 16/2;  // amount the top surface of the hat is inset on each side

    font = "Liberation Sans";

    // model
    //render()
    difference() {
        union() {
            // top
            hull () {
                translate([-w/2+inset, -d/2+inset, 10])
                    roundedCube([w-2*inset,d-2*inset,dw], 5);

                translate([-w/2, -d/2, 0])
                    roundedCube([w,d,dw], 5);
            }

            // inner locating rim
            translate([-w/2 + dw + tol, -d/2 + dw, -dw])
                roundedCube([w - 2*dw - 2*tol, d - 2*dw - tol, 2*dw], 6);

            // upper back wall
            translate([-w/2+5 + tol, -d/2, -h/2 + dw])
                cube([w-10 - 2*tol,dw,h/2]);

            // lower back wall
            translate([-w/2+5 + tol, -d/2, -h + 30 + dw])
                roundedRectY([w-10 - 2*tol,dw,h-30], 5);

            // emboss some text
            translate([-19.5, -d/2 + 0.5, -h/3])
                rotate([90,0,0])
                linear_extrude(1)
                text("SCOB", size=10, font=font);
        }

        // hollow out the top
        hull () {
            translate([-w/2+inset, -d/2+inset, 10 - dw])
                roundedCube([w-2*inset,d-2*inset,dw], 5);

                translate([-w/2 + 2*dw + tol, -d/2 + 2*dw + tol, -dw-eta])
                    roundedCube([w - 4*dw - 2*tol, d - 4*dw - 2*tol, dw], 6);
        }

        // finger notch in back panel
        translate([0, -d/2, -h + 30 - dw])
            scale([1,1,0.8])
            rotate([90,0,0])
            cylinder(r=10, h=10, center=true);
    }
}
