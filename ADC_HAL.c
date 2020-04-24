/*
 * ADC_HAL.c
 *
 *  Created on: Oct 18, 2018
 *      Author: Leyla
 */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ADC_HAL.h>

#define VX_MIDDLE 8000//ABOUT 8000 REALLY (0XFFFF/2)
#define VY_MIDDLE 8000

#define LEFT_THRESHOLD  1500
#define RIGHT_THRESHOLD 10000


void initADC();
void startADC();
void initJoyStick();

Joystick Joystick_construct(){
    Joystick joystick;

    joystick.vxOld = VX_MIDDLE;
    joystick.vyOld = VY_MIDDLE;

    initADC();
    initJoyStick();
    startADC();


    //There are two ways to do this. One is using FSM states, another is keeping the old value of the JS

    // The outputs of the FSM.
    joystick.isTappedRight = false;  // The output of the joystick FSM (true or false).
    joystick.isTappedLeft = false;  // The output of the joystick FSM (true or false).
    joystick.isTappedUp = false;  // The output of the joystick FSM (true or false).
    joystick.isTappedDown = false;  // The output of the joystick FSM (true or false).
}

// Initializing the ADC which resides on SoC
void initADC() {
    ADC14_enableModule();

    // This sets the conversion clock to 3MHz
    ADC14_initModule(ADC_CLOCKSOURCE_ADCOSC,
                     ADC_PREDIVIDER_1,
                     ADC_DIVIDER_1,
                      0
                     );

    // This configures the ADC to store output results
    // in ADC_MEM0 up to ADC_MEM1. Each conversion will
    // thus use two channels.
    ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM1, true);

    // This configures the ADC in manual conversion mode
    // Software will start each conversion.
    ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);
}


void startADC() {
   // Starts the ADC with the first conversion
   // in repeat-mode, subsequent conversions run automatically
   ADC14_enableConversion();
   ADC14_toggleConversionTrigger();
}


// Interfacing the Joystick with ADC (making the proper connections in software)
void initJoyStick() {

    // This configures ADC_MEM0 to store the result from
    // input channel A15 (Joystick X), in non-differential input mode
    // (non-differential means: only a single input pin)
    // The reference for Vref- and Vref+ are VSS and VCC respectively
    ADC14_configureConversionMemory(ADC_MEM0,
                                  ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                  ADC_INPUT_A15,                 // joystick X
                                  ADC_NONDIFFERENTIAL_INPUTS);

    // This selects the GPIO as analog input
    // A15 is multiplexed on GPIO port P6 pin PIN0
    // TODO: which one of GPIO_PRIMARY_MODULE_FUNCTION, or
    //                    GPIO_SECONDARY_MODULE_FUNCTION, or
    //                    GPIO_TERTIARY_MODULE_FUNCTION
    // should be used in place of 0 as the last argument?
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6,
                                               GPIO_PIN0,
                                               GPIO_TERTIARY_MODULE_FUNCTION);


    ADC14_configureConversionMemory(ADC_MEM1,
                                   ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                   ADC_INPUT_A9,                 // joystick y
                                   ADC_NONDIFFERENTIAL_INPUTS);

    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4,
                                               GPIO_PIN4,
                                               GPIO_TERTIARY_MODULE_FUNCTION);




}

void getSampleJoyStick(unsigned *X, unsigned *Y) {
    // ADC runs in continuous mode, we just read the conversion buffers
    *X = ADC14_getResult(ADC_MEM0);

    // TODO: Read the Y channel
    *Y = ADC14_getResult(ADC_MEM1);
}

void refreshJoystick(Joystick *joystick_p)//_p is for pointer
{
    joystick_p->vxOld = joystick_p->vx;//place old values in the memory variables
    joystick_p->vyOld = joystick_p->vy;

    joystick_p->vx = ADC14_getResult(ADC_MEM0);//update current values
    joystick_p->vy = ADC14_getResult(ADC_MEM1);

    if ((joystick_p->vx < LEFT_THRESHOLD) && (joystick_p->vyOld > LEFT_THRESHOLD))
        joystick_p->isTappedLeft = true;
    else
        joystick_p->isTappedLeft = false;
      /*Previous example is equal to saying that
       * if (x)
       * bool y=true;
       *
       * or bool y = x;
       */
}

bool isTiltedLeft(Joystick joystick)//note it is not a pointer for the parameter
{
    if (joystick.vx < LEFT_THRESHOLD)///Combinational -- for individual movements
        return true;
    else
        return false;
    /*Coding note: this function is equal to just placing
     * return(joystick.vx < LEFT_THRESHOLD)
     * in the function
     */

}

bool isTiltedRight(Joystick joystick)//note it is not a pointer for the parameter
{
    if (joystick.vx > RIGHT_THRESHOLD)///Combinational -- for individual movements
        return true;
    else
        return false;
}
