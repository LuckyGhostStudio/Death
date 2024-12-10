local LuckyRootDir = "../.."

workspace "Project"                 -- 解决方案名称 TODO 动态设置为项目名称
    architecture "x86_64"           -- 体系结构
    startproject "Assembly-CSharp"  -- 启动项目

    configurations  -- 配置
    {
        "Debug",
        "Release",
        "Dist"
    }

    flags
    {
        "MultiProcessorCompile"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"     -- 输出目录

project "Assembly-CSharp"
    kind "SharedLib"
    language "C#"
    dotnetframework "4.7.2"

    targetdir ("Binaries")
    objdir ("Intermediates")

    files 
    {
        "Assets/**.cs",
        "Properties/**.cs"
    }

    links
    {
        "Lucky-ScriptCore"  -- 链接脚本内核
    }

    filter "configurations:Debug"
        optimize "Off"
        symbols "Default"

    filter "configurations:Release"
        optimize "On"
        symbols "Default"

    filter "configurations:Dist"
        optimize "Full"
        symbols "Off"

group "Lucky"
    include (LuckyRootDir .. "/Lucky-ScriptCore/Build-Lucky-ScriptCore.lua")
group ""