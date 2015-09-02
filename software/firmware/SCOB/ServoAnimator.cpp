#include "ServoAnimator.h"

ServoAnimator::ServoAnimator() {

}

void ServoAnimator::initServo(uint8_t num, uint8_t pin, uint8_t center) {
    if (num >= _numServos) return;
    SERVO *s = &_servos[num];

    s->pin = pin;
    s->center = center;
    s->pos = center;
    s->targetPos = center;
    s->startPos = center;
    s->servo = new Servo();
    s->servo->attach(pin);
    s->servo->write(center);
}

void ServoAnimator::moveServosTo(const byte keyframe[SERVOANIMATOR_NUM_SERVOS], unsigned long dur) {
   if (_busy) return;

   _moveStartedAt = millis();
   _busy = true;
   _moveDuration = dur;
   for (uint8_t i=0; i<_numServos; i++) {
       _servos[i].targetPos = _servos[i].center + keyframe[i];

       Serial.print(i);
       Serial.print(':');
       Serial.println(keyframe[i]);
   }

}

void ServoAnimator::setAnimation(const ANIMATION * animation) {
    _animation = (ANIMATION*) animation;
    moveToFrame(0);
}

boolean ServoAnimator::isBusy() {
    return _busy;
}

void ServoAnimator::update() {
  if (!_busy || _moveDuration == 0) return; // nothing to do

  // calc elapsed time
  float t = millis() - _moveStartedAt;
  if (t < 0) // overflow
    t += 4294967295 - _moveStartedAt;

  t = t / _moveDuration;  // t now in range 0..1
  if (t>=1) t = 1;

  // apply easing - smooth in/out
  t = bezierAt(t, 0, 0.6, 0.4, 1);
  if (t>1) t = 1;
  if (t<0) t = 0;

  for (uint8_t i=0; i<_numServos; i++) {
    // update joint positions
    _servos[i].pos = _servos[i].startPos + t*(_servos[i].targetPos - _servos[i].startPos);

    // update servos
    _servos[i].servo->write(_servos[i].pos);
  }

  // see if we're done with the current move
  if (t>=1) {
    _busy = false;
    for (uint8_t i=0; i<_numServos; i++)
      _servos[i].startPos = _servos[i].pos;

    // play next keyFrame?
    nextFrame();
  }
}

void ServoAnimator::moveToFrame(uint8_t frame) {
    if (_animation ==0 || frame >= _animation->numFrames) return;
    _animFrame = frame;
    moveServosTo(_animation->frames[_animFrame][0], _animation->durations[_animFrame]);
}

void ServoAnimator::nextFrame() {
    if (_animation ==0) return;
    _animFrame++;
    moveToFrame(_animFrame);
}
