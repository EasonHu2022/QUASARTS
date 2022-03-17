project "Editor"
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
		"Engine",
	}

	includedirs
	{
		"../Base/src",
		"../Engine/src",
		"../ThirdParty/GLFW/include/GLFW",
		"../ThirdParty/spdlog/include",
		"../ThirdParty/ImGui",
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