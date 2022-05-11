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
		"../Assets/**.vsh",
		"../Assets/**.fsh",
		"../Assets/**.gsh",
		"../ThirdParty/ImGui/*.cpp",
		"../ThirdParty/ImGui/*.h",
		"../ThirdParty/ImGuizmo/*.cpp",
		"../ThirdParty/ImGuizmo/*.h",
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
		"../ThirdParty/stb_image/**.h",
		"../ThirdParty/stb_image/**.cpp"
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
		"sndfile",
		"vorbisenc",
		"FLAC",
		"ogg",
		"opus",
		"vorbis"
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
		"../ThirdParty/ImGuizmo",
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
		"../ThirdParty/stb_image"
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
		links
		{
			"OpenAL32"
		}
		libdirs
		{
			"../ThirdParty/OpenAL/libs/Win32",
			"../ThirdParty/sndfile/libs/Win32"
		}
		postbuildcommands {
			"powershell Copy-Item '"..rootdir.."/ThirdParty/OpenAL/libs/Win32/OpenAL32.dll' '"..dir.."/Engine/'".." -recurse -Force",
			"powershell Copy-Item '"..rootdir.."/ThirdParty/sndfile/libs/Win32/vorbisenc.dll' '"..dir.."/Engine/'".." -recurse -Force",
			"powershell Copy-Item '"..rootdir.."/ThirdParty/sndfile/libs/Win32/FLAC.dll' '"..dir.."/Engine/'".." -recurse -Force",
			"powershell Copy-Item '"..rootdir.."/ThirdParty/sndfile/libs/Win32/ogg.dll' '"..dir.."/Engine/'".." -recurse -Force",
			"powershell Copy-Item '"..rootdir.."/ThirdParty/sndfile/libs/Win32/opus.dll' '"..dir.."/Engine/'".." -recurse -Force",
			"powershell Copy-Item '"..rootdir.."/ThirdParty/sndfile/libs/Win32/vorbis.dll' '"..dir.."/Engine/'".." -recurse -Force",
			"powershell Copy-Item '"..rootdir.."/ThirdParty/sndfile/libs/Win32/sndfile.dll' '"..dir.."/Engine/'".." -recurse -Force",
		}
		postbuildmessage "copying thirdparty"
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
			"openal",
			"dl",
			"pthread",
			"X11"
		}
		libdirs
		{
			"../ThirdParty/OpenAL/libs/Linux",
			"../ThirdParty/sndfile/libs/Linux"
		}
		postbuildcommands {
			"cp -rf '"..rootdir.."/ThirdParty/OpenAL/libs/Linux/OpenAL.so' '"..dir.."/Engine/OpenAL.so'",
			"cp -rf '"..rootdir.."/ThirdParty/sndfile/libs/Linux/vorbisenc.a' '"..dir.."/Engine/vorbisenc.a'",
			"cp -rf '"..rootdir.."/ThirdParty/sndfile/libs/Linux/FLAC.a' '"..dir.."/Engine/FLAC.a'",
			"cp -rf '"..rootdir.."/ThirdParty/sndfile/libs/Linux/ogg.a' '"..dir.."/Engine/ogg.a'",
			"cp -rf '"..rootdir.."/ThirdParty/sndfile/libs/Linux/opus.a' '"..dir.."/Engine/opus.a'",
			"cp -rf '"..rootdir.."/ThirdParty/sndfile/libs/Linux/vorbis.a' '"..dir.."/Engine/vorbis.a'",
			"cp -rf '"..rootdir.."/ThirdParty/sndfile/libs/Linux/sndfile.a' '"..dir.."/Engine/sndfile.a'"
		}
		postbuildmessage "copying thirdparty"
	
	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"