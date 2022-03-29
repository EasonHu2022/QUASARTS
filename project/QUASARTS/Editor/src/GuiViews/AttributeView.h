#pragma once
#include "QuasartsEngine.h"
#include <string>
#include "GuiViews/GuiView.h"


class AttributeView : public GuiView
{
public:
	
	void on_add() override;
	void on_gui() override;
	void on_remove() override;

	std::vector<float> v1;
	std::vector<float> v2;
	float vec3f[3];
	float vec3f2[3];
	bool game = false;
	int i = -1;
};
