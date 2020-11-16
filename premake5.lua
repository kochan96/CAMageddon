workspace "CAMageddon"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "CAMageddon/vendor/glfw/include"
IncludeDir["Glad"] = "CAMageddon/vendor/glad/include"
IncludeDir["ImGui"] = "CAMageddon/vendor/imgui"
IncludeDir["glm"] = "CAMageddon/vendor/glm"
IncludeDir["spdlog"] = "CAMageddon/vendor/spdlog"
IncludeDir["tinygiledialogs"] = "CAMageddon/vendor/tinyfiledialogs"
IncludeDir["tinyxml2"] = "CAMageddon/vendor/tinyxml2"
IncludeDir["stb_image"] = "CAMageddon/vendor/stb_image"

group "Dependencies"
    include "CAMageddon/vendor/glfw"
    include "CAMageddon/vendor/glad"
    include "CAMageddon/vendor/imgui"
    include "CAMageddon/vendor/tinyxml2"
group ""

project "CAMageddon"
    location "CAMageddon"
    kind "ConsoleApp"
    language "C++"
    cppdialect "c++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/assets/**",
    }

    excludes
    {
        "%{prj.name}/assets/textures/**.pdf",
        "%{prj.name}/assets/textures/**.txt",
    }

    defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

    includedirs
    {
        "%{prj.name}/src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.tinyxml2}",
        "%{IncludeDir.tinygiledialogs}",
        "%{IncludeDir.stb_image}"
    }

    links 
	{ 
		"GLFW",
        "Glad",
        "ImGui",
        "tinyxml2",
		"opengl32.lib"
    }

    postbuildcommands 
	{
        '{COPY} "../CAMageddon/assets" "%{cfg.targetdir}/assets"',
        '{COPY} "../CAMageddon/imgui.ini" "%{cfg.targetdir}"'
	}
    
    filter "system:windows"
        systemversion "latest"
        
        defines
        {
        }

    filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		runtime "Release"
		optimize "on"