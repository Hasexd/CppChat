workspace "CppChat"
    architecture "x86_64"
    startproject "CppChat"

    configurations
    {
        "Debug",
        "Release"
    }

    flags
    {
        "MultiProcessorCompile"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "vendor/glfw/include"
IncludeDir["Glad"] = "vendor/glad/include"
IncludeDir["ImGui"] = "vendor/ImGui"

include "vendor/glfw"
include "vendor/ImGui"

project "CppChat"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++23"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp",
        "vendor/glad/src/glad.c"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
        "GLFW_INCLUDE_NONE"
    }

    includedirs
    {
        "src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
    }

    links
    {
        "GLFW",
        "ImGui",
        "opengl32"
    }

    vpaths 
    {
        ["Header Files"] = { "src/**.h" },
        ["Source Files"] = { "src/**.cpp" }
    }

    filter "system:windows"
        systemversion "latest"
        links
        {
            "opengl32",
            "gdi32",
            "shell32",
            "user32"
        }

    filter "system:linux"
        systemversion "latest"

        links
        {
            "GL",
            "dl",
            "pthread",
            "X11",
            "Xrandr",
            "Xi",
            "Xcursor",
            "Xinerama"
        }

    filter "configurations:Debug"
        defines "CPPCHAT_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "CPPCHAT_RELEASE"
        runtime "Release"
        optimize "on"
