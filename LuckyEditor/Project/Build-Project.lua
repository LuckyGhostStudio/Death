local LuckyRootDir = "../.."

workspace "Project"                 -- ����������� TODO ��̬����Ϊ��Ŀ����
    architecture "x86_64"           -- ��ϵ�ṹ
    startproject "Assembly-CSharp"  -- ������Ŀ

    configurations  -- ����
    {
        "Debug",
        "Release",
        "Dist"
    }

    flags
    {
        "MultiProcessorCompile"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"     -- ���Ŀ¼

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
        "Lucky-ScriptCore"  -- ���ӽű��ں�
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