// Connectors
NeoPixelMouth_Con = [[0,3,0], [0,0,-1], 0,0,0];


module NeoPixelMouth_STL() {

    printedPart("printedparts/NeoPixelMouth.scad", "NeoPixelMouth", "NeoPixelMouth_STL()") {

        view(t=[18,4,0],r=[37.5,0,16],d=140);

        if (DebugCoordinateFrames) frame();
        if (DebugConnectors) {
            connector(NeoPixelMouth_Con);
        }

        color(Level3PlasticColor) {
            if (UseSTL) {
                import(str(STLPath, "NeoPixelMouth.stl"));
            } else {
                NeoPixelMouth_Model();
            }
        }
    }
}

module NeoPixelMouth_Model()
{
	type = NeoPixel_Strip8;
	tol = 0.5;
	w = NeoPixel_Strip_PcbWidth(type) + 2 * dw + tol;
	d = NeoPixel_Strip_PcbHeight(type) + dw + tol;
	h_int = 1.5 + NeoPixel_Height + 1.75;
	npix = NeoPixel_Strip_NumPixels(type);
	spix = NeoPixel_Strip_PixelSpacing(type);
	hoff = NeoPixel_Strip_MountHoleOff(type);

	// Connector Pin
	render()
	translate([0, d/2 - dw/2 - 2.5, 0])
	rotate([0, 180, 0])
		pin(h=5);


	// Back
	translate([0, -dw/2, 0])
	linear_extrude(1.2)
	difference() {
		square([w,d], center=true);
		
		for(i = [-1,1])
		translate([i * 10, d/2 - 2.5, 0])
			circle(d=PinDiameter);
	}
	
	// Walls
	translate([0, -dw/2, 0])
	linear_extrude(1.2 + h_int)
	difference() {
		square([w,d], center=true);
		
		translate([0, dw/2, 0])
			square([w-2*dw, d-dw], center=true);
	}

	// Front
	translate([0, -dw/2, 1.2 + h_int])
	linear_extrude(1.2)
	difference() {
		square([w,d], center=true);
			
		// Teeth
		for (j=[0,1])
		mirror([0, j, 0])
		mirror([j, 0, 0])
		for (i=[0:npix/2 - 1])
		translate([(spix + NeoPixel_Width)/2 + (spix + NeoPixel_Width) * i, 0, 0])
		mirror([0, i%2 == 0 ? 1 : 0, 0])
			Tooth(6.5);
	}



}

module Tooth(size)
{
	translate([-size/2, -size/2, 0])
	scale(size)
		polygon(points = [[0,0], [1,0], [0.5, 1]]);
}
