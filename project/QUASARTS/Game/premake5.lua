project "Game"
	language "C++"
	kind "StaticLib" --what if linux//todo
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin_obj/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"src/**.h",
		"src/**.cpp",
		"**.lua",
	}
	
	links
	{
		"Engine",
		"Editor",
	}
	
	includedirs
	{
		"../Engine/src",
		"../Editor/src",
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