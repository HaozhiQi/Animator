#ifndef _MODELER_GLOBALS_H
#define _MODELER_GLOBALS_H

#ifndef M_PI
#define M_PI 3.141592653589793238462643383279502
#endif

#define TOTAL_FRAME 200
enum ModelNames
{
	MENGMEI,
	UPPERBODY,
	THORAX,
	LEFTUPPERARM,
	LEFTELBOW,
	LEFTLOWERARM,
	LEFTHAND,
	RIGHTUPPERARM,
	RIGHTELBOW,
	RIGHTLOWERARM,
	RIGHTHAND,
	HEAD,
	LEFTEYE,
	RIGHTEYE,
	LEFTLOWERBODY,
	LEFTUPPERLEG,
	LEFTKNEE,
	LEFTLOWERLEG,
	LEFTFOOT,
	LEFTTHRUSTER,
	RIGHTLOWERBODY,
	RIGHTUPPERLEG,
	RIGHTKNEE,
	RIGHTLOWERLEG,
	RIGHTFOOT,
	RIGHTTHRUSTER,
	HEADCONSTRAINT,
	LHANDCONSTRAINT,
	RHANDCONSTRAINT,
	LFOOTCONSTRAINT,
	RFOOTCONSTRAINT,
	NUMMODELS
};

// This is a list of the controls for the SampleModel
// We'll use these constants to access the values 
// of the controls from the user interface.
enum ModelControls
{ 
	// First Lighting control, position and diffusion color
	LIGHT0_X,
	LIGHT0_Y,
	LIGHT0_Z,
	LIGHT0_R,
	LIGHT0_G,
	LIGHT0_B,
	// Second Lighting control, position and diffusion color
	LIGHT1_X,
	LIGHT1_Y,
	LIGHT1_Z,
	LIGHT1_R,
	LIGHT1_G,
	LIGHT1_B,
	// Modeler position control
	XPOS, 
	YPOS, 
	ZPOS, 
	HEIGHT, 
	ROTATE,
	// Subdivision control part
	DIVISION_LEVEL,
	DIVISION_CONTROL1,
	DIVISION_CONTROL2,
	DIVISION_CONTROL3,
	DIVISION_CONTROL4,
	// Particle Source Control
	PS_EMIT_1,
	PS_NUM_1,
	PS_ANGLE_1,
	PS_EMIT_2,
	PS_NUM_2,
	PS_ANGLE_2,
	PS_EMIT_3,
	PS_NUM_3,
	PS_ANGLE_3,
	PS_LIFE,
	PS_GRAVITY,
	//Inverse Kinematics Constraints
	HEAD_CSTRN_X,
	HEAD_CSTRN_Y,
	HEAD_CSTRN_Z,
	LHAND_CSTRN_X,
	LHAND_CSTRN_Y,
	LHAND_CSTRN_Z,
	RHAND_CSTRN_X,
	RHAND_CSTRN_Y,
	RHAND_CSTRN_Z,
	LFOOT_CSTRN_X,
	LFOOT_CSTRN_Y,
	LFOOT_CSTRN_Z,
	RFOOT_CSTRN_X,
	RFOOT_CSTRN_Y,
	RFOOT_CSTRN_Z,
	//level of detail. This is the start of model parameters
	LEVEL_OF_DETAIL,
	//upperleg control
	LEFT_ULEG_ROTATE,
	LEFT_ULEG_SWAYLR,
	LEFT_ULEG_SWAYFB,
	RIGHT_ULEG_ROTATE,
	RIGHT_ULEG_SWAYLR,
	RIGHT_ULEG_SWAYFB,
	//knee angle control
	LEFT_KNEE,
	RIGHT_KNEE,
	//feet angle control
	//LEFT_FOOT_UD,
	LEFT_FOOT_LR,
	//RIGHT_FOOT_UD,
	RIGHT_FOOT_LR,
	//elbow angle control
	LEFT_ELBOW,
	RIGHT_ELBOW,
	//upperarm rotation
	LEFT_UARM_ROTATE,
	RIGHT_UARM_ROTATE,
	//Make sure this stays at the last
	NUMCONTROLS
};

// Colors
#define COLOR_RED		1.0f, 0.0f, 0.0f
#define COLOR_GREEN		0.0f, 1.0f, 0.0f
#define COLOR_BLUE		0.0f, 0.0f, 1.0f

// We'll be getting the instance of the application a lot; 
// might as well have it as a macro.
#define VAL(x) (ModelerApplication::Instance()->GetControlValue(x))
//CUSTOM: sets the control value
#define SET(controlNum, value) (ModelerApplication::Instance()->SetControlValue(controlNum, value))
#define STEPUP(controlNum) (ModelerApplication::Instance()->incrementControlValue(controlNum,1))
#define STEPDOWN(controlNum) (ModelerApplication::Instance()->incrementControlValue(controlNum,-1))
#define RANDOMIZE(controlNum, rangePercent, shiftPercent) (ModelerApplication::Instance()->randomizeControlValue(controlNum,VAL(controlNum), rangePercent, shiftPercent)) //the percentile interval should not exceed 50%
#define RANGE(controlNum) (ModelerApplication::Instance()->getControlMaximum(controlNum) - ModelerApplication::Instance()->getControlMinimum(controlNum))
#endif