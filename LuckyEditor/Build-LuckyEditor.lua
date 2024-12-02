project "LuckyEditor"
    kind "ConsoleApp"       -- ����̨Ӧ��
    language "C++"          -- ����
    cppdialect "C++20"
    staticruntime "off"

    targetdir ("%{wks.location}/Binaries/" .. outputdir .. "/%{prj.name}")              -- Ŀ��Ŀ¼
    objdir ("%{wks.location}/Binaries/Intermediates/" .. outputdir .. "/%{prj.name}")   -- �м�Ŀ¼

    files
    {
        "Source/**.h",
        "Source/**.cpp"
    }

    includedirs
    {
        "%{wks.location}/Lucky/Vendor/spdlog/include",
        "%{wks.location}/Lucky/Source",
        "%{wks.location}/Lucky/Vendor",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.ImGuizmo}"
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