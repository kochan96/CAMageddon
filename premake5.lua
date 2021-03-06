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

group "Dependencies"
    include "CAMageddon/vendor/glfw"
    include "CAMageddon/vendor/glad"
    include "CAMageddon/vendor/imgui"
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
    }

    links 
	{ 
		"GLFW",
        "Glad",
        "ImGui",
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