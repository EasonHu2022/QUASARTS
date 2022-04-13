project "Bullet3Geometry"

    kind "StaticLib"
    language "C++"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin_obj/" .. outputdir .. "/%{prj.name}")

    files
	{
		"**.cpp",
		"**.h",

		"**.lua"
	}
	
	includedirs
    {
		".."
    }

    filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "Off"

	filter "system:linux"
		pic "On"

    filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"