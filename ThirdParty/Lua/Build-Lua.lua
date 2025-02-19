project "Lua"
   kind "StaticLib"
   language "C"
   targetdir "../Binaries/%{cfg.buildcfg}"
   staticruntime "off"

   files { "Source/*.c", "Source/*.h", "Source/*.hpp" }
   removefiles { "Source/lua.c", "Source/luac.c" }

   includedirs
   {
      "Source"
   }

   targetdir ("../../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

   filter "system:windows"
       systemversion "latest"
       defines { }

    filter "system:linux"
       links { "png", "z", "bz2" }

   filter "configurations:Debug"
       defines { "DEBUG" }
       runtime "Debug"
       symbols "On"

   filter "configurations:Release"
       defines { "RELEASE" }
       runtime "Release"
       optimize "On"
       symbols "On"

   filter "configurations:Dist"
       defines { "DIST" }
       runtime "Release"
       optimize "On"
       symbols "Off"