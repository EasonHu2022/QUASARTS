#pragma once
#include "QuasartsEngine.h"
#include <string>
#include "GuiViews/GuiView.h"
#include <memory>
#include <map>
#include <unordered_map>
#include <functional>

class AttributeView : public GuiView
{
public:
	
	void on_add() override;
	void on_gui() override;
	void on_remove() override;

	bool game = false;
private:
	size_t current_entity_id;


	

private:
	void show_transform();
	void change_transform(Engine::TransformComponent*, float[], float[], float[]);
	void show_mesh();
	void show_collision_sphere();
	void show_material();
	void show_lighting();
	void show_script();
	void show_orbit();
	void show_particle();

};
