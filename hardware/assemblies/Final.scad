
module FinalAssembly () {

    assembly("assemblies/Final.scad", "Final", str("FinalAssembly()")) {

    // base part
    Head_STL();

    // steps
    step(1, "Insert sensor") {
            view(t=[0,0,0], r=[52,0,218], d=400);

            attach(Head_Con_SensorTX, HCSR04UltrasonicSensor_Con_TXCan)
                HCSR04UltrasonicSensor();
        }



    }
}
