project "Engine"
	language "C++"
	kind "ConsoleApp" 
	
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
		"Editor",
		"GLFW",
		"spdlog",
		"Glad",
		"ImGui",
	}
	
	includedirs
	{
		"../Base/src",
		"../Editor/src",
		"../ThirdParty/GLFW/include/GLFW",
		"../ThirdParty/spdlog/include",
		"../ThirdParty/GLAD/include/glad",
		"../ThirdParty/GLAD/include",
		"../ThirdParty/ImGui",
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
		}

	filter "system:linux"
		system "Linux"
		systemversion "latest"
		staticruntime "On"
		defines
		{
			"GLFW_INCLUDE_NONE"
		}
		
	
	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
