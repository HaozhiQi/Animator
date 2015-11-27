#ifndef __PARTICLE_SOURCE_H__
#define __PARTICLE_SOURCE_H__

#include <list>
#include <map>
#include <random>
#include "model.h"
#include "../vec.h"

struct Particle
{
public:
	Vec3d position; //double for precision
	Vec3d velocity;
	float life;
};

typedef list<Particle> Particles; //use list to remove particles more easily

/**
 * The sole purpose of this class is to facilitate adding of new
 * particles.
 */
class ParticleSource : public Model
{
	//parameters for the particle source
	double m_angle; //360 degrees for a spherical explosion. Spherical symmetry with respect to z axis.
	double m_minSpeed; //how fast the particles are projected
	double m_maxSpeed;
	int m_numParticles; //number of particles projected
	float m_initialLife;

	//random distributions
	std::default_random_engine generator;
	std::uniform_real_distribution<double> theta_distribution;
	std::uniform_real_distribution<double> phi_distribution;
	std::uniform_real_distribution<double> speed_distribution;

	Vec3d m_lastPos;
	float m_lastTime;
	Vec3d m_velocity; //velocity of particle source in world coordinate
	bool m_emit;
public:
	ParticleSource(ModelNames name) : Model(name), m_angle(30), m_minSpeed(5.0), m_maxSpeed(10.0), m_initialLife(3.0), m_numParticles(5), m_velocity(0, 0, 0), m_lastPos(0, 0, 0), m_lastTime(9999999999)
	{}

	~ParticleSource();
	
	void setAngle(double angle) {
		m_angle = angle;
	}
	void setMinSpeed(double minSpeed) {
		m_minSpeed = minSpeed;
	}
	void setMasSpeed(double maxSpeed) {
		m_maxSpeed = maxSpeed;
	}
	void setNumParticles(int numParticles) {
		m_numParticles = numParticles;
	}
	void setInitialLife(double initialLife) {
		m_initialLife = initialLife;
	}
	void setEmit(bool emit) {
		m_emit = emit;
	}

	//does nothing here
	virtual void onDraw();

	//generate new particles
	void newParticles(Particles& particles);

};

#endif