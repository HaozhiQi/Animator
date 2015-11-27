// The sample model.  You should build a file
// very similar to this for when you make your model.
#include "modelerview.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include "particleSystem.h"
#include <FL/gl.h>

#include "modelerglobals.h"

#include "model/MengMeiView.h"

// We need to make a creator function, mostly because of
// nasty API stuff that we'd rather stay away from.
ModelerView* createSampleModel(int x, int y, int w, int h, char *label)
{ 
    return new MengMeiView(x,y,w,h,label); 
}

int main()
{
	// Initialize the controls
	// Constructor is ModelerControl(name, minimumvalue, maximumvalue, 
	// stepsize, defaultvalue)
    ModelerControl controls[NUMCONTROLS];
	controls[LIGHT0_X] = ModelerControl("Light0 X", -5, 5, 0.1, -5.0f);
	controls[LIGHT0_Y] = ModelerControl("Light0 Y", -5, 5, 0.1, -2.0);
	controls[LIGHT0_Z] = ModelerControl("Light0 Z", -5, 5, 0.1, -4.0);
	controls[LIGHT0_R] = ModelerControl("Light0 R", 0.0f, 1.0f, 0.1f, 1.0f);
	controls[LIGHT0_G] = ModelerControl("Light0 G", 0.0f, 1.0f, 0.1f, 1.0f);
	controls[LIGHT0_B] = ModelerControl("Light0 B", 0.0f, 1.0f, 0.1f, 1.0f);

	controls[LIGHT1_X] = ModelerControl("Light1 X", -5.0f, 5.0f, 0.1f, 4.0f);
	controls[LIGHT1_Y] = ModelerControl("Light1 Y", -5.0f, 5.0f, 0.1f, 1.0f);
	controls[LIGHT1_Z] = ModelerControl("Light1 Z", -5.0f, 5.0f, 0.1f, 5.0f);
	controls[LIGHT1_R] = ModelerControl("Light1 R", 0.0f, 1.0f, 0.1f, 1.0f);
	controls[LIGHT1_G] = ModelerControl("Light1 G", 0.0f, 1.0f, 0.1f, 1.0f);
	controls[LIGHT1_B] = ModelerControl("Light1 B", 0.0f, 1.0f, 0.1f, 1.0f);
    
	controls[XPOS] = ModelerControl("X Position", -10, 10, 0.1f, 0);
    controls[YPOS] = ModelerControl("Y Position", -10, 10, 0.1f, 0);
    controls[ZPOS] = ModelerControl("Z Position", -10, 10, 0.1f, 0);
    controls[HEIGHT] = ModelerControl("Height", 1, 2.5, 0.1f, 1);
	controls[ROTATE] = ModelerControl("Rotate", -135, 135, 1, 0);

	controls[HEAD_CSTRN_X] = ModelerControl("Head Constraint X", -20, 20, 0.1, 0);
	controls[HEAD_CSTRN_Y] = ModelerControl("Head Constraint Y", -20, 20, 0.1, 6.0);
	controls[HEAD_CSTRN_Z] = ModelerControl("Head Constraint Z", -20, 20, 0.1, 0);
	controls[LHAND_CSTRN_X] = ModelerControl("Left Hand Constraint X", -20, 20, 0.1, 5.356248);
	controls[LHAND_CSTRN_Y] = ModelerControl("Left Hand Constraint Y", -20, 20, 0.1, 3.550485);
	controls[LHAND_CSTRN_Z] = ModelerControl("Left Hand Constraint Z", -20, 20, 0.1, 0);
	controls[RHAND_CSTRN_X] = ModelerControl("Right Hand Constraint X", -20, 20, 0.1, -5.356248);
	controls[RHAND_CSTRN_Y] = ModelerControl("Right Hand Constraint Y", -20, 20, 0.1, 3.550485);
	controls[RHAND_CSTRN_Z] = ModelerControl("Right Hand Constraint Z", -20, 20, 0.1, 0);
	controls[LFOOT_CSTRN_X] = ModelerControl("Left Foot Constraint X", -20, 20, 0.1, 0.942230);
	controls[LFOOT_CSTRN_Y] = ModelerControl("Left Foot Constraint Y", -20, 20, 0.1, -6.781559);
	controls[LFOOT_CSTRN_Z] = ModelerControl("Left Foot Constraint Z", -20, 20, 0.1, -0.086824);
	controls[RFOOT_CSTRN_X] = ModelerControl("Right Foot Constraint X", -20, 20, 0.1, -0.948529);
	controls[RFOOT_CSTRN_Y] = ModelerControl("Right Foot Constraint Y", -20, 20, 0.1, -6.601208);
	controls[RFOOT_CSTRN_Z] = ModelerControl("Right Foot Constraint Z", -20, 20, 0.1, -0.760312);

	controls[LEVEL_OF_DETAIL] = ModelerControl("Level of Detail", 1, 10, 1, 10);

	controls[LEFT_ULEG_ROTATE] = ModelerControl("Left Upper Leg Rotate", 0, 80, 1, 0);
	controls[LEFT_ULEG_SWAYLR] = ModelerControl("Left Upper Leg Sway Left/Right", 0, 90, 1, 0);
	controls[LEFT_ULEG_SWAYFB] = ModelerControl("Left Upper Leg Sway Front/Back", -60, 150, 1, 10);
	controls[RIGHT_ULEG_ROTATE] = ModelerControl("Right Upper Leg Rotate", 0, 80, 1, 0);
	controls[RIGHT_ULEG_SWAYLR] = ModelerControl("Right Upper Leg Sway Left/Right", 0, 90, 1, 0);
	controls[RIGHT_ULEG_SWAYFB] = ModelerControl("Right Upper Leg Sway Front/Back", -60, 150, 1, 10);

	controls[LEFT_KNEE] = ModelerControl("Left Knee", 0, 160, 1, 20);
	controls[RIGHT_KNEE] = ModelerControl("Right Knee", 0, 160, 1, 20);

	//controls[LEFT_FOOT_UD] = ModelerControl("Left Foot UD", -30, 90, 1, 0);
	controls[LEFT_FOOT_LR] = ModelerControl("Left Foot LR", -30, 30, 1, 0);
	//controls[RIGHT_FOOT_UD] = ModelerControl("Right Foot UD", -30, 90, 1, 0);
	controls[RIGHT_FOOT_LR] = ModelerControl("Right Foot LR", -30, 30, 1, 0);

	controls[LEFT_ELBOW] = ModelerControl("Left Elbow", 0, 160, 1, 0);
	controls[RIGHT_ELBOW] = ModelerControl("Right Elbow", 0, 160, 1, 0);

	controls[LEFT_UARM_ROTATE] = ModelerControl("Left Uarm Rotate", -90, 90, 1, 0);
	controls[RIGHT_UARM_ROTATE] = ModelerControl("Right Uarm Rotate", -90, 90, 1, 0);

	controls[DIVISION_LEVEL] = ModelerControl("Division Level", 0, 10, 1, 0);
	controls[DIVISION_CONTROL1] = ModelerControl("Division Control1", 0, 2, 0.2, 1);
	controls[DIVISION_CONTROL2] = ModelerControl("Division Control2", 0, 2, 0.2, 1);
	controls[DIVISION_CONTROL3] = ModelerControl("Division Control3", 0, 2, 0.2, 1);
	controls[DIVISION_CONTROL4] = ModelerControl("Division Control4", 0, 2, 0.2, 1);

	//particle source controls

	controls[PS_EMIT_1] = ModelerControl("PS_EMIT_1", 0, 1, 0.1, 1);
	controls[PS_NUM_1] = ModelerControl("PS_NUM_1", 0, 5, 1, 1);
	controls[PS_ANGLE_1] = ModelerControl("PS_ANGLE_1", 0, 180, 1, 10);
	
	controls[PS_EMIT_2] = ModelerControl("PS_EMIT_2", 0, 1, 0.1, 1);
	controls[PS_NUM_2] = ModelerControl("PS_NUM_2", 0, 5, 1, 1);
	controls[PS_ANGLE_2] = ModelerControl("PS_ANGLE_2", 0, 180, 1, 10);
	
	controls[PS_EMIT_3] = ModelerControl("PS_EMIT_3", 0, 1, 0.1, 1);
	controls[PS_NUM_3] = ModelerControl("PS_NUM_3", 0, 5, 1, 1);
	controls[PS_ANGLE_3] = ModelerControl("PS_ANGLE_3", 0, 180, 1, 10);

	controls[PS_LIFE] = ModelerControl("PS_LIFE", 0, 10.0, 0.1, 3.0);

	controls[PS_GRAVITY] = ModelerControl("PS_GRAVITY", 0, 20, 1, 10);


    ModelerApplication::Instance()->Init(&createSampleModel, controls, NUMCONTROLS);
	ModelerApplication::Instance()->SetParticleSystem(&ParticleSystem::Instance());
    return ModelerApplication::Instance()->Run();
}
