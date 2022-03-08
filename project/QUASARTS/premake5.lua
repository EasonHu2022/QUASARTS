workspace "QUASARTS" --solution name
	language "C++"
	architecture "x86_64"
	configurations
	{
		"Debug",
		"Release"
	}
	startproject "Engine"
	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	
include "Game"	
include "Editor"
include "Engine"
include "Base"
include "ThirdParty/GLFW"
include "ThirdParty/spdlog"
include "ThirdParty/GLAD"
include "ThirdParty/ImGui"

