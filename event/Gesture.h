#pragma once

class Gesture {
public:
	static Gesture* PRESS;
	static Gesture* LONG_PRESS;
	static Gesture* TAP;
	static Gesture* DOUBLE_TAP;
};

class Press : public Gesture {};
class LongPress : public Gesture {};
class Tap : public Gesture {};
class DoubleTap : public Gesture {};

