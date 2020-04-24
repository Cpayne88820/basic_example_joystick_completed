#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include "ADC_HAL.h"
#include "graphics_HAL.h"




// This function initializes all the peripherals except graphics
void initialize();
void ModifyLEDColor(bool leftButtonWasPushed, bool rightButtonWasPushed);

unsigned colormix(unsigned r,unsigned g,unsigned b)
{//-------copied from app.c of basic-graphics-nonblocking
    return ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff);
}


int main(void)
{

    Graphics_Context g_sContext;

    initialize();
    InitGraphics(&g_sContext);//changes initialized graphics colors and shapes.
    draw_Base(&g_sContext);

    unsigned vx, vy;
    unsigned vx_old, vy_old;

    Joystick myJoystick = Joystick_construct();

    while (1)
    {
        refreshJoystick(&myJoystick);//myJoystick found in Joystick_construct() in ADC_HAL.c

        vx = myJoystick.vx;
        vy = myJoystick.vy;
        vx_old = myJoystick.vxOld;
        vy_old = myJoystick.vyOld;

        bool joyStickTappedtoRight = false;
        bool joyStickTappedtoLeft = false;

        bool joyStickTiltedtoRight = false;
        bool joyStickTiltedtoLeft = false;

        drawXY(&g_sContext, vx, vy);


        joyStickTiltedtoLeft = isTiltedLeft(myJoystick);
        joyStickTiltedtoRight = isTiltedRight(myJoystick);

        joyStickTappedtoLeft = myJoystick.isTappedLeft;
        joyStickTappedtoRight = myJoystick.isTappedRight;

        /*if Tapped logic,
 * if vx < left thresh, && vx_old > left thresh, then joystick moves left
 */

        unsigned int r, g, b;
        r = vx/64 ; //must scale ~16000 to 256 increments
        g = 255-r;//makes green complement of r /// ---- copied from App.c of basic_example_graphics_non_blocking
        b = 0;
      //  Graphics_setForegroundColor(&g_sContext, colormix(r,g,b));
    //    Graphics_fillCircle(&g_sContext, 63, 63, 10);



        MoveCircle(&g_sContext, joyStickTappedtoLeft,joyStickTiltedtoRight);///-----Use this function to change smoothness of circle movements

     }
}




void initialize()
{
    // stop the watchdog timer
    WDT_A_hold(WDT_A_BASE);

    // Initialize the timers needed for debouncing
    Timer32_initModule(TIMER32_0_BASE, // There are two timers, we are using the one with the index 0
                       TIMER32_PRESCALER_1, // The prescaler value is 1; The clock is not divided before feeding the counter
                       TIMER32_32BIT, // The counter is used in 32-bit mode; the alternative is 16-bit mode
                       TIMER32_PERIODIC_MODE); //This options is irrelevant for a one-shot timer

    Timer32_initModule(TIMER32_1_BASE, // There are two timers, we are using the one with the index 1
                       TIMER32_PRESCALER_1, // The prescaler value is 1; The clock is not divided before feeding the counter
                       TIMER32_32BIT, // The counter is used in 32-bit mode; the alternative is 16-bit mode
                       TIMER32_PERIODIC_MODE); //This options is irrelevant for a one-shot timer

/*
    initADC();
    initJoyStick();
    startADC();
*/
}


