
LeftLeg_Con_Hip = [[10,30,40], [0,0,1], 0,0,0];

module LeftLegAssembly () {

    if (DebugConnectors) {
        connector(LeftLeg_Con_Hip);
    }

    assembly("assemblies/LeftLeg.scad", "Left Leg", str("LeftLegAssembly()")) {

        // base part
        Foot_STL();

        // steps
        step(1, "Fix servo to foot") {
            view(t=[0,0,0], r=[52,0,218], d=200);

            attach(Foot_Con_ServoFixing1b, MicroServo_Con_Fixing1) {
                MicroServo();
                attach(rollConnector(MicroServo_Con_Horn,-90), DefConDown)
                    ServoHorn();
            }
        }



    }
}
