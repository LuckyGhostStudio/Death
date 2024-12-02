project "LuckyEditor"
    kind "ConsoleApp"       -- 控制台应用
    language "C++"          -- 语言
    cppdialect "C++20"
    staticruntime "off"

    targetdir ("%{wks.location}/Binaries/" .. outputdir .. "/%{prj.name}")              -- 目标目录
    objdir ("%{wks.location}/Binaries/Intermediates/" .. outputdir .. "/%{prj.name}")   -- 中间目录

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