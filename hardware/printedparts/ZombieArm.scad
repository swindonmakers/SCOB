// Connectors

ZombieArm_Con_Def = [[0,0,0], [0,0,-1], 0,0,0];


module ZombieArm_STL() {

    printedPart("printedparts/ZombieArm.scad", "Zombie Arm", "ZombieArm_STL()") {

        view(t=[0,0,0],r=[72,0,130],d=400);

        if (DebugCoordinateFrames) frame();
        if (DebugConnectors) {
            connector(ZombieArm_Con_Def);
        }

        color(Level3PlasticColor) {
            if (UseSTL) {
                import(str(STLPath, "ZombieArm.stl"));
            } else {
                ZombieArm_Model();
            }
        }
    }
}


module ZombieArm_Model()
{
    render()
	rotate([0, 0, 90])
		pin(side=true, h=5);

	translate([wrist_dia/2, 0, 0]) {
		linear_extrude(5) {
			hull()
			{
				square([wrist_dia, wrist_dia], center=true);
				translate([arm_len, 5, 0])
					circle(d=1.5*wrist_dia/2);
			}

            // forearm
            hull()
			{
				translate([arm_len, 5, 0])
					circle(d=1.5*wrist_dia/2);
                translate([arm_len, arm_len +5, 0])
					circle(d=wrist_dia/2);
			}
		}

		linear_extrude(3) {
			translate([arm_len, arm_len + 5, 0])
                rotate([0,0,90])
				FlexyArmHand();
		}
	}
}
