project "Core"
   kind "StaticLib"
   language "C++"
   cppdialect "C++17"
   targetdir "Binaries/%{cfg.buildcfg}"
   staticruntime "off"

   
   libdirs { "../Binaries/" .. OutputDir .. "/**", "../Libraries/vulkan/Lib"}
   files { "Source/**.h", "Source/**.cpp", "../Libraries/glad/src/**.c", "../Libraries/freetype/src/**.c"}

   dependson { "Lua", "freetype2", "glfw3" }

   includedirs
   {
      "Source",
      "Source/**",

      -- Include Lua
      "../ThirdParty/Lua/Source",

      -- Include freetype2
      "../ThirdParty/freetype2/src",
      "../ThirdParty/freetype2/include",

      -- Include glfw
      "../ThirdParty/glfw3/include",


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
      -- json
      "../Libraries/nlohmann_json"
   }

   links
   {
      "Lua",
      "glfw3",
      "freetype2",
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