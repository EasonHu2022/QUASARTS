project "Editor"
	language "C++"
	kind "ConsoleApp"
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin_obj/" .. outputdir .. "/%{prj.name}")
	
	postbuildcommands { 
		"powershell Copy-Item '"..rootdir.."/ThirdParty/OpenAL/libs/Win32/OpenAL32.dll' '"..dir.."/Editor/'".." -recurse -Force",
		"powershell Copy-Item '"..rootdir.."/ThirdParty/sndfile/libs/Win32/vorbisenc.dll' '"..dir.."/Editor/'".." -recurse -Force",
		"powershell Copy-Item '"..rootdir.."/ThirdParty/sndfile/libs/Win32/FLAC.dll' '"..dir.."/Editor/'".." -recurse -Force",
		"powershell Copy-Item '"..rootdir.."/ThirdParty/sndfile/libs/Win32/ogg.dll' '"..dir.."/Editor/'".." -recurse -Force",
		"powershell Copy-Item '"..rootdir.."/ThirdParty/sndfile/libs/Win32/opus.dll' '"..dir.."/Editor/'".." -recurse -Force",
		"powershell Copy-Item '"..rootdir.."/ThirdParty/sndfile/libs/Win32/vorbis.dll' '"..dir.."/Editor/'".." -recurse -Force",
		"powershell Copy-Item '"..rootdir.."/ThirdParty/sndfile/libs/Win32/sndfile.dll' '"..dir.."/Editor/'".." -recurse -Force"
	}
	postbuildmessage "copying thirdparty"
	
	files
	{
		"src/**.h",
		"src/**.cpp",
		"src/*/**.h",
		"src/*/**.cpp",
		"**.lua",
		"../ThirdParty/glm/glm/*.hpp",
		"../ThirdParty/glm/glm/*.inl",
		"../ThirdParty/glm/glm/*/*.hpp",
		"../ThirdParty/glm/glm/*/*.inl",
	}
	
	libdirs
	{
		"../ThirdParty/OpenAL/libs/Win32",
		"../ThirdParty/sndfile/libs/Win32"
	}
	
	links
	{
		"Engine",
		"OpenAL32",
		"sndfile",
		"vorbisenc",
		"FLAC",
		"ogg",
		"opus",
		"vorbis",
	}

	includedirs
	{
		"../Engine/src",
		"../ThirdParty/ImGui",
		"../ThirdParty/ImGuizmo",
		"./src",
		"../ThirdParty/GLFW/include/GLFW",
		"../ThirdParty/GLFW/include",
		"../ThirdParty/spdlog/include",
		"../ThirdParty/GLAD/include/glad",
		"../ThirdParty/GLAD/include",
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
		systemversion "latest"
		staticruntime "Off"
		defines
		{
			"_UNICODE",
            "UNICODE",
			"_CONSOLE",
			"GLFW_INCLUDE_NONE",
			"QS_WINDOWS",
		}

	filter "system:linux"
		systemversion "latest"
		staticruntime "Off"
		defines
		{
			"GLFW_INCLUDE_NONE",
		}		
	
	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"