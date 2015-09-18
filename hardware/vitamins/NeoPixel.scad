/*
    Vitamin: Neopixel library

    NeoPixel();
	NeoPixel(NeoPixel_Single);
	NeoPixel(NeoPixle_Strip8);
	NeoPixel(NeoPixle_Strip16);
*/

NeoPixel_Width = 5;
NeoPixel_Depth = 5;
NeoPixel_Height = 1.5;

// getters
function NeoPixel_TypeSuffix(t)      		= t[0];
function NeoPixel_Strip_PcbWidth(t)	 		= t[1];
function NeoPixel_Strip_PcbHeight(t)	 	= t[2];
function NeoPixel_Strip_NumPixels(t)	 	= t[3];
function NeoPixel_Strip_PixelSpacing(t)		= t[4];
function NeoPixel_Strip_MountHoleOff(t) 	= t[5];

// types
//                 TypeSuffix,
NeoPixel_Single = ["Single"];
//                           pcb_w, pcb_h, n_pix, pix_spacing, mountholespacing
NeoPixel_Strip8 = ["Strip8", 51, 10.2, 8, 1.5, 25.5];
NeoPixel_Strip16 = ["Strip16", 102, 10.2, 16, 1.5, 25.5];

NeoPixel_Types = [
	NeoPixel_Single,
	NeoPixel_Strip8,
	NeoPixel_Strip16
];

// Vitamin Catalogue]
module NeoPixel_Catalog() 
{
	for (t = NeoPixel_Types)
		NeoPixel(t);
}

// Connectors
NeoPixel_Con_Def				= DefConUp;

//[ [0,0,0], [0,0,-1], 0, 0, 0];


module NeoPixel(type=NeoPixel_Single)
{
	ts = NeoPixel_TypeSuffix(type);
	
	vitamin("vitamins/NeoPixels.scad", str("NeoPixel ", ts), str("NeoPixel(NeoPixel_", ts, ")")) {
		view(t=[6.9, 13.6, 10.3], r=[72,0,33], d=280);
		
		if (DebugCoordinateFrames) frame();
		
		if (type == NeoPixel_Single) NeoPixelSingle();
		if (type == NeoPixel_Strip8) NeoPixelStrip(type);
		if (type == NeoPixel_Strip16) NeoPixelStrip(type);
	}
}

module NeoPixelSingle()
{
	if (DebugConnectors)
		connector(NeoPixel_Con_Def);
	
	color("White")
	linear_extrude(1.5)
		square([5, 5], center=true);
}

module NeoPixelStrip(type)
{
	if (DebugConnectors) {
		// TODO:
	}
	
	w = NeoPixel_Strip_PcbWidth(type);
	h = NeoPixel_Strip_PcbHeight(type);
	pcb_thickness = 1.5;
	npix = NeoPixel_Strip_NumPixels(type);
	spix = NeoPixel_Strip_PixelSpacing(type);
	hoff = NeoPixel_Strip_MountHoleOff(type);
	
	// PCB
	color("DarkGreen")
	linear_extrude(pcb_thickness)
	difference() {
		square([w, h], center=true);
		
		for (i=[0, 1])
		mirror([i, 0, 0])
		translate([hoff/2, h/2 - 1 - 1, 0])
			circle(d=2);
	}
	
	// Pixels
	for (j=[0,1])
	mirror([j, 0, 0])
	for (i=[0:npix/2 - 1])
	translate([(spix + NeoPixel_Width)/2 + (spix + NeoPixel_Width) * i, -h/2 + 1.3 + NeoPixel_Depth/2, pcb_thickness])
		NeoPixelSingle();

}
