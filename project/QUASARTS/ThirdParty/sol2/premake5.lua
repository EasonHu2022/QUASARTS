project "sol2"
	kind "StaticLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"include/sol/**.hpp",
		"include/sol/compatibility/**.hpp",
		"include/sol/compatibility/**.h",
		"include/sol/detail/**.hpp",
		"include/sol/stack/detail/**.hpp"
	}

	includedirs
	{
		"./include"
	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "On"

	filter "system:linux"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "On"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"