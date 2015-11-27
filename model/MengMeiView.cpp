#include "MengMeiView.h"
#include "../IK/InverseKinematics.h"
//for debug use
#include <ctime>

void MengMeiView::draw()
{
	ModelerView::draw();
	//printf("MengMeiView::draw() %d\n", time(0));

	//if (VAL(LSYS_SHOW) > 0) {
	//	//draw the lsystem
	//	LSystem* lsys;
	//	switch ((int)VAL(LSYS_TYPE)) {
	//	case 0:
	//		lsys = &m_treeLSystem;
	//		break;
	//	case 1:
	//		lsys = &m_plantLSystem;
	//		break;
	//	}
	//	//calculate the lsys tree with given parameters
	//	lsys->evolve(VAL(LSYS_DEPTH));
	//	//draw the lsys tree following the calculated string
	//	lsys->draw(VAL(LSYS_DEPTH));
	//}
	//else {
#ifdef _DEBUG
		//printf("MengMeiView::draw() %d\n", time(0));
		//Draw the axis
		setAmbientColor(0.1f, 0.1f, 0.1f);
		glPushMatrix();
		setAmbientColor(.1f, .1f, .1f);
		setDiffuseColor(COLOR_RED);
		drawBox(100, .1f, .1f);
		glPopMatrix();

		glPushMatrix();
		setAmbientColor(.1f, .1f, .1f);
		setDiffuseColor(COLOR_GREEN);
		drawBox(.1f, 100, .1f);
		glPopMatrix();

		glPushMatrix();
		setAmbientColor(.1f, .1f, .1f);
		setDiffuseColor(COLOR_BLUE);
		drawBox(.1f, .1f, 100);
		glPopMatrix();

		setAmbientColor(.1f, .1f, .1f);
		setDiffuseColor(1.0f, 1.0f, 1.0f);
#endif
		/*ModelNames modelName = LEFTFOOT;
		Vec3f origin = Model::m_modelList[LEFTFOOT]->getOrigin();
		printf("%d:\n\tx: %f\n\ty: %f\n\tz: %f\n", modelName, origin[0], origin[1], origin[2]);
		*/
		//DO INVERSE KINEMATICS

		if (ModelerApplication::Instance()->IK()) {
			//get the optimized parameters
			IK::optimize();
		}

#ifdef _DEBUG
		
		Vec3d origin = Model::m_modelList[HEAD]->getOrigin();
		printf("HEAD %f %f %f\n", origin[0], origin[1], origin[2]);

		origin = Model::m_modelList[LEFTHAND]->getOrigin();
		printf("LEFTHAND %f %f %f\n", origin[0], origin[1], origin[2]);

		origin = Model::m_modelList[RIGHTHAND]->getOrigin();
		printf("RIGHTHAND %f %f %f\n", origin[0], origin[1], origin[2]);

		origin = Model::m_modelList[LEFTFOOT]->getOrigin();
		printf("LEFTFOOT %f %f %f\n", origin[0], origin[1], origin[2]);

		origin = Model::m_modelList[RIGHTFOOT]->getOrigin();
		printf("RIGHTFOOT %f %f %f\n", origin[0], origin[1], origin[2]);
#endif
		m_MengMei.Draw(VAL(LEVEL_OF_DETAIL));
	//} //LSYS
}

void MengMeiView::onLighting()
{
	GLfloat pos0[] = { VAL(LIGHT0_X), VAL(LIGHT0_Y), VAL(LIGHT0_Z), 0.0f };
	GLfloat diffuse0[] = { VAL(LIGHT0_R), VAL(LIGHT0_G), VAL(LIGHT0_B) };
	GLfloat pos1[] = { VAL(LIGHT1_X), VAL(LIGHT1_Y), VAL(LIGHT1_Z), 0.0f };
	GLfloat diffuse1[] = { VAL(LIGHT1_R), VAL(LIGHT1_G), VAL(LIGHT1_B) };

	//glLightfv(GL_LIGHT0, GL_POSITION, pos0);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);
	glLightfv(GL_LIGHT1, GL_POSITION, pos1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1);
}