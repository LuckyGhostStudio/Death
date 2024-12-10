project "Lucky-ScriptCore"  -- ½Å±¾ÄÚºË
    kind "SharedLib"
    language "C#"
    dotnetframework "4.7.2"

    targetdir ("../LuckyEditor/Resources/Scripts")
    objdir ("../LuckyEditor/Resources/Scripts/Intermediates")

    files 
    {
        "Source/**.cs",
        "Properties/**.cs"
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