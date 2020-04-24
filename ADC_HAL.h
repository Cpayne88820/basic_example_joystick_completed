/*
 * ADC_HAL.h
 *
 *  Created on: Oct 18, 2018
 *      Author: Leyla
 */

#ifndef ADC_HAL_H_
#define ADC_HAL_H_

struct _Joystick
{

    unsigned int vx;
    unsigned int vy;

    unsigned int vxOld;
    unsigned int vyOld;

    //There are two ways to do this. One is using FSM states, another is keeping the old value of the JS

    // The outputs of the FSM.
    bool isTappedRight;  // The output of the joystick FSM (true or false).
    bool isTappedLeft;  // The output of the joystick FSM (true or false).
    bool isTappedUp;  // The output of the joystick FSM (true or false).
    bool isTappedDown;  // The output of the joystick FSM (true or false).
};
typedef struct _Joystick Joystick;

Joystick Joystick_construct();
void getSampleJoyStick(unsigned *X, unsigned *Y);
void refreshJoyStick(Joystick *);
bool isTiltedLeft(Joystick joystick);
bool isTiltedRight(Joystick joystick);

#endif /* ADC_HAL_H_ */
