project "spdlog"
    kind "StaticLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin_obj/" .. outputdir .. "/%{prj.name}")

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
		staticruntime "On"

    filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"