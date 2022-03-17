project "Engine"
	language "C++"
	kind "SharedLib" 
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin_obj/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"src/**.h",
		"src/**.cpp",
		"**.lua",
	}
	
	links
	{
		"Base",
		"GLFW",
		"spdlog",
		"Glad",
		"ImGui",
		"Bullet3Collision",
		"Bullet3Common",
		"Bullet3Geometry",
		"BulletCollision",
		"LinearMath",
	}
	
	includedirs
	{
		"../Base/src",
		"../ThirdParty/GLFW/include/GLFW",
		"../ThirdParty/spdlog/include",
		"../ThirdParty/GLAD/include/glad",
		"../ThirdParty/GLAD/include",
		"../ThirdParty/ImGui",
		"../ThirdParty/Bullet3/Bullet3Collision",
		"../ThirdParty/Bullet3/Bullet3Common",
		"../ThirdParty/Bullet3/Bullet3Geometry",
		"../ThirdParty/Bullet3",
	}

	filter "system:windows"
		system "Windows"
		systemversion "latest"
		staticruntime "On"
		defines
		{
			"_UNICODE",
            "UNICODE",
			"_CONSOLE",
			"GLFW_INCLUDE_NONE",
			"QS_BUILD_DLL",
		}
		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../Editor/bin/" .. outputdir .. "/Editor")
		}

	filter "system:linux"
		system "Linux"
		systemversion "latest"
		staticruntime "On"
		defines
		{
			"GLFW_INCLUDE_NONE",
			"QS_BUILD_DLL",
		}
		links
		{
			"dl",
			"pthread",
			"X11"
		}
	
	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
