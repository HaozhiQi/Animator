/*
* ObjFile.h
*
* Author: Qi Haozhi, Sun Yushi
* Copyright (c) 2015 Qi Haozhi, Sun Yushi
*
* You can star the project via:
* https://github.com/Oh233/Animator OR
* https://github.com/JeffOwOSun/Animator
*/

#ifndef OBJ_FILE_H
#define OBJ_FILE_H

#include <vector>
#include <FL/gl.h>
#include "../vec.h"
#include "../mat.h"
#include "ObjUtil.h"

class ObjFile
{
public:

	ObjFile(std::string file);
	GLuint createModel(bool flat = false, bool cartoon = false);
	void findAdjTriangles();
	void subdivide();
	void computeNormals();
	void defineUserControl(double a, double b, double c, double d);

	void triangleSplit(int index, int callee, int newPoint, int t1, int t2);
	void loadNormalIndices();
	void computeNewVerts();

	std::vector<Vec3f>		normals_;
	std::vector<Vec3f>		vertices_;
	std::vector<Triangle>		triangles_;
	std::vector<bool>			processed_;

	std::vector<Vec3f>		newverts_;

	std::vector<int> midPoints_;
	std::vector<int> midTriangles_;
	std::vector<Triangle> tmpTriangles_;
	std::vector<SplitData> splits_;

	GLuint listName_;
};

#endif