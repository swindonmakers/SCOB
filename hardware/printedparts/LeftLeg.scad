// Connectors

LeftLeg_Con_Hip = [[13/2-2.5, 22, 0], [0,0,1], 0,0,0];
LeftLeg_Con_Ankle = [[13/2+2.5, dw , 22], [0,1,0], 180,0,0];


module LeftLeg_STL() {

    printedPart("printedparts/LeftLeg.scad", "Left Leg", "LeftLeg_STL()") {

        view(t=[0,0,0],r=[72,0,130],d=400);

        if (DebugCoordinateFrames) frame();
        if (DebugConnectors) {
            connector(LeftLeg_Con_Hip);
            connector(LeftLeg_Con_Ankle);
        }

        color(Level3PlasticColor) {
            if (UseSTL) {
                import(str(STLPath, "LeftLeg.stl"));
            } else {
                LeftLeg_Model();
            }
        }
    }

    //ref
    *color([0,0,0,0.5])
        import("../ref/Leg_LH__repaired_.stl");
}


module LeftLeg_Model()
{
    // local vars
    w = 13;
    d = 25.5;
    h = 25.5;
    r1 = 3;
    r2 = 6;

    offset = 2.5;
    d1 = 22;
    d2 = d1 - 12.4;

    // model
    difference() {
        union() {
            // base
            linear_extrude(dw)
                difference() {
                    hull() {
                        square([w,1]);
                        translate([r1, d-r1]) circle(r1);
                        translate([w-r1, d-r1]) circle(r1);
                    }

                    // servo fixings
                    translate([w/2 - offset, d1]) circle(2.5/2);
                    translate([w/2 - offset, d2]) circle(2.5/2);
                }


            // riser
            rotate([90,0,0])
                translate([0,0,-dw])
                linear_extrude(dw)
                difference() {
                    hull() {
                        square([w,1]);
                        translate([r2, d-r2]) circle(r2);
                        translate([w-r1, d-r1]) circle(r1);
                    }

                    // servo fixings
                    translate([w/2 + offset, d1]) circle(2.5/2);
                    translate([w/2 + offset, d2]) circle(2.5/2);
                }

            // fillets
            translate([perim,dw,dw])
                rotate([0,-90,0])
                right_triangle(6, 6, 2perim, center=true);

            translate([w-perim,dw,dw])
                rotate([0,-90,0])
                right_triangle(4, 4, 2perim, center=true);
        }


    }
}
