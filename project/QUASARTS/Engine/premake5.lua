project "Engine"
	language "C++"
	kind "SharedLib" 
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin_obj/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"src/*/**.h",
		"src/*/**.cpp",
		"src/**.h",
		"src/**.cpp",
		"**.lua",
		"../ThirdParty/ImGui/*.cpp",
		"../ThirdParty/ImGui/*.h",
		"../ThirdParty/tiny_obj_loader/*.h",
		"../ThirdParty/tiny_obj_loader/*.cc",
	}
	
	links
	{
		
		"GLFW",
		"spdlog",
		"Glad",
		"Bullet3Collision",
		"Bullet3Common",
		"Bullet3Geometry",
		"BulletCollision",
		"LinearMath",
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
		"../ThirdParty/Bullet3/Bullet3Collision",
		"../ThirdParty/Bullet3/Bullet3Common",
		"../ThirdParty/Bullet3/Bullet3Geometry",
		"../ThirdParty/Bullet3",
		"../ThirdParty/tiny_obj_loader",
	}

	filter "system:windows"
		system "Windows"
		systemversion "latest"
		staticruntime "On"
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
		staticruntime "On"
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
