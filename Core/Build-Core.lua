project "Core"
   kind "StaticLib"
   language "C++"
   cppdialect "C++20"
   targetdir "Binaries/%{cfg.buildcfg}"
   staticruntime "off"

   files { "Source/**.h", "Source/**.cpp", "../Libraries/glad/src/**.c", "../Libraries/freetype/src/**.c"}


   includedirs
   {
      "Source",

      -- glfw
      "../Libraries/glfw-3.4/include",
      -- Opengl / glad
      "../Libraries/glad/include",
      -- glm
      "../Libraries/glm",
      -- stb_image
      "../Libraries/stb_image",
      -- miniaudio
      "../Libraries/miniaudio",
      -- freetype
      "../Libraries/freetype/freetype2/include"
   }

   links
   {
      "glfw3",
      "freetype"
   }

   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

   filter "system:windows"
       systemversion "latest"
       defines { }

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