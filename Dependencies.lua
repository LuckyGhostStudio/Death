-- °üº¬Ä¿Â¼
IncludeDir = {}
IncludeDir["Box2D"] = "%{wks.location}/Lucky/Vendor/Box2D/include"
IncludeDir["GLFW"] = "%{wks.location}/Lucky/Vendor/GLFW/include"
IncludeDir["GLAD"] = "%{wks.location}/Lucky/Vendor/GLAD/include"
IncludeDir["ImGui"] = "%{wks.location}/Lucky/Vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/Lucky/Vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/Lucky/Vendor/stb_image"
IncludeDir["entt"] = "%{wks.location}/Lucky/Vendor/entt/include"
IncludeDir["mono"] = "%{wks.location}/Lucky/vendor/mono/include"
IncludeDir["yaml_cpp"] = "%{wks.location}/Lucky/Vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/Lucky/Vendor/ImGuizmo"

-- ¿âÄ¿Â¼
LibraryDir = {}
LibraryDir["mono"] = "%{wks.location}/Lucky/vendor/mono/lib/%{cfg.buildcfg}"

-- Libs
Library = {}
Library["mono"] = "%{LibraryDir.mono}/libmono-static-sgen.lib"

-- Windows
Library["WinSock"] = "Ws2_32.lib"
Library["WinMM"] = "Winmm.lib"
Library["WinVersion"] = "Version.lib"
Library["BCrypt"] = "Bcrypt.lib"