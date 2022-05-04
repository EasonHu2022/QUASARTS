#pragma once
#include "Application.h"

extern Engine::Application* Engine::create_application();

int main(int argc, char** argv)
{

	//create application for engine
	auto app = Engine::create_application();
	app->init();
	app->start();
	

	
	return 0;
}