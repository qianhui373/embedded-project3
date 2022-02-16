/*
 * ADC_HAL.h
 *
 *  Created on: Oct 18, 2018
 *      Author: Leyla
 */

#ifndef ADC_HAL_H_
#define ADC_HAL_H_
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>





////////////////////////////////////////////////////////////


typedef enum  {left, notleft} _joystickleftstates;////////////////////////////////////////
typedef enum  {right, notright} _joystickrightstates;
typedef enum  {down, notdown} _joystickdownstates;
typedef enum  {up, notup} _joystickupstates;
typedef enum  {upleft, notupleft} _joystickupleftstates;
typedef enum  {upright, notupright} _joystickuprightstates;
typedef enum  {still, notstill} _joystickstillstates;


///////////////////////////////////
struct _Joystick
{
    int x; //reading of the joystick
    int y;


   _joystickleftstates leftState;
   _joystickrightstates rightState;
   _joystickdownstates downState;
   _joystickupstates upState;
   _joystickupleftstates upleftState;
   _joystickuprightstates uprightState;
   _joystickstillstates stillState;
    bool isTappedToLeft;
    bool isTappedToRight;
    bool isTappedToDown;
    bool isTappedToUp;

};
typedef struct _Joystick Joystick;

//////////////////////////////////////////accle
struct _Accel
{
    //raw data
    int x; //reading of the accel  x-axis
    int y; //y-axis
    int z;  //z-axis



};
typedef struct _Accel Accel;

//////////////////////////////////////////////////////
Accel Accel_construct();

void Accel_refresh(Accel *);

void initADC();
void startADC();


bool Accel_isMovedToLeft(Accel *);
bool Accel_isMovedToLefta(Accel *);
bool Accel_isMovedToLeftb(Accel *);

bool Accel_isMovedToRight(Accel *);
bool Accel_isMovedToRighta(Accel *);
bool Accel_isMovedToRightb(Accel *);

bool Accel_isMovedToUp(Accel *);
bool Accel_isMovedToUpa(Accel *);
bool Accel_isMovedToUpb(Accel *);

bool Accel_isMovedToDown(Accel *);
bool Accel_isMovedToDowna(Accel *);
bool Accel_isMovedToDownb(Accel *);


















////////////////////////////////////////////////////////////////
Joystick Joystick_construct();

void Joystick_refresh(Joystick *);
//bool Joystick_isPressedToLeft(Joystick *);
bool Joystick_isTappedToLeft(Joystick *);
bool Joystick_isTappedToRight(Joystick *);
bool Joystick_isTappedToDown(Joystick *);
bool Joystick_isTappedToUp(Joystick *);

bool Joystick_isPushedToLeft(Joystick *);
bool Joystick_isPushedToRight(Joystick *);
bool Joystick_isPushedToUp(Joystick *);
bool Joystick_isPushedToDown(Joystick *);

bool Joystick_isPushedToUpleft(Joystick *);
bool Joystick_isPushedToUpright(Joystick *);

bool Joystick_isStill(Joystick *);


//void initADC();
//void startADC();
//void initJoyStick();
void getSampleJoyStick(unsigned *X, unsigned *Y);
void getSampleAccel(unsigned *X, unsigned *Y, unsigned *Z);



#endif /* ADC_HAL_H_ */
