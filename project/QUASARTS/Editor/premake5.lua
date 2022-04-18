project "Editor"
	language "C++"
	kind "ConsoleApp"
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin_obj/" .. outputdir .. "/%{prj.name}")
	
	
	files
	{
		"src/**.h",
		"src/**.cpp",
		"src/*/**.h",
		"src/*/**.cpp",
		"**.lua",
	}
	
	links
	{
		"Engine"
	}

	includedirs
	{
		"../Engine/src",
		"../ThirdParty/ImGui",
		"./src",
		"../ThirdParty/GLFW/include/GLFW",
		"../ThirdParty/GLFW/include",
		"../ThirdParty/spdlog/include",
		"../ThirdParty/GLAD/include/glad",
		"../ThirdParty/GLAD/include",
		"../ThirdParty/lua/src",
		"../ThirdParty/sol2/include",
		"../ThirdParty/Bullet3/Bullet3Collision",
		"../ThirdParty/Bullet3/Bullet3Common",
		"../ThirdParty/Bullet3/Bullet3Geometry",
		"../ThirdParty/Bullet3",
		"../ThirdParty/tiny_obj_loader",
		"../ThirdParty/glm",
		"../ThirdParty/OpenAL/include/AL",
		"../ThirdParty/sndfile/include",
	}

	filter "system:windows"
		systemversion "latest"
		staticruntime "Off"
		defines
		{
			"_UNICODE",
            "UNICODE",
			"_CONSOLE",
			"GLFW_INCLUDE_NONE",
			"QS_WINDOWS",
		}

	filter "system:linux"
		systemversion "latest"
		staticruntime "Off"
		defines
		{
			"GLFW_INCLUDE_NONE",
		}		
	
	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"