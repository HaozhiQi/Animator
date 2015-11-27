
#include "MengMei.h"
//#include "MengMeiHand.h"
#include "MengMeiHead.h"
#include "MengMeiLowerLeg.h"
//#include "MengMeiMetaBall.h"
//#include "../MetaBall.h"
#include "MengMeiFoot.h"
#include "indicatorModel.h"
#include "ParticleSource.h"
#include "../modelerapp.h"
#include "../particleSystem.h"
#include "../ObjDivider/ObjFile.h"

MengMei::MengMei() :Model(MENGMEI)
{
	GeneralModel* upperBody = new GeneralModel(UPPERBODY, GeneralModel::CYLINDER_SHAPE);
	upperBody->setTexture("./res/MengMeiBody.jpg");
	upperBody->setParam(3.0f, 0.5f, 0.75f);
	upperBody->getController()->setRotate(-90, 90, 0);
		
		GeneralModel* thorax = new GeneralModel(THORAX,GeneralModel::SPHERE_SHAPE);
		thorax->getController()->setTransZ(3.5f);
		thorax->setParam(0.75f);
		
			//--------------------------------
			//leftUpperArm
			//--------------------------------
			GeneralModel* leftUpperArm = new GeneralModel(LEFTUPPERARM, GeneralModel::CYLINDER_SHAPE);
			leftUpperArm->setParam(2.7f, 0.6f, 0.5f);
			leftUpperArm->getController()->setRotate(70, 180, 0);
			
				GeneralModel* leftElbow = new GeneralModel(LEFTELBOW, GeneralModel::SPHERE_SHAPE);
				leftElbow->setParam(0.6f);
				leftElbow->getController()->setTransZ(2.7f);
				leftElbow->m_beforeDraw = [](Model* self) {
					self->getController()->setRotateZ(VAL(LEFT_UARM_ROTATE));
				};

					GeneralModel* leftLowerArm = new GeneralModel(LEFTLOWERARM, GeneralModel::CYLINDER_SHAPE);
					leftLowerArm->setTexture(std::string("./res/MengMeiSkin.jpg"));
					leftLowerArm->setParam(3.0f, 0.5f, 0.4f);
					leftLowerArm->m_beforeDraw = [](Model* self) {
						self->getController()->setRotateY(VAL(LEFT_ELBOW));
					};
						
#if 0 //remove metaball for speed
						Model* leftHand = new MengMeiHand(LEFTHAND);
						leftHand->getController()->setTransZ(3.5f)->setRotateY(-90);
#else
						GeneralModel* leftHand = new GeneralModel(LEFTHAND, GeneralModel::SPHERE_SHAPE);
						leftHand->setParam(0.5f);
						leftHand->getController()->setTransZ(3.0f);
#endif
						/*	Model* metaball = new MengMeiMetaBall();
						leftHand->addChild(metaball);*/
					leftLowerArm->addChild(leftHand);
				leftElbow->addChild(leftLowerArm);
			leftUpperArm->addChild(leftElbow);

			//--------------------------------
			//rightUpperArm
			//--------------------------------
			GeneralModel* rightUpperArm = new GeneralModel(RIGHTUPPERARM, GeneralModel::CYLINDER_SHAPE);
			rightUpperArm->setParam(2.7f, 0.6f, 0.5f);
			rightUpperArm->getController()->setRotate(-70, 180, 0);

				GeneralModel* rightElbow = new GeneralModel(RIGHTELBOW, GeneralModel::SPHERE_SHAPE);
				rightElbow->setParam(0.6f);
				rightElbow->getController()->setTransZ(2.7f);
				rightElbow->m_beforeDraw = [](Model* self) {
					self->getController()->setRotateZ(VAL(RIGHT_UARM_ROTATE));
				};

					GeneralModel* rightLowerArm = new GeneralModel(RIGHTLOWERARM, GeneralModel::CYLINDER_SHAPE);
					rightLowerArm->setTexture(std::string("./res/MengMeiSkin.jpg"));
					rightLowerArm->setParam(3.0f, 0.5f, 0.4f);
					rightLowerArm->m_beforeDraw = [](Model* self) {
						self->getController()->setRotateY(VAL(RIGHT_ELBOW));
					};

						GeneralModel* rightHand = new GeneralModel(RIGHTHAND, GeneralModel::SPHERE_SHAPE);
						rightHand->setParam(0.5f);
						rightHand->getController()->setTransZ(3.0f);

					rightLowerArm->addChild(rightHand);
				rightElbow->addChild(rightLowerArm);
			rightUpperArm->addChild(rightElbow);

			//---------------------------------
			//Head
			//---------------------------------
			Model* head = new MengMeiHead(HEAD);
			head->getController()->setRotate(90, 0, -90)->setTransZ(0.5f);

		thorax->addChild(leftUpperArm);
		thorax->addChild(rightUpperArm);
		thorax->addChild(head);
	upperBody->addChild(thorax);
	
	// ------------------------------------------
	// Left Leg
	// ------------------------------------------
	
	GeneralModel* leftLowerBody = new GeneralModel(LEFTLOWERBODY, GeneralModel::SPHERE_SHAPE);
	leftLowerBody->setParam(0.7f);
	leftLowerBody->getController()->setTransX(1.2f)->setTransY(-1.4f)->setRotate(60,90,0);
	
		GeneralModel* leftUpperLeg = new GeneralModel(LEFTUPPERLEG, GeneralModel::CYLINDER_SHAPE);
		leftUpperLeg->setParam(3.5f, 0.6f, 0.5f);
		leftUpperLeg->m_beforeDraw = [](Model* self) {
			self->getController()->setRotateZ(90 - VAL(LEFT_ULEG_ROTATE));
			self->getController()->setRotateY(VAL(LEFT_ULEG_SWAYLR) - 32);
			self->getController()->setRotateX(-VAL(LEFT_ULEG_SWAYFB));
		};
		
			GeneralModel* leftKnee = new GeneralModel(LEFTKNEE, GeneralModel::SPHERE_SHAPE);
			leftKnee->setParam(0.5f);
			leftKnee->getController()->setTransZ(3.5f)->setRotateZ(-90);
			
				GeneralModel* leftLowerLeg = new GeneralModel(LEFTLOWERLEG, GeneralModel::CYLINDER_SHAPE);
				leftLowerLeg->setParam(4.0f, 0.5f, 0.4f);
				leftLowerLeg->m_beforeDraw = [](Model* self) {
					self->getController()->setRotateY(VAL(LEFT_KNEE));
				};

					GeneralModel* leftFoot = new GeneralModel(LEFTFOOT, GeneralModel::SPHERE_SHAPE);
					leftFoot->setParam(0.4f);
					leftFoot->getController()->setTransZ(4.0f);
					leftFoot->m_beforeDraw = [](Model* self) {
						//self->getController()->setRotateY(VAL(LEFT_FOOT_UD));
						self->getController()->setRotateZ(VAL(LEFT_FOOT_LR));
					};

						ParticleSource* leftThruster = new ParticleSource(LEFTTHRUSTER);
						leftFoot->addChild(leftThruster);
						ParticleSystem::Instance().addParticleSource(leftThruster);
				leftLowerLeg->addChild(leftFoot);

			leftKnee->addChild(leftLowerLeg);
			
		leftUpperLeg->addChild(leftKnee);
		
	leftLowerBody->addChild(leftUpperLeg);
	
	// ------------------------------------------
	// Right Leg
	// ------------------------------------------

	GeneralModel* rightLowerBody = new GeneralModel(RIGHTLOWERBODY, GeneralModel::SPHERE_SHAPE);
	rightLowerBody->setParam(0.7f);
	rightLowerBody->getController()->setTransX(-1.2f)->setTransY(-1.4f)->setRotate(120, 90, 0);
	
		GeneralModel* rightUpperLeg = new GeneralModel(RIGHTUPPERARM, GeneralModel::CYLINDER_SHAPE);
		rightUpperLeg->setParam(3.5f, 0.6f, 0.5f);
		rightUpperLeg->m_beforeDraw = [](Model* self) {
			self->getController()->setRotateZ(90 + VAL(RIGHT_ULEG_ROTATE));
			self->getController()->setRotateY(32 - VAL(RIGHT_ULEG_SWAYLR));
			self->getController()->setRotateX(-VAL(RIGHT_ULEG_SWAYFB));
		};
		
			GeneralModel* rightKnee = new GeneralModel(RIGHTKNEE, GeneralModel::SPHERE_SHAPE);
			rightKnee->setParam(0.5f);
			rightKnee->getController()->setTransZ(3.5f)->setRotateZ(-90);
			rightKnee->getController()->setRotateY(10);

				/*GeneralModel* rightLowerLeg = new GeneralModel("RightLowerLeg", GeneralModel::CYLINDER_SHAPE);
				rightLowerLeg->setParam(4.0f, 0.5f, 0.4f);*/
				Model* rightLowerLeg = new MengMeiLowerLeg(RIGHTLOWERLEG);
				rightLowerLeg->m_beforeDraw = [](Model* self) {
					self->getController()->setRotateY(VAL(RIGHT_KNEE));
				};

					/*GeneralModel* rightFoot = new GeneralModel("RightFoot", GeneralModel::SPHERE_SHAPE);
					rightFoot->setParam(0.4f);
					rightFoot->getController()->setTransZ(4.0f);*/
					Model* rightFoot = new MengMeiFoot(RIGHTFOOT);
					rightFoot->getController()->setTransZ(4.0f);
					rightFoot->m_beforeDraw = [](Model* self) {
						//self->getController()->setRotateY(VAL(RIGHT_FOOT_UD));
						self->getController()->setRotateZ(VAL(RIGHT_FOOT_LR));
					};

				rightLowerLeg->addChild(rightFoot);

			rightKnee->addChild(rightLowerLeg);

		rightUpperLeg->addChild(rightKnee);
		
	rightLowerBody->addChild(rightUpperLeg);

	addChild(upperBody);
	addChild(leftLowerBody);
	addChild(rightLowerBody);

	//-----------------------------------------
	//Constraint Indicators
	//THEY ARE NOT CHILDREN OF MAIN CHARACTER
	//-----------------------------------------
	IndicatorModel* headConstraint = new IndicatorModel(HEADCONSTRAINT);
	headConstraint->m_beforeDraw = [](Model* self) {
		self->getController()->setTrans(VAL(HEAD_CSTRN_X), VAL(HEAD_CSTRN_Y), VAL(HEAD_CSTRN_Z));
	};

	IndicatorModel* leftHandConstraint = new IndicatorModel(LHANDCONSTRAINT);
	leftHandConstraint->m_beforeDraw = [](Model* self) {
		self->getController()->setTrans(VAL(LHAND_CSTRN_X), VAL(LHAND_CSTRN_Y), VAL(LHAND_CSTRN_Z));
	};

	IndicatorModel* rightHandConstraint = new IndicatorModel(RHANDCONSTRAINT);
	rightHandConstraint->m_beforeDraw = [](Model* self) {
		self->getController()->setTrans(VAL(RHAND_CSTRN_X), VAL(RHAND_CSTRN_Y), VAL(RHAND_CSTRN_Z));
	};

	IndicatorModel* leftFootConstraint = new IndicatorModel(LFOOTCONSTRAINT);
	leftFootConstraint->m_beforeDraw = [](Model* self) {
		self->getController()->setTrans(VAL(LFOOT_CSTRN_X), VAL(LFOOT_CSTRN_Y), VAL(LFOOT_CSTRN_Z));
	};

	IndicatorModel* rightFootConstraint = new IndicatorModel(RFOOTCONSTRAINT);
	rightFootConstraint->m_beforeDraw = [](Model* self) {
		self->getController()->setTrans(VAL(RFOOT_CSTRN_X), VAL(RFOOT_CSTRN_Y), VAL(RFOOT_CSTRN_Z));
	};

	//set the position of MengMei
	this->m_beforeDraw = [](Model* self){
		self->getController()->setTrans(VAL(XPOS), VAL(YPOS), VAL(ZPOS));
	};
}

void MengMei::onDraw()
{
	glPushMatrix();
	drawSphere(0.8f);
	glPopMatrix();

	createSubDivision();

	//delete mb;
	glPushMatrix();
	glTranslatef(0.5f, -0.25f, 0.0f);
	glRotatef(90, 0.0f, 1.0f, 0.0f);
	glRotatef(60, 1.0f, 0.0f, 0.0f);
	drawCylinder(1.0f, 0.5f, 0.5f);
#ifdef _DEBUG
	//draw the reference coordinate system
	setAmbientColor(0.1f, 0.1f, 0.1f);
	glPushMatrix();
	setAmbientColor(.1f, .1f, .1f);
	setDiffuseColor(COLOR_RED);
	drawBox(10, .01f, .01f);
	glPopMatrix();

	glPushMatrix();
	setAmbientColor(.1f, .1f, .1f);
	setDiffuseColor(COLOR_GREEN);
	drawBox(.01f, 10, .01f);
	glPopMatrix();

	glPushMatrix();
	setAmbientColor(.1f, .1f, .1f);
	setDiffuseColor(COLOR_BLUE);
	drawBox(.01f, .01f, 10);
	glPopMatrix();

	setDiffuseColor(1.0f, 1.0f, 1.0f);
#endif
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5f, -0.25f, 0.0f);
	glRotatef(90, 0.0f, 1.0f, 0.0f);
	glRotatef(-60, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, -1.0f);
	drawCylinder(1.0f, 0.5f, 0.5f);
#ifdef _DEBUG
	//draw the reference coordinate system
	setAmbientColor(0.1f, 0.1f, 0.1f);
	glPushMatrix();
	setAmbientColor(.1f, .1f, .1f);
	setDiffuseColor(COLOR_RED);
	drawBox(10, .01f, .01f);
	glPopMatrix();

	glPushMatrix();
	setAmbientColor(.1f, .1f, .1f);
	setDiffuseColor(COLOR_GREEN);
	drawBox(.01f, 10, .01f);
	glPopMatrix();

	glPushMatrix();
	setAmbientColor(.1f, .1f, .1f);
	setDiffuseColor(COLOR_BLUE);
	drawBox(.01f, .01f, 10);
	glPopMatrix();

	setDiffuseColor(1.0f, 1.0f, 1.0f);
#endif
	glPopMatrix();
}

void MengMei::createSubDivision()
{
	
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHT1);
	glDisable(GL_LIGHTING);

	glPushMatrix();

	glTranslatef(4.5f, 5.0f, 0.0f);
	glScalef(0.8f, 0.8f, 0.8f);

	int divisionLevel = VAL(DIVISION_LEVEL);

	ObjFile* objFile = new ObjFile("objModel/star.ply");
	objFile->defineUserControl(VAL(DIVISION_CONTROL1), VAL(DIVISION_CONTROL2),
		VAL(DIVISION_CONTROL3), VAL(DIVISION_CONTROL4));
	if (divisionLevel == 0) {
		objFile->createModel(true);
	}
	else {
		objFile->findAdjTriangles();
		for (int i = 0; i < divisionLevel; ++i)
		{
			objFile->subdivide();
		}
		objFile->createModel(false);
	}
	printf("Vertices %d\n", objFile->vertices_.size());
	delete objFile;

	glPopMatrix();

	// ---------------------------------------------------------------------------

	glPushMatrix();
	// Second one
	glTranslatef(-4.5f, 5.0f, 0.0f);
	glScalef(0.8f, 0.8f, 0.8f);

	ObjFile* objFile2 = new ObjFile("objModel/star.ply");
	objFile2->defineUserControl(VAL(DIVISION_CONTROL1), VAL(DIVISION_CONTROL2),
		VAL(DIVISION_CONTROL3), VAL(DIVISION_CONTROL4));
	if (divisionLevel == 0) {
		objFile2->createModel(true);
	}
	else {
		objFile2->findAdjTriangles();
		for (int i = 0; i < divisionLevel; ++i)
		{
			objFile2->subdivide();
		}
		objFile2->createModel(false);
	}
	printf("Vertices %d\n", objFile2->vertices_.size());
	delete objFile2;


	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glPopMatrix();
}