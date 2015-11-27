// DO NOT mess with this file.  If you do, animator will not work with
// your model, and you'll have to write a new one.  If you really really
// really need to do something here (unlikely) then don't complain if and
// when animator doesn't work.  -- Eugene
#ifndef MODELERAPP_H
#define MODELERAPP_H

#include "modelerview.h"

struct ModelerControl
{
	ModelerControl();
	ModelerControl(const char* name, float minimum, float maximum, float stepsize, float value);
	ModelerControl(const ModelerControl &o);
	ModelerControl& operator=(const ModelerControl &o);
	void SetVals(const char* name, float minimum, float maximum, float stepsize, float value);

	char  m_name[128];
	float m_minimum;
	float m_maximum;
	float m_stepsize;
	float m_value;
};

// Forward declarations for ModelerApplication
class ModelerView;
class ModelerUI;
class Fl_Box;
class Fl_Slider;
class Fl_Value_Slider;
class ParticleSystem;

// The ModelerApplication is implemented as a "singleton" design pattern,
// the purpose of which is to only allow one instance of it.
class ModelerApplication
{
public:
	~ModelerApplication();

	// Fetch the global ModelerApplication instance
	static ModelerApplication* Instance();

    // Initialize the application; see sample models for usage
	void Init(ModelerViewCreator_f createView, 
              const ModelerControl controls[], 
              unsigned numControls); 

    // Starts the application, returns when application is closed
	int  Run();

    // Get and set slider values.
    double GetControlValue(int controlNumber);
    void   SetControlValue(int controlNumber, double value);

	// Get and set particle system
	ParticleSystem *GetParticleSystem();
	void SetParticleSystem(ParticleSystem *s);

	// Return the current time
	float GetTime();

	// Return the current fps
	int GetFps();

	// Returns animating flag
	bool Animating();

	//rigid body
	bool rb();

	//IK
	bool IK();
	//CUSTOM: Get increment the slider
	void   incrementControlValue(int controlNumber, int times);
	//CUSTOM: Randomize the slider value, with given randomization Center and the percent
	//range of the random intervals
	void   randomizeControlValue(int controlNumber, double randomizeCenter, double rangePercentile, double shiftPercentile = 0.0);
	//CUSTOM: Get the maximum and minimum of the values of slider
	double getControlMaximum(int controlNumber);
	double getControlMinimum(int controlNumber);

private:
	// Private for singleton
	ModelerApplication() : m_numControls(-1) { ps = 0; }
	ModelerApplication(const ModelerApplication&) {}
	ModelerApplication& operator=(const ModelerApplication&) {}
	
	// The instance
	static ModelerApplication *m_instance;

	ModelerUI *m_ui;
	int					  m_numControls;

    static void ValueChangedCallback();
	static void RedrawLoop(void*);
public:
	// Just a flag for updates
	bool m_animating;
private:
	// Particle System variables
	ParticleSystem *ps;
};

#endif