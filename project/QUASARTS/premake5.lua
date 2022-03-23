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
	startproject "Editor"
	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	
include "Game"	
include "Editor"
include "Engine"
group "ThirdParty"
	include "ThirdParty/GLFW"
	include "ThirdParty/spdlog"
	include "ThirdParty/GLAD"
	include "ThirdParty/sol2"
	include "ThirdParty/lua"
	include "ThirdParty/Bullet3/Bullet3Collision"
	include "ThirdParty/Bullet3/Bullet3Common"
	include "ThirdParty/Bullet3/Bullet3Geometry"
	include "ThirdParty/Bullet3/BulletCollision"
	include "ThirdParty/Bullet3/LinearMath"