newoption {
   trigger = "with_zlib",
   description = "Include zlib support"
}
newoption {
   trigger = "with_bzip2",
   description = "Include BZip2 support"
}
newoption {
   trigger = "with_png",
   description = "Include PNG support"
}
newoption {
   trigger = "with_harfbuzz",
   description = "Include HarfBuzz support"
}
newoption {
   trigger = "ios_platform",
   description = "Specify iOS platform (OS or SIMULATOR)"
}
newoption {
   trigger = "build_framework",
   description = "Build as an OS X Framework"
}

project "freetype2"
  language "C"
  kind "StaticLib"
  targetdir "../Binaries/%{cfg.buildcfg}/ThirdParty/"
  staticruntime "off"

  -- Basic definitions and include directories
  defines { "FT2_BUILD_LIBRARY" }
  includedirs { "include", "include/freetype" }

  -- Source files (using wildcards for convenience)
  files {
      "src/autofit/autofit.c",
      "src/base/ftbase.c",
      "src/base/ftbbox.c",
      "src/base/ftbdf.c",
      "src/base/ftbitmap.c",
      "src/base/ftcid.c",
      "src/base/ftfstype.c",
      "src/base/ftgasp.c",
      "src/base/ftglyph.c",
      "src/base/ftgxval.c",
      "src/base/ftinit.c",
      "src/base/ftmm.c",
      "src/base/ftotval.c",
      "src/base/ftpatent.c",
      "src/base/ftpfr.c",
      "src/base/ftstroke.c",
      "src/base/ftsynth.c",
      "src/base/ftsystem.c",
      "src/base/fttype1.c",
      "src/base/ftwinfnt.c",
      "src/bdf/bdf.c",
      "src/bzip2/ftbzip2.c",
      "src/cache/ftcache.c",
      "src/cff/cff.c",
      "src/cid/type1cid.c",
      "src/gzip/ftgzip.c",
      "src/lzw/ftlzw.c",
      "src/pcf/pcf.c",
      "src/pfr/pfr.c",
      "src/psaux/psaux.c",
      "src/pshinter/pshinter.c",
      "src/psnames/psnames.c",
      "src/raster/raster.c",
      "src/sfnt/sfnt.c",
      "src/smooth/smooth.c",
      "src/truetype/truetype.c",
      "src/type1/type1.c",
      "src/type42/type42.c",
      "src/winfonts/winfnt.c"
  }

  targetdir ("../../Binaries/" .. OutputDir .. "/ThirdParty/%{prj.name}")
  objdir ("../../Binaries/Intermediates/" .. OutputDir .. "/ThirdParty/%{prj.name}")

  -- Platform-specific handling for the debug file.
  -- Default to the Unix debug file...
  files { "src/base/ftdebug.c"}
  -- ...but if building on Windows, remove the default and use the Windows-specific version.
  filter "system:windows"
     removefiles { "src/base/ftdebug.c" }
     files { "builds/windows/ftdebug.c", "src/base/ftver.rc" }
  filter {}  -- clear filter

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


  -- External dependency settings.
  if _OPTIONS["with_zlib"] then
     links { "zlib" }
     defines { "FT_CONFIG_OPTION_SYSTEM_ZLIB" }
  end
  if _OPTIONS["with_bzip2"] then
     links { "bzip2" }
     defines { "FT_CONFIG_OPTION_USE_BZIP2" }
  end
  if _OPTIONS["with_png"] then
     links { "png" }
     defines { "FT_CONFIG_OPTION_USE_PNG" }
  end
  if _OPTIONS["with_harfbuzz"] then
     links { "harfbuzz" }
     defines { "FT_CONFIG_OPTION_USE_HARFBUZZ" }
  end

  -- iOS-specific settings.
  if _OPTIONS["ios_platform"] then
     filter "system:ios"
        kind "StaticLib"   -- iOS builds use static libraries
        -- (Additional iOS-specific flags and toolchain settings may be added here.)
     filter {}
  end

  -- OS X Framework build settings.
  if _OPTIONS["build_framework"] then
     filter "system:macosx"
        kind "SharedLib"  -- Frameworks are built as shared libraries
        framework "true"
        xcodebuildsettings {
           ["INFOPLIST_FILE"] = "builds/mac/freetype-Info.plist",
           ["ARCHS"] = "$(ARCHS_STANDARD_32_64_BIT)",
           ["INSTALL_PATH"] = "@rpath"
        }
     filter {}
  end
