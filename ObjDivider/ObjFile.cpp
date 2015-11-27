/*
* ObjFile.cpp
*
* Author: Qi Haozhi, Sun Yushi
* Copyright (c) 2015 Qi Haozhi, Sun Yushi
*
* You can star the project via:
* https://github.com/Oh233/Animator OR
* https://github.com/JeffOwOSun/Animator
*/

#include "ObjFile.h"
#include "../modelerglobals.h"
#include "../modelerapp.h"
#include "../modelerdraw.h"
#include <fstream>
#include <iostream>
#include <string>
#include <assert.h>
#include <algorithm>
#include <functional>

ObjFile::ObjFile(string file)
{
	std::ifstream ifs(file.c_str());
	if (!ifs)
	{
		std::cout << "ObjFile::ObjFile: file " << file << " could not be read! \n";
		exit(1);
	}

	// clear cached data
	normals_.clear();
	vertices_.clear();
	triangles_.clear();

	int vertex_num, face_num;
	std::string temp;
	ifs >> temp >> vertex_num >> temp >> face_num;
	normals_.reserve(vertex_num);
	vertices_.reserve(vertex_num);
	triangles_.reserve(face_num);

	for (int i = 0; i < vertex_num; ++i)
	{
		float bar_x, bar_y, bar_z;
		ifs >> bar_x >> bar_y >> bar_z;
		vertices_.push_back(Vec3f(bar_x, bar_y, bar_z));
	}

	for (int i = 0; i < face_num; ++i)
	{
		int num_index; ifs >> num_index;
		if (num_index != 3)
		{
			std::cout << "Only triangles are supported, terminating now \n";
			exit(1);
		}
		// store information of obj file to triangle
		Triangle t;
		ifs >> t.v[0] >> t.v[1] >> t.v[2];
		for (int i = 0; i < 3; ++i) t.n[i] = t.v[i];
		triangles_.push_back(t);
	}
}

void ObjFile::defineUserControl(double a, double b, double c, double d)
{
	int size = vertices_.size();
	int partition = size / 4;
	vertices_[0] *= a;
	vertices_[partition] *= b;
	vertices_[partition * 2] *= c;
	vertices_[partition * 3] *= d;
}


GLuint ObjFile::createModel(bool flat)
{
	glBegin(GL_TRIANGLES);
	Vec3f lightdir = Vec3f(VAL(LIGHT0_X), VAL(LIGHT0_Y), VAL(LIGHT0_Z));

	
	for (vector<Triangle>::const_iterator it = triangles_.begin(); it != triangles_.end(); ++it)
	{
		for (int k = 0; k < 3; ++k)
		{
			if (flat)
			{
				Vec3f normal = (vertices_[it->v[1]] - vertices_[it->v[0]]) 
					^ (vertices_[it->v[2]] - vertices_[it->v[0]]);
				normal.normalize();
				glNormal3fv((float*)&normal);
			}
			else
			{
				normals_[k].normalize(); lightdir.normalize();
				float intensity = normals_[k] * lightdir;
				//cout << intensity << endl;
				if (intensity > 0.95)
					setDiffuseColor(1.0, 0.5, 0.5);
				else if (intensity > 0.5)
					setDiffuseColor(0.6, 0.3, 0.3);
				else if (intensity > 0.25)
					setDiffuseColor(0.4, 0.2, 0.2);
				else
					setDiffuseColor(0.2, 0.1, 0.1);

				glNormal3fv((float*) &normals_[it->n[k]]);
			}
			glVertex3fv((float*)&vertices_[it->v[k]]);
		}
	}

	glEnd();
	return 1;
}

void ObjFile::findAdjTriangles()
{
	// Initialize all index to be invalid
	for (std::vector<Triangle>::iterator it = triangles_.begin(); it != triangles_.end(); ++it)
	{
		for (int e = 0; e < 3; ++e) it->t[e] = -1;
	}

	for (std::vector<Triangle>::iterator it = triangles_.begin(); it != triangles_.end(); ++it)
	{
		for (int e = 0; e < 3; ++e)
		{
			int next_e = (e + 1) % 3;
			if (it->t[e] == -1)
			{
				for (std::vector<Triangle>::iterator it2 = it + 1; it2 != triangles_.end(); ++it2)
				{
					int index;
					if ((index = it2->containsEdge(it->v[next_e], it->v[e])) >= 0)
					{
						it->t[e] = it2 - triangles_.begin();
						it2->t[index] = it - triangles_.begin();
					}
				}
			}
		}
	}
}

void ObjFile::subdivide()
{
	// clear old data
	midPoints_ = std::vector<int>(triangles_.size() * 3, -1);
	midTriangles_ = std::vector<int>(triangles_.size() * 3, -1);
	tmpTriangles_.clear();
	processed_ = std::vector<bool>(triangles_.size(), false);

	int oldVertSize = vertices_.size();

	splits_.clear();
	splits_.push_back(SplitData(0, -1, -1, -1, -1));
	while(!splits_.empty())
	{
		SplitData sd = splits_.back();
		splits_.pop_back();
		triangleSplit(sd.index, sd.callee, sd.newPoint, sd.t1, sd.t2);
	}

	newverts_ = std::vector<Vec3f>(oldVertSize);
	computeNewVerts();
	std::copy(newverts_.begin(), newverts_.end(), vertices_.begin());

	triangles_.swap(tmpTriangles_);
	loadNormalIndices();
	computeNormals();
}

void ObjFile::triangleSplit(int index, int callee, int newPoint, int t1, int t2)
{
	if (processed_[index])
		return;

	int sz;
	int midPoints[3];
	int indices[3];
	bool process[3];

	const Triangle& cur = triangles_[index];
	Triangle tri[4];
	int off = tmpTriangles_.size();

	if (callee < 0) {
		sz = 3;
		indices[0] = 0; indices[1] = 1; indices[2] = 2;
	}
	else
	{
		sz = 2;
		int update = cur.findNeighborIndex(callee);

		if (update < 0)
		{
			std::cerr << "ObjFile::triangleSplit: semantic error !\n";
			exit(1);
		}

		midPoints[update] = newPoint;
		midPoints_[3 * index + update] = newPoint;

		tri[update].t[0] = t1;
		tri[(update + 1) % 3].t[2] = t2;

		tmpTriangles_[t1].t[2] = off + update;
		tmpTriangles_[t2].t[0] = off + (update + 1) % 3;

		if (update == 0) {
			indices[0] = 1; indices[1] = 2;
		}
		else if (update == 1) {
			indices[0] = 0; indices[1] = 2;
		}
		else {
			indices[0] = 0; indices[1] = 1;
		}
	}

	// process the other ones
	for (int i = 0; i < sz; ++i)
	{
		// check whether already processed ?
		// get the neighbour of current edge
		int ti = cur.t[indices[i]];
		Triangle& t = triangles_[ti];
		int tv = 3 * ti + t.findNeighborIndex(index);
		int tvnext = 3 * ti + (t.findNeighborIndex(index) + 1) % 3;

		if (midPoints_[tv] >= 0) {
			midPoints[indices[i]] = midPoints_[3 * index + indices[i]] = midPoints_[tv];
			process[i] = false;
			// update triangle connectivity

			assert(midTriangles_[tv] != -1);
			assert(midTriangles_[tvnext] != -1);

			tri[indices[i]].t[0] = midTriangles_[tvnext];
			tri[(indices[i] + 1) % 3].t[2] = midTriangles_[tv];

			tmpTriangles_[midTriangles_[tvnext]].t[2] = off + indices[i];
			tmpTriangles_[midTriangles_[tv]].t[0] = off + (indices[i] + 1) % 3;

		}
		else {
			//insert new one
			int k = indices[i];
			int knext = (k + 1) % 3;
			int kprev = (k + 2) % 3;
			int opposite = (t.findNeighborIndex(index) + 2) % 3;

			Vec3f p = (vertices_[cur.v[k]] + vertices_[cur.v[knext]]) * (3.0f / 8.0f) +
				(vertices_[cur.v[kprev]] + vertices_[t.v[opposite]]) * (1.0f / 8.0f);
			vertices_.push_back(p);
			midPoints[indices[i]] = midPoints_[3 * index + indices[i]] = vertices_.size() - 1;
			process[i] = true;
		}
	}

	// add subdivided triangles to new list
	//					2
	//					/\
	//			       /  \
	//				  /  2 \
	//               /      \
	//			    *--------*
	//             /  \  3 /  \
	//            /  0 \  /  1 \
	//           *------**------*
	//           0               1

	assert(midPoints[0] != -1);
	assert(midPoints[1] != -1);
	assert(midPoints[2] != -1);

	tri[0].v[0] = cur.v[0];
	tri[0].v[1] = midPoints[0];
	tri[0].v[2] = midPoints[2];
	tri[0].t[1] = off + 3;

	tri[3].v[0] = midPoints[2];
	tri[3].v[1] = midPoints[0];
	tri[3].v[2] = midPoints[1];
	tri[3].t[0] = off;
	tri[3].t[1] = off + 1;
	tri[3].t[2] = off + 2;

	tri[1].v[0] = cur.v[1];
	tri[1].v[1] = midPoints[1];
	tri[1].v[2] = midPoints[0];
	tri[1].t[1] = off + 3;

	tri[2].v[0] = cur.v[2];
	tri[2].v[1] = midPoints[2];
	tri[2].v[2] = midPoints[1];
	tri[2].t[1] = off + 3;
	//	vertexTriangle_[cur.v[0]] = vertexTriangle_[midPoints[0]] = off;
	//	vertexTriangle_[cur.v[1]] = vertexTriangle_[midPoints[1]] = off+1;
	//	vertexTriangle_[cur.v[2]] = vertexTriangle_[midPoints[2]] = off+2;

	midTriangles_[3 * index] = off;
	midTriangles_[3 * index + 1] = off + 1;
	midTriangles_[3 * index + 2] = off + 2;

	for (int i = 0; i < 4; ++i)
		tmpTriangles_.push_back(tri[i]);

	processed_[index] = true;

	// call the neigbor triangles
	for (int i = 0; i < sz; ++i) {
		if (process[i])
		{
			splits_.push_back(SplitData(cur.t[indices[i]], index, midPoints[indices[i]],
				off + (indices[i] + 1) % 3, off + indices[i]));
		}
	}
}


void ObjFile::computeNewVerts()
{
	std::vector<int> ks(newverts_.size(), 0);
	for (std::vector<Triangle>::const_iterator t = triangles_.begin(); t != triangles_.end(); ++t)
	{
		for (int i = 0; i < 3; ++i)
		{
			// all vertices are summed double
			newverts_[t->v[i]] = newverts_[t->v[i]] + vertices_[t->v[(i + 1) % 3]] + vertices_[t->v[(i + 2) % 3]];
			++ks[t->v[i]];
		}
	}

	for (std::vector<Vec3f>::iterator nv = newverts_.begin(); nv != newverts_.end(); ++nv)
	{
		int num = ks[nv - newverts_.begin()];
		float beta = 3.0f / (8.0f * (float)num);
		if (num == 3)
			beta = 3.0f / 16.0f;

		//float interm = (3.0/8.0 - 0.25 * cos ( 2 * M_PI / num ));
		//float beta = 1.0/num*(5.0/8.0 - interm*interm);

		*nv = (*nv * (0.5f * beta)) + vertices_[nv - newverts_.begin()] * (1 - beta*num);

	}
}

void ObjFile::loadNormalIndices()
{
	for (std::vector<Triangle>::iterator t = triangles_.begin(); t != triangles_.end(); ++t) {
		t->n[0] = t->v[0];
		t->n[1] = t->v[1];
		t->n[2] = t->v[2];
	}
}


void ObjFile::computeNormals()
{
	// initialize to zero
	normals_ = std::vector<Vec3f>(vertices_.size());

	for (std::vector<Triangle>::const_iterator t = triangles_.begin(); t != triangles_.end(); ++t)
	{
		Vec3f normal = (vertices_[t->v[1]] - vertices_[t->v[0]]) ^ (
			vertices_[t->v[2]] - vertices_[t->v[0]]);
		normal.normalize();

		normals_[t->v[0]] = normals_[t->v[0]] + normal;
		normals_[t->v[1]] = normals_[t->v[1]] + normal;
		normals_[t->v[2]] = normals_[t->v[2]] + normal;
	}

	std::for_each(normals_.begin(), normals_.end(), std::mem_fun_ref(&Vec3f::normalize));
}
