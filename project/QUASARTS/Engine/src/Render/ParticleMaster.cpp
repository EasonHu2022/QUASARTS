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
			for (int i = 0; i < value.second.size(); i++) {
				bool alive = value.second[i].update();
				if (!alive)
					value.second.erase(value.second.begin() + i);
			}
		}
	}
	
	void ParticleMaster::render() {
		renderer->render(emitters);
	}

	void ParticleMaster::release() {
		renderer->release();
	}

	void ParticleMaster::addParticle(std::string texName, ParticleTexture tex, Particle particle) {
		if (!(emitters.count(texName))) {
			std::pair<ParticleTexture, std::vector<Particle>> pair;
			std::vector<Particle> particles;
			pair.first = tex;
			pair.second = particles;
			emitters.insert(std::make_pair(texName, pair));
		}
		emitters[texName].second.push_back(particle);
	}
};

