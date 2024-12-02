project "Lucky"         -- 引擎内核
    kind "StaticLib"    -- 静态库
    language "C++"      -- 语言
    cppdialect "C++20"
    staticruntime "off"

    targetdir ("%{wks.location}/Binaries/" .. outputdir .. "/%{prj.name}")              -- 目标目录
    objdir ("%{wks.location}/Binaries/Intermediates/" .. outputdir .. "/%{prj.name}")   -- 中间目录

    pchheader "lcpch.h" -- 预编译头文件
    pchsource "Source/lcpch.cpp"

    files               -- 文件
    {
        "Source/**.h",
        "Source/**.cpp",
        "Vendor/stb_image/**.h",
        "Vendor/stb_image/**.cpp",
        "Vendor/glm/glm/**.hpp",
        "Vendor/glm/glm/**.inl",
        "Vendor/ImGuizmo/ImGuizmo.h",
        "Vendor/ImGuizmo/ImGuizmo.cpp"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS"
    }

    includedirs         -- 附加包含目录
    {
        "Source",
        "Vendor",
        "Vendor/spdlog/include",
        "%{IncludeDir.Box2D}",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.mono}",
        "%{IncludeDir.yaml_cpp}",
        "%{IncludeDir.ImGuizmo}"
    }

    links
    {
        "Box2D",        -- 引用 Box2D
        "GLFW",         -- 引用 GLFW
        "GLAD",         -- 引用 GLAD
        "ImGui",        -- 引用 imgui
        "yaml-cpp",     -- 引用 yaml-cpp
        "opengl32.lib",
        "%{Library.mono}",
    }

    filter "files:Vendor/ImGuizmo/**.cpp"
    flags {"NoPCH"}      -- 该 cpp 文件不使用预编译头文件

    filter "system:windows"     -- windows 系统
        systemversion "latest"  -- sdk版本

        defines
        {
            "GLFW_INCLUDE_NONE"
        }

        links
        {
            "%{Library.WinSock}",
            "%{Library.WinMM}",
            "%{Library.WinVersion}",
            "%{Library.BCrypt}",
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