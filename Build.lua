workspace "Lucky"               -- 解决方案名称
    architecture "x64"          -- 体系结构
    startproject "LuckyEditor"  -- 启动项目

    configurations              -- 配置
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"     -- 输出目录

-- 包含目录列表
IncludeDir = {}
-- IncludeDir["Box2D"] = "Lucky/Vendor/Box2D/include"
IncludeDir["GLFW"] = "Lucky/Vendor/GLFW/include"
IncludeDir["GLAD"] = "Lucky/Vendor/GLAD/include"
IncludeDir["ImGui"] = "Lucky/Vendor/imgui"
IncludeDir["glm"] = "Lucky/Vendor/glm"
IncludeDir["stb_image"] = "Lucky/Vendor/stb_image"
IncludeDir["entt"] = "Lucky/Vendor/entt/include"
IncludeDir["yaml_cpp"] = "Lucky/Vendor/yaml-cpp/include"
-- IncludeDir["ImGuizmo"] = "Lucky/Vendor/ImGuizmo"

group "Dependencies"
    -- include "Lucky/Vendor/Box2D"     -- 包含 Box2D 目录
    include "Lucky/Vendor/GLFW"         -- 包含 GLFW 目录
    include "Lucky/Vendor/GLAD"         -- 包含 GLAD 目录
    include "Lucky/Vendor/imgui"        -- 包含 imgui 目录
    include "Lucky/Vendor/yaml-cpp"

group ""

project "Lucky"         -- 项目
    location "Lucky"    -- 相对路径
    kind "StaticLib"    -- 静态库
    language "C++"      -- 语言
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("Binaries/" .. outputdir .. "/%{prj.name}")              -- 目标目录
    objdir ("Binaries/Intermediates/" .. outputdir .. "/%{prj.name}")   -- 中间目录

    pchheader "lcpch.h" -- 预编译头文件
    pchsource "Lucky/Source/lcpch.cpp"

    files               -- 文件
    {
        "%{prj.name}/Source/**.h",
        "%{prj.name}/Source/**.cpp",
        "%{prj.name}/Vendor/stb_image/**.h",
        "%{prj.name}/Vendor/stb_image/**.cpp",
        "%{prj.name}/Vendor/glm/glm/**.hpp",
        "%{prj.name}/Vendor/glm/glm/**.inl",
        -- "%{prj.name}/Vendor/ImGuizmo/ImGuizmo.h",
        -- "%{prj.name}/Vendor/ImGuizmo/ImGuizmo.cpp"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS"
    }

    includedirs         -- 附加包含目录
    {
        "%{prj.name}/Source",
        "%{prj.name}/Vendor/spdlog/include",
        -- "%{IncludeDir.Box2D}",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.yaml_cpp}",
        -- "%{IncludeDir.ImGuizmo}"
    }

    links
    {
        -- "Box2D",     -- 引用 Box2D
        "GLFW",         -- 引用 GLFW
        "GLAD",         -- 引用 GLAD
        "ImGui",        -- 引用 imgui
        "yaml-cpp",     -- 引用 yaml-cpp
        "opengl32.lib"
    }

    -- filter "files:Lucky/Vendor/ImGuizmo/**.cpp"
    -- flags {"NoPCH"}      -- 该 cpp 文件不使用预编译头文件

    filter "system:windows"     -- windows 系统
        systemversion "latest"  -- sdk版本

        defines
        {
            "GLFW_INCLUDE_NONE"
        }

    filter "configurations:Debug"       -- Debug
        defines "LC_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"     -- Release
        defines "LC_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"        -- Dist
        defines "LC_DIST"
        runtime "Release"
        optimize "on"

project "LuckyEditor"
    location "LuckyEditor"  -- 相对路径
    kind "ConsoleApp"       -- 控制台应用
    language "C++"          -- 语言
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("Binaries/" .. outputdir .. "/%{prj.name}")              -- 目标目录
    objdir ("Binaries/Intermediates/" .. outputdir .. "/%{prj.name}")   -- 中间目录

    files
    {
        "%{prj.name}/Source/**.h",
        "%{prj.name}/Source/**.cpp"
    }

    includedirs
    {
        "Lucky/Vendor/spdlog/include",
        "Lucky/Source",
        "Lucky/Vendor",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.entt}",
        -- "%{IncludeDir.ImGuizmo}"
    }

    links
    {
        "Lucky"
    }

    filter "system:windows"     -- windows
        systemversion "latest"

        defines
        {

        }

    filter "configurations:Debug"
        defines "LC_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "LC_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "LC_DIST"
        runtime "Release"
        optimize "on"