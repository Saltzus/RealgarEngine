project "Core"
   kind "StaticLib"
   language "C++"
   cppdialect "C++20"
   targetdir "Binaries/%{cfg.buildcfg}"
   staticruntime "off"

   
   libdirs { "../Libraries/glfw-3.4", "../Libraries/freetype/freetype2/include", "../Libraries/vulkan/Lib"}
   files { "Source/**.h", "Source/**.cpp", "../Libraries/glad/src/**.c", "../Libraries/freetype/src/**.c"}


   includedirs
   {
      "Source",

      -- glfw
      "../Libraries/glfw-3.4/include",
      -- Opengl / glad
      "../Libraries/glad/include",
      -- Vulkan
      "../Libraries/vulkan/Include",
      -- glm
      "../Libraries/glm",
      -- stb_image
      "../Libraries/stb_image",
      -- miniaudio
      "../Libraries/miniaudio",
      -- freetype
      "../Libraries/freetype/freetype2/include",
      "../Libraries/freetype/"
   }

   links
   {
      "glfw3",
      "freetype",
      "vulkan-1"
   }

   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

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