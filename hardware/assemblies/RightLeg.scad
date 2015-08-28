
RightFootAssembly_Con_Servo = [[10.3,50,8.2], [0,-1,0], 0,0,0];

module RightLegAssembly () {

    if (DebugConnectors) {
        connector(RightFootAssembly_Con_Servo);
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
