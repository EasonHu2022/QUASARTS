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
	}
	
	includedirs
	{
		"../Base/src",
		"../Editor/src",
		"../ThirdParty/GLFW/include/GLFW",
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
		}
		
	
	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"