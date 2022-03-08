workspace "QUASARTS" --solution name
	language "C++"
	cppdialect "C++11"
	cdialect "C99"
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
include "ThirdParty/sol2"
include "ThirdParty/Bullet3/Bullet3Collision"
include "ThirdParty/Bullet3/Bullet3Common"
include "ThirdParty/Bullet3/Bullet3Geometry"