project "Bullet3Collision"

    kind "StaticLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin_obj/" .. outputdir .. "/%{prj.name}")

    files
	{
		"**.cpp",
		"**.h",

		"**.lua",
	}
	
	includedirs
    {
		".."
    }

    filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "On"

	filter "system:linux"
		pic "On"

    filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"