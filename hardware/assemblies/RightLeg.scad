
RightLeg_Con_Hip = [[10,30,40], [0,0,1], 0,0,0];

module RightLegAssembly () {

    if (DebugConnectors) {
        connector(RightLeg_Con_Hip);
    }

    assembly("assemblies/RightLeg.scad", "Right Leg", str("RightLegAssembly()")) {

        // base part
        Foot_STL();

        // steps
        step(1, "Fix servo to foot") {
            view(t=[0,0,0], r=[52,0,218], d=200);

            attach(Foot_Con_ServoFixing1a, MicroServo_Con_Fixing1) {
                MicroServo();
                attach(rollConnector(MicroServo_Con_Horn,90), DefConDown)
                    ServoHorn();
            }
        }
    }
}
