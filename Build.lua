workspace "Lucky"               -- 解决方案名称
    architecture "x64"          -- 体系结构
    startproject "LuckyEditor"  -- 启动项目 TODO 无法设置成功

    configurations              -- 配置
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

-- 依赖
group "Dependencies"
    include "Lucky/Vendor/Box2D"        -- 包含 Box2D 目录
    include "Lucky/Vendor/GLFW"         -- 包含 GLFW 目录
    include "Lucky/Vendor/GLAD"         -- 包含 GLAD 目录
    include "Lucky/Vendor/imgui"        -- 包含 imgui 目录
    include "Lucky/Vendor/yaml-cpp"
group ""

include "Dependencies.lua"
-- TODO 修改
group "Core"
    include "Lucky/Build-Lucky.lua"
    include "Lucky-ScriptCore/Build-Lucky-ScriptCore.lua"
group ""

group "Tools"
    include "LuckyEditor/Build-LuckyEditor.lua"
group ""