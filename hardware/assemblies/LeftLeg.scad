
LeftFootAssembly_Con_Servo = [[10.3,10,8.2], [0,1,0], 0,0,0];

module LeftLegAssembly () {

    if (DebugConnectors) {
        connector(LeftFootAssembly_Con_Servo);
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
