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
	Vec3d m_gravity; //gravitational acceleration for curved projectiles
	double m_minSpeed; //how fast the particles are projected
	double m_maxSpeed;
	int m_numParticles; //number of particles projected
	float m_initialLife;

	//random distributions
	std::default_random_engine generator1, generator2, generator3;
	std::uniform_real_distribution<double> theta_distribution;
	std::uniform_real_distribution<double> phi_distribution;
	std::uniform_real_distribution<double> speed_distribution;

public:
	ParticleSource() : m_angle(10), m_minSpeed(0.1), m_maxSpeed(1.0), m_initialLife(10.0), m_numParticles(1), m_gravity(0, -10, 0)
	{}

	~ParticleSource();
	//draw the system based on given parameters
	void setGravity(double x, double y, double z) {
		m_gravity[0] = x; m_gravity[1] = y; m_gravity[2] = z;
	}
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

	//does nothing here
	virtual void onDraw();

	//generate new particles
	void newParticles(Particles& particles);

};

#endif