#include "ParticleMaster.h"


namespace Engine
{
	ParticleMaster::ParticleMaster()
	{
		
	}
	ParticleMaster::~ParticleMaster()
	{

	}

	ParticleMaster* ParticleMaster::instance = nullptr;
	ParticleMaster* ParticleMaster::Instance()
	{
		if (nullptr == instance)
			instance = new ParticleMaster();
		return instance;
	}

	void ParticleMaster::init(RenderContext* renderContext) {

		renderer = new ParticleRenderer(renderContext);
		renderer->init();

	}

	void ParticleMaster::update() {
		for (int i = 0; i < particles.size(); i++) {
			bool alive = particles[i].update();
			if (!alive)
				particles.erase(particles.begin() + i);
		}
	}
	
	void ParticleMaster::render() {
		renderer->render(particles);
	}

	void ParticleMaster::release() {
		renderer->release();
	}

	void ParticleMaster::addParticle(Particle particle) {
		particles.push_back(particle);
	}
};

