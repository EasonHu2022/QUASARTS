project "spdlog"
    kind "StaticLib"
    language "C++"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin_obj/" .. outputdir .. "/%{prj.name}")

    files
	{
		"include/spdlog/**.cpp",
		"include/spdlog/**.h",

		"src/**.cpp",
		"src/**.h",

		"**.lua",
	}

	defines
	{
		"SPDLOG_COMPILED_LIB",
	}
	
	includedirs
    {
        "./include",
		"./src"
    }


    filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "Off"

    filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
