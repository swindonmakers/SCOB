#ifndef SEQUENCES_H
#define SEQUENCES_H

#include <Arduino.h>
#include <FastLED.h>

#define NUM_LEDS 8

class Sequence
{
public:
	Sequence(CRGB(&refleds)[NUM_LEDS]);

	CRGB(*strip)[NUM_LEDS];

	virtual void init();
	virtual void next() = 0;
	int delayMs;
	bool finished;

	void runSync();

protected:
	void off();
};

class StartUpSequence : public Sequence
{
public:
	StartUpSequence(CRGB(&refleds)[NUM_LEDS]);

	void init();
	void next();

private:
	int step;
};

class ScannerSequence : public Sequence
{
public:
	ScannerSequence(CRGB(&refleds)[NUM_LEDS]);

	void init();
	void next();

private:
	int8_t pos;
	int8_t dir;

	uint8_t hue;
};

class RetreatSequence : public Sequence
{
public:
  RetreatSequence(CRGB(&refleds)[NUM_LEDS]);
  
  void init();
  void next();
  
private:
  uint8_t sat;
};

class AdvanceSequence : public Sequence
{
public:
  AdvanceSequence(CRGB(&refleds)[NUM_LEDS]);
  
  void init();
  void next();
  
private:
  uint8_t step;
};

#endif
