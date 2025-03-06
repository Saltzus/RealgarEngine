
workspace "RealgarEngine"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "Game"
 
   makesettings [[
      CC = gcc
      CXX = g++
   ]]

   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }
      os.execute("Libraries\\vulkan\\Lib\\7-Zip\\7z.exe e Libraries/vulkan/Lib/vulkan-1.7z -oLibraries/vulkan/Lib/ -aoa")

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "ThirdParty"
	include "ThirdParty/Lua/Build-Lua.lua"
   include "ThirdParty/freetype2/Build-freetype2.lua"
   include "ThirdParty/glfw3/Build-glfw3.lua"
group ""

group "Core"
	include "Core/Build-Core.lua"
group ""

include "Game/Build-Game.lua"
include "Editor/Build-Editor.lua"
