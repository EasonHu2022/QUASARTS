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
		for (auto& [key, value] : emitters) {
			for (int i = 0; i < value.size(); i++) {
				bool alive = value[i].update();
				if (!alive)
					value.erase(value.begin() + i);
			}
			if (emitters[key].empty()) {
				emitters.erase(key);
			}
		}
	}
	
	void ParticleMaster::render() {
		renderer->render(emitters);
	}

	void ParticleMaster::release() {
		renderer->release();
	}

	void ParticleMaster::addParticle(Texture2D* tex, Particle particle) {
		if (emitters.find(tex) == emitters.end()) {
			std::vector<Particle> particles;
			emitters.insert({ tex, particles });
		}
		emitters[tex].push_back(particle);
	}
};

