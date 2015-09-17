// Connectors
FlexyArm_Con_Def = [[0,0,2.5], [-1,0,0], 90,0,0];

wrist_dia = 10;
finger_dia = 8;
finger_spread = 11;
finger_len_short = 15;
finger_len_long = 22;
arm_len = 20;


module FlexyArm_STL() {

    printedPart("printedparts/FlexyArm.scad", "FlexyArm", "FlexyArm_STL()") {

        view(t=[18,4,0],r=[37.5,0,16],d=140);

        if (DebugCoordinateFrames) frame();
        if (DebugConnectors) {
            connector(FlexyArm_Con_Def);
        }

        color(Level2PlasticColor) {
            if (UseSTL) {
                import(str(STLPath, "FlexyArm.stl"));
            } else {
                FlexyArm_Model();
            }
        }
    }
}

module FlexyArm_Model()
{
	render()
	rotate([0, 0, 90])
		pin(side=true, h=5);

	translate([wrist_dia/2, 0, 0]) {
		linear_extrude(5) {
			hull()
			{
				square([wrist_dia, wrist_dia], center=true);
				translate([arm_len, 0, 0])
					circle(d=wrist_dia/2);
			}
		}

		linear_extrude(3) {
			translate([arm_len, 0, 0])
				FlexyArmHand();
		}
	}
}

module FlexyArmHand() 
{
	hull() {
		circle(d=wrist_dia);
		translate([finger_len_short, finger_spread, 0])
			circle(d=finger_dia);
	}

	hull() {
		circle(d=wrist_dia);
		translate([finger_len_long, 0, 0])
			circle(d=finger_dia);
	}

	hull() {
		circle(d=wrist_dia);
		translate([finger_len_short, -finger_spread, 0])
			circle(d=finger_dia);
	}
}