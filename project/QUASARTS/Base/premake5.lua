project "Base"
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
	

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"
		
	
	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"