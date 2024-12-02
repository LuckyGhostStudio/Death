project "Lucky"         -- �����ں�
    kind "StaticLib"    -- ��̬��
    language "C++"      -- ����
    cppdialect "C++20"
    staticruntime "off"

    targetdir ("%{wks.location}/Binaries/" .. outputdir .. "/%{prj.name}")              -- Ŀ��Ŀ¼
    objdir ("%{wks.location}/Binaries/Intermediates/" .. outputdir .. "/%{prj.name}")   -- �м�Ŀ¼

    pchheader "lcpch.h" -- Ԥ����ͷ�ļ�
    pchsource "Source/lcpch.cpp"

    files               -- �ļ�
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

    includedirs         -- ���Ӱ���Ŀ¼
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
        "Box2D",        -- ���� Box2D
        "GLFW",         -- ���� GLFW
        "GLAD",         -- ���� GLAD
        "ImGui",        -- ���� imgui
        "yaml-cpp",     -- ���� yaml-cpp
        "opengl32.lib",
        "%{Library.mono}",
    }

    filter "files:Vendor/ImGuizmo/**.cpp"
    flags {"NoPCH"}      -- �� cpp �ļ���ʹ��Ԥ����ͷ�ļ�

    filter "system:windows"     -- windows ϵͳ
        systemversion "latest"  -- sdk�汾

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