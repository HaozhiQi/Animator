
#include "../modelerview.h"
#include "../modelerapp.h"
#include "../modelerdraw.h"
#include "model.h"
#include <Fl/gl.h>

map<ModelNames, Model* > Model::m_modelList;

Model::Model(ModelNames name, Model* parent) :m_parent(parent), m_beforeDraw(NULL)
{
	anim_cnt = 0;
	//add this model to the static modelList
	m_modelList[name] = this;
}

void Model::addChild(Model* p_model)
{
	p_model->setParent(this);
	m_children.push_back(p_model);
}

Model* Model::getChild(const int pos)
{
	return m_children[pos];
}

void Model::Draw(int levelOfDetail)
{
	--levelOfDetail;
	glPushMatrix();
	//call the beforeDraw callback to update transformations
	if (m_beforeDraw) m_beforeDraw(this);
	//Calls Control to apply the transformations
	m_controller.Control();
	onDraw();

	if (levelOfDetail) 
		for (Model* m : m_children)
		{
			m->Draw(levelOfDetail);
		}

	glPopMatrix();
}

bool Model::isAnimating()
{
	return ModelerApplication::Instance()->m_animating;
}

void Model::refreshParameters()
{
	//make sure it comes from above
	if (getParent()) getParent()->refreshParameters();
	//call before draw to update parameters
	if (m_beforeDraw) m_beforeDraw(this);
}

void Model::drawTexture(std::string& fileName, GLuint& handle)
{
	int height, width, numComponent;
	unsigned char* textureData = stbi_load(fileName.c_str(), &width, &height, &numComponent, 4);
	if (textureData == NULL)
	{
		std::cerr << "loading texture failed" << std::endl;
	}

	glGenTextures(1, &handle);
	glBindTexture(GL_TEXTURE_2D, handle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, textureData);
	glFlush();

	stbi_image_free(textureData);
}

void Model::drawTorus(GLdouble r1, GLdouble r2)
{
	ModelerDrawState *mds = ModelerDrawState::Instance();
	int divisions;

	_setupOpenGl();

	switch (mds->m_quality)
	{
	case HIGH:
		divisions = 32; break;
	case MEDIUM:
		divisions = 20; break;
	case LOW:
		divisions = 12; break;
	case POOR:
		divisions = 8; break;
	}

	if (mds->m_rayFile)
	{
		_dump_current_modelview();
		fprintf(mds->m_rayFile,
			"cone { bottom_radius=%f; top_radius=%f;\n", r1, r2);
		_dump_current_material();
		fprintf(mds->m_rayFile, "})\n");
	}
	else
	{
		divisions *= 6;

		for (int i = 0; i < divisions; i++){
			GLdouble curAngle = 3.1415926 * 2.0 / divisions * i;
			GLdouble curOx, curOz;
			curOx = r1 * cos(curAngle);
			curOz = r1 * sin(curAngle);

			glPushMatrix();
			glTranslated(curOx, 0.0, curOz);
			glRotated(-curAngle / 3.1415926*180.0, 0.0, 1.0, 0.0);
			drawCylinder(3.1415926 * 2.0 / divisions * (r1 + r2)*1.02, r2, r2);
			glPopMatrix();
		}

	}
}

Vec3d Model::getOrigin() {
	Vec4d origin = Vec4d(0, 0, 0, 1);
	Model* current = this;
	origin = getController()->getMatrix() * origin;
	while (current = current->getParent()) {
		origin = current->getController()->getMatrix() * origin;
	}
	//convert homogeneous coordinate to 3D spatial coordinate
	return Vec3d(origin[0] / origin[3], origin[1] / origin[3], origin[2] / origin[3]);
}

Mat4d Model::getTransMatrix() {
	Mat4d mtx = getController()->getMatrix();
	Model* current = this;
	while (current = current->getParent()) {
		mtx = current->getController()->getMatrix() * mtx;
	}
	return mtx;
}

Model::~Model()
{/*
	for (Model* m : m_children)
	{
		delete m;
	}*/
}