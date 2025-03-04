
workspace "RedEngine"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "Game"

   filter "system:windows"
      makesettings [[
         CC = gcc
         CXX = g++
      ]]

   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "ThirdParty"
	include "ThirdParty/Lua/Build-Lua.lua"
group ""

group "Core"
	include "Core/Build-Core.lua"
group ""

include "Game/Build-Game.lua"
include "Editor/Build-Editor.lua"
