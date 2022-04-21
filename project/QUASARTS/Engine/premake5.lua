project "Engine"
	language "C++"
	kind "SharedLib" 
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin_obj/" .. outputdir .. "/%{prj.name}")
	
	root_dir = os.getcwd()
	dir = "%{wks.location}/bin/" .. outputdir .. "/%{prj.name}";
	postbuildcommands { 
		"powershell Copy-Item "..root_dir.."/../ThirdParty/OpenAL/libs/Win32/OpenAL32.dll "..""..dir,
		"powershell Copy-Item "..root_dir.."/../ThirdParty/sndfile/lib/sndfile.dll "..""..dir
	}
	postbuildmessage "copying thirdparty"
	
	files
	{
		"src/*/**.h",
		"src/*/**.cpp",
		"src/**.h",
		"src/**.cpp",
		"**.lua",
		"src/**.vsh",
		"src/**.fsh",
		"src/**.gsh",
		"../ThirdParty/ImGui/*.cpp",
		"../ThirdParty/ImGui/*.h",
		"../ThirdParty/tiny_obj_loader/*.h",
		"../ThirdParty/tiny_obj_loader/*.cc",
		"../ThirdParty/glm/glm/*.hpp",
		"../ThirdParty/glm/glm/*.inl",
		"../ThirdParty/glm/glm/*/*.hpp",
		"../ThirdParty/glm/glm/*/*.inl",
		"../ThirdParty/GLAD/**.c",
		"../ThirdParty/GLAD/**.h",
		"../ThirdParty/lua/**.c",
		"../ThirdParty/lua/**.h",
	}
	libdirs
	{
		"../ThirdParty/OpenAL/libs/Win32",
		"../ThirdParty/sndfile/lib"
	}
	links
	{
		
		"GLFW",
		"spdlog",
		"Glad",
		"lua",
		"Bullet3Collision",
		"Bullet3Common",
		"Bullet3Geometry",
		"BulletCollision",
		"LinearMath",
		"OpenAL32",
		"sndfile",
	}
	
	includedirs
	{
		"./src",
		"../ThirdParty/GLFW/include/GLFW",
		"../ThirdParty/GLFW/include",
		"../ThirdParty/spdlog/include",
		"../ThirdParty/GLAD/include/glad",
		"../ThirdParty/GLAD/include",
		"../ThirdParty/ImGui",
		"../ThirdParty/lua/src",
		"../ThirdParty/sol2/include",
		"../ThirdParty/Bullet3/Bullet3Collision",
		"../ThirdParty/Bullet3/Bullet3Common",
		"../ThirdParty/Bullet3/Bullet3Geometry",
		"../ThirdParty/Bullet3",
		"../ThirdParty/tiny_obj_loader",
		"../ThirdParty/glm",
		"../ThirdParty/OpenAL/include",
		"../ThirdParty/sndfile/include",

	}

	filter "system:windows"
		system "Windows"
		systemversion "latest"
		staticruntime "Off"
		defines
		{
			"_UNICODE",
            "UNICODE",
			"_CONSOLE",
			"GLFW_INCLUDE_NONE",
			"QS_BUILD_DLL",
			"QS_WINDOWS",
		}
		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Editor")
		}

	filter "system:linux"
		system "Linux"
		systemversion "latest"
		staticruntime "Off"
		defines
		{
			"GLFW_INCLUDE_NONE",
			"QS_BUILD_DLL",
		}
		links
		{
			"dl",
			"pthread",
			"X11"
		}
	
	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"