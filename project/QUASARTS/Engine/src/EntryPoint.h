#pragma once
#include "Application.h"
#include "QEngine.h"

extern Engine::Application* Engine::create_application();

int main(int argc, char** argv)
{
	auto engineInst = new Engine::QEngine();

	//create application for engine
	auto app = Engine::create_application();

	engineInst->init(app);

	engineInst->start();
	
	return 0;
}