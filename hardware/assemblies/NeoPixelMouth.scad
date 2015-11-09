NeoPixelMouthAssembly_Con = NeoPixelMouth_Con;

module NeoPixelMouthAssembly() {

    if (DebugConnectors) {
        connector(NeoPixelMouthAssembly_Con);
    }

    assembly("assemblies/NeoPixelMouth.scad", "Neo Pixel Mouth", str("NeoPixelMouthAssembly()")) {

        // base part
        NeoPixelMouth_STL();

        // steps
        step(1, "Insert Neo Pixel Strip") {
            view(t=[0,0,0], r=[52,0,218], d=200);

			attach(DefCon, [[0, 0, -dw], [0, 0, 1], 0, 0, 0])
				NeoPixel(NeoPixel_Strip8);

        }
    }
}
