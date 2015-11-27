#ifndef MENGMEI_H
#define MENGMEI_H

#include <FL/gl.h>
#include "../modelerdraw.h"
#include "../modelerapp.h"

#include "model.h"
#include "generalModel.h"

/**
 * \class This is the connection part between upper body and leg
 */
class MengMei : public Model
{

public:
	MengMei();
	void onDraw();
	void createSubDivision();
};

#endif