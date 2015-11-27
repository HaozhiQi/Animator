#pragma warning(disable : 4786)

#include "particleSystem.h"
#include "modelerapp.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <limits.h>


/***************
 * Constructors
 ***************/

ParticleSystem::ParticleSystem() : bake_fps(30), simulate(false), dirty(false), m_gravity(0, -10, 0), m_groundY(-10.0)
{
	// TODO

}





/*************
 * Destructor
 *************/

ParticleSystem::~ParticleSystem() 
{
	// TODO

}


/******************
 * Simulation fxns
 ******************/

/** Start the simulation */
void ParticleSystem::startSimulation(float t)
{
#ifdef _DEBUG
	printf("start simulation %f\n", t);
#endif
	// TODO
	if (!simulate) {
		bake_start_time = t;
		clearBaked();
	}
	// These values are used by the UI ...
	// -ve bake_end_time indicates that simulation
	// is still progressing, and allows the
	// indicator window above the time slider
	// to correctly show the "baked" region
	// in grey.
	bake_end_time = -1;
	simulate = true;
	dirty = true;

}

/** Stop the simulation */
void ParticleSystem::stopSimulation(float t)
{
    
	// TODO
	bake_end_time = t;
	// These values are used by the UI
	simulate = false;
	dirty = true;

}

/** Reset the simulation */
void ParticleSystem::resetSimulation(float t)
{
    
	// TODO

	// These values are used by the UI
	simulate = false;
	dirty = true;

}

/** Compute forces and update particles **/
void ParticleSystem::computeForcesAndUpdateParticles(float t)
{
	m_gravity[1] = -VAL(PS_GRAVITY);
#ifdef _DEBUG
#if 0 //just curious
	if (m_cache.find(t) == m_cache.end())
		printf("NEW time %f\n", t);
	else
		printf("OLD time %f\n", t);
	m_cache[t];
#endif
#endif
	//only handle unbaken situation
	if (m_cache.find(get_frame(t)) == m_cache.end()) { //no, not baken
		if (simulate) {
			//initialize
			//the last tick
			auto last_iter = m_cache.rbegin();
			if (last_iter == m_cache.rend()) { //the ticks are empty
				//this is the very first tick in history
				//add new particles
				for (auto particleSrc : m_particleSrcs) {
					particleSrc->newParticles(m_cache[get_frame(t)]);
				}
			}
			else { //there's a last tick
				float delta_t = 1.0f / bake_fps;
				int last_frame = last_iter->first;
				for (int frame = last_frame + 1; frame <= get_frame(t); ++frame) {
#ifdef _DEBUG
					printf("simulating frame %d\n", frame);
#endif
					m_cache[frame] = m_cache[frame-1]; //copy the last tick state

					//add new particles
					for (auto particleSrc : m_particleSrcs) {
						particleSrc->newParticles(m_cache[frame]);
					}

					//simulate the particles
					auto iter = m_cache[frame].begin();
					while (iter != m_cache[frame].end()) {
						Particle& particle = *iter;
						particle.life -= delta_t;

						//kill the ones that are dead
						if (particle.life < 0) {
							iter = m_cache[frame].erase(iter);
							continue;
						}
						else {
							++iter;
						}

						//evolve
						particle.position += particle.velocity * delta_t;
						if (ModelerApplication::Instance()->rb() && particle.position[1] < m_groundY) {
							particle.position[1] = 2 * m_groundY - particle.position[1];
							particle.velocity[1] = -particle.velocity[1];
						}
						else {
							particle.velocity += m_gravity * delta_t;
						}
					}
				}	
			}
		}
	}
}


/** Render particles */
void ParticleSystem::drawParticles(float t)
{
	auto iter = m_cache.find(get_frame(t));
	if (iter == m_cache.end()) return; //do nothing if this frame is not yet baken

	glPushMatrix();
	glTranslated(-10, m_groundY, -10);
	drawBox(20, 0.1, 20);
	glPopMatrix();

	Particles& particles = iter->second;
	for (Particle & particle : particles) {
		glPushMatrix();
		glTranslated(particle.position[0], particle.position[1], particle.position[2]);
		drawSphere(0.1);
		glPopMatrix();
	}
}





/** Adds the current configuration of particles to
  * your data structure for storing baked particles **/
void ParticleSystem::bakeParticles(float t) 
{
	//UNUSED
	// TODO
}

/** Clears out your data structure of baked particles */
void ParticleSystem::clearBaked()
{
	m_cache.clear();
	// TODO
}





