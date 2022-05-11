#pragma once

#include "Particle.h"
namespace Engine
{
	Particle::Particle(glm::vec3 pos, glm::vec3 vel, float grav, float life, float rot, float scal)
	{
		position = pos;
		velocity = vel;
		gravity = grav;
		lifetime = life;
		rotation = rot;
		scale = scal;

	}
	Particle::~Particle()
	{
		
	}
	
};