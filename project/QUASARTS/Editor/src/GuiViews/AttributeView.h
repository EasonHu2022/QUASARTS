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
	void show_transform();
	void change_transform(Engine::TransformComponent*, float[], float[], float[]);
	void show_mesh();

	
	/********************** drawfunctions ************************************/
	void draw_property(std::string name, glm::vec2 target);
	void draw_property(std::string name, glm::vec3 target);
	void draw_property(std::string name, glm::vec4 target);
	void draw_property(std::string name, std::string target);
	void draw_property(std::string name, float target);
	/********************** drawfunctions ************************************/


	
};
