#pragma once

class MotionEvent {
public:
    enum ACTION_TYPE {
        ACTION_DOWN = 0,
        ACTION_UP = 1,
        ACTION_MOVE = 2,
        ACTION_CANCEL = 3,
        ACTION_POINTER_DOWN = 5,
        ACTION_POINTER_UP = 6,

        ACTION_MASK = 0xff,
        ACTION_POINTER_INDEX_MASK = 0xff00,
        ACTION_POINTER_INDEX_SHIFT = 8,
    };

    MotionEvent() {};
    virtual ~MotionEvent() {}

public:
    virtual long getTime() = 0;
    virtual int getAction() = 0;
    virtual float getX() = 0;
    virtual float getY() = 0;
    virtual float getX(int idx) = 0;
    virtual float getY(int idx) = 0;
    virtual int getPointerCount() = 0;
};