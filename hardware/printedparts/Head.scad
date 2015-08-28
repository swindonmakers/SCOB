// Connectors

Head_Con_Def = [[0,0,0], [0,0,-1], 0,0,0];

Head_Con_SensorTX = [[13,Head_Depth/2-dw-4,45], [0,1,0], 180,0,0];
Head_Con_SensorRX = [[-13,Head_Depth/2-dw-4,45], [0,1,0], 180,0,0];

Head_Con_LeftServo = [[-Head_ServoSpacing/2, -MicroServo_FixingSpacing/2,-2], [0,0,1], -90,0,0];
Head_Con_RightServo = [[Head_ServoSpacing/2, -MicroServo_FixingSpacing/2,-2], [0,0,1], -90,0,0];

Head_Con_PowerSwitch = [[0,-Head_Depth/2 + dw, 10], [0,-1,0], 0,0,0];

Head_Con_Arduino = [[0,0,30], [0,0,-1], 90,0,0];

Head_Con_BatteryPack = [[0,-Head_Depth/2 + 10,20], [0,0,-1], 0,0,0];

// Pin fixings
Head_Con_Mouth = [[0, Head_Depth/2, 20], [0,-1,0], 0,0,0];
Head_Con_Nose = [[0, Head_Depth/2, 30], [0,-1,0], 0,0,0];
Head_Con_LeftArm = [[-Head_Width/2, 0, 20], [1,0,0], 0,0,0];
Head_Con_LeftEar = [[-Head_Width/2, 0, 40], [1,0,0], 0,0,0];
Head_Con_RightArm = [[Head_Width/2, 0, 40], [-1,0,0], 0,0,0];
Head_Con_RightEar = [[Head_Width/2, 0, 20], [-1,0,0], 0,0,0];

module Head_STL() {

    printedPart("printedparts/Head.scad", "Head", "Head_STL()") {

        view(t=[0,0,0],r=[72,0,130],d=400);

        if (DebugCoordinateFrames) frame();
        if (DebugConnectors) {
            connector(Head_Con_Def);
            connector(Head_Con_SensorTX);
            connector(Head_Con_LeftServo);
            connector(Head_Con_RightServo);
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

    $fn=32;

    // model
    render()
    difference() {
        union() {
            //base
            translate([-w/2, -d/2, 0])
                roundedCube([w,d,dw], 5);

            // walls
            translate([-w/2, -d/2, 0])
                roundedCube([w,d,h-dw], 5, shell=dw);
        }

        // notch the back to allow access - lid/cap will cover this region
        translate([-(w-10)/2, -d/2-1, 30])
            roundedRectY([w-10, 10, h],5);

        // holes for ultrasonic sensor
        attach(Head_Con_SensorTX, DefConUp)
            cylinder(r=HCSR04UltrasonicSensor_CanOD/2 + 0.5, h=50);
        attach(Head_Con_SensorRX, DefConUp)
            cylinder(r=HCSR04UltrasonicSensor_CanOD/2 + 0.5, h=50);

        // mouth fixing
        attach(Head_Con_Mouth, DefConUp)
            cylinder(r=PinDiameter/2, h=10, center=true);

        // nose fixing
        *attach(Head_Con_Nose, DefConUp)
            cylinder(r=PinDiameter/2, h=10, center=true);

        // left arm + ear
        attach(Head_Con_LeftArm, DefConUp)
            cylinder(r=PinDiameter/2, h=10, center=true);
        attach(Head_Con_LeftEar, DefConUp)
            cylinder(r=PinDiameter/2, h=10, center=true);

        // right arm + ear
        attach(Head_Con_RightArm, DefConUp)
            cylinder(r=PinDiameter/2, h=10, center=true);
        attach(Head_Con_RightEar, DefConUp)
            cylinder(r=PinDiameter/2, h=10, center=true);

        // cable ways - front + back
        translate([-8, d/2 - 10, -2])
            cube([16,6,10]);

        translate([-8, -d/2 + 4, -2])
            cube([16,6,10]);

        // power switch
        attach(Head_Con_PowerSwitch, DefConUp)
            cylinder(r=PinDiameter/2, h=10, center=true);

        // servo mounts
        attach(Head_Con_LeftServo, MicroServo_Con_Fixing1)
            MicroServo_BodySubVolume(tol=0);
        attach(Head_Con_RightServo, MicroServo_Con_Fixing1)
            MicroServo_BodySubVolume(tol=0);

        // servo screw fixings
        attach(Head_Con_LeftServo, DefConUp)
            cylinder(r=1, h=10, center=true);

        attach(Head_Con_RightServo, DefConUp)
            cylinder(r=1, h=10, center=true);

        attach(Head_Con_LeftServo, DefConDown)
            translate([MicroServo_FixingSpacing,0,0])
            cylinder(r=1, h=10, center=true);

        attach(Head_Con_RightServo, DefConDown)
            translate([MicroServo_FixingSpacing,0,0])
            cylinder(r=1, h=10, center=true);

    }
}
