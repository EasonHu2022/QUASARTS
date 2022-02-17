project "Editor"
	language "C++"
	kind "StaticLib"
	
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
	}

	includedirs
	{
		"../Base/src",
		"../ThirdParty/GLFW/include/GLFW",
		"../ThirdParty/spdlog/include",
	}

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"
		
	
	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"