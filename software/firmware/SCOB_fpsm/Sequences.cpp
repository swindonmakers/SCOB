#include <Arduino.h>
#include <FastLED.h>
#include "Sequences.h"


Sequence::Sequence(CRGB(&refleds)[NUM_LEDS]) {
	strip = &refleds;
}

void Sequence::init() {
	delayMs = 1000;
	finished = false;
}

void Sequence::runSync() {
	init();
	FastLED.show();
	delay(delayMs);

	while (!finished) {
		next();
		FastLED.show();
		delay(delayMs);
	}
}

void Sequence::off() {
	for (int i = 0; i<NUM_LEDS; i++) {
		(*strip)[i].r = 0;
		(*strip)[i].g = 0;
		(*strip)[i].b = 0;
	}
}




StartUpSequence::StartUpSequence(CRGB(&refleds)[NUM_LEDS]) : Sequence(refleds) {}

void StartUpSequence::init() {
	Sequence::init();
	step = 0;
	off();
}

void StartUpSequence::next() {
	if (step == 4) return;

	((*strip)[3 - step]).r = 255;
	((*strip)[4 + step]).r = 255;

	delayMs = 500 + step * 250;

	step++;
	if (step == 4)
		finished = true;
}

ScannerSequence::ScannerSequence(CRGB(&refleds)[NUM_LEDS]) : Sequence(refleds) {}

void ScannerSequence::init() {
	Sequence::init();
	pos = 0;
	dir = 1;
	hue = random(0, 255);
	delayMs = 100;
	off();
	(*strip)[pos] = CHSV(hue, 255, 255);
}

void ScannerSequence::next() {
	(*strip)[pos] = CRGB(0, 0, 0);
	pos += dir;
	if (pos == NUM_LEDS || pos == -1) {
		dir = -dir;
		pos += dir;
		hue = random(0, 255);
	}
	(*strip)[pos] = CHSV(hue, 255, 255);
}


RetreatSequence::RetreatSequence(CRGB(&refleds)[NUM_LEDS]) : Sequence(refleds) {}

void RetreatSequence::init() {
  Sequence::init();
  sat = 0;
  delayMs = 50;
  off();
}

void RetreatSequence::next() {
  sat += 10;
  if (sat > 255) sat = 0;
  for (int i=0; i<NUM_LEDS; i++)
    (*strip)[i].r = sat;
}

AdvanceSequence::AdvanceSequence(CRGB(&refleds)[NUM_LEDS]) : Sequence(refleds) {}

void AdvanceSequence::init() {
  Sequence::init();
  step = 0;
  delayMs = 100;
  off();
}

void AdvanceSequence::next() {
  off();
  (*strip)[3-step].g = 255;
  (*strip)[3+step].g = 255;
  step++;
  if (step == 4)
    step = 0;
}
