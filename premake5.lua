-- premake.lua - public domain
local BUILD_DIR = ("build/" .. _ACTION)

workspace "Papaya"
   configurations { "Debug", "Release" }
   platforms {"x64", "x32"}
   location (BUILD_DIR)
   startproject "Papaya"

   filter "configurations:Debug"
      defines { "DEBUG" }
      flags { "Symbols", "MultiProcessorCompile" }

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
      flags { "Optimize", "MultiProcessorCompile" }

  
   -- imgui sample_gl3
   project "Papaya"
      kind ("WindowedApp")
      location (BUILD_DIR)
      language "C++"
      targetdir (BUILD_DIR .. "/%{cfg.buildcfg}")
      links {"glfw", "glew"}
      defines { "GLEW_STATIC" }
      flags{ 'Unicode', 'WinMain' }

      files { 
         -- "./src/papaya.cpp", 
         "./src/*.h", 
         "./src/*.impl",
         "./ext/imgui/*.cpp",
         "./ext/imgui/*.h",
         "./ext/nativefiledialog/src/nfd_common.c"
      }

      includedirs  {
         "./ext/",
         "./ext/stb/",
         "./ext/nativefiledialog/src/include/",
         "./ext/imgui/",
         "./ext/easytab/",
         "./ext/glew/include/",
         "./ext/glfw/include/"
      }

      filter "files:**.png"
         buildaction "Embed"

      excludes "./src/papaya.cpp"
      -- filter "files:papaya.cpp"
      --    flags {"ExcludeFromBuild"}

      configuration {"linux"}
         files {"./src/linux*.cpp", "./ext/nativefiledialog/src/nfd_gtk.c"}

      configuration {"windows"}
         files {"./src/win32*.cpp", "./ext/nativefiledialog/src/nfd_win.cpp"}

      configuration {"Macosx"}
         files {"./src/osx*.cpp", "./ext/nativefiledialog/src/nfd_cocoa.m"}

     
      configuration { "linux" }
         links {"X11","Xrandr", "rt", "GL", "GLU", "pthread"}
       
      configuration { "windows" }
         links {"glu32","opengl32", "gdi32", "winmm", "user32"}

      configuration { "Macosx" }
         linkoptions { 
         "-framework OpenGL", 
         "-framework Cocoa", 
         "-framework IOKit",
         "-framework Carbon", 
         "-framework CoreVideo"
       }

   -- GLFW Library
   project "glfw"
      targetdir (BUILD_DIR .. "/lib/%{cfg.buildcfg}")
      location (BUILD_DIR .. "/glfw")
      kind "StaticLib"
      language "C"
      files { 
         "./ext/glfw/src/*.h", 
         "./ext/glfw/include/GL/*.h",          
         "./ext/glfw/src/window.c",
         "./ext/glfw/src/input.c",
         "./ext/glfw/src/init.c",
         "./ext/glfw/src/context.c",
         "./ext/glfw/src/monitor.c"
      }
      includedirs { "./ext/glfw/include", "./ext/glfw/src"}
      defines {"_GLFW_USE_OPENGL"}

      configuration {"linux"}
         files { 
            "./ext/glfw/src/linux_*.c", 
            "./ext/glfw/src/x11_*.c", 
            "./ext/glfw/src/posix*.c",
            "./ext/glfw/src/xkb_unicode.c",
            "./ext/glfw/src/glx_context.c"
         }
         includedirs { "lib/glfw/lib/x11" }
         defines { 
            "_GLFW_USE_LINUX_JOYSTICKS", 
            "_GLFW_HAS_XRANDR", 
            "_GLFW_HAS_PTHREAD" ,
            "_GLFW_HAS_SCHED_YIELD", 
            "_GLFW_HAS_GLXGETPROCADDRESS",
            "_GLFW_GLX"
         }
         buildoptions { "-pthread" }
       
      configuration {"windows"}
         files { 
            "./ext/glfw/src/win32_*.c", 
            "./ext/glfw/src/winmm_joystick.c",
            "./ext/glfw/src/wgl*.c"
         }
         defines { "_GLFW_WIN32", "_GLFW_WGL", "_GLFW_HAS_XINPUT" }
       
      configuration {"Macosx"}
         files { 
            "./ext/glfw/src/cocoa*.c",
            "./ext/glfw/src/posix_tls.c",
            "./ext/glfw/src/mach_time.c",
            "./ext/glfw/src/*.m",
         }
         defines { 
               "_GLFW_COCOA", 
               "_GLFW_NSGL", 
               "_GLFW_USE_CHDIR",
               "_GLFW_USE_MENUBAR", 
               "_GLFW_USE_OPENGL", 
               "_GLFW_USE_RETINA"
           }
         buildoptions { " -fno-common" }
         linkoptions { 
            "-framework OpenGL", 
            "-framework Cocoa", 
            "-framework IOKit", 
            "-framework CoreVideo",
            "-framework Carbon" }

      configuration "Debug"
         defines { "DEBUG" }
         flags { "Symbols", "ExtraWarnings" }

      configuration "Release"
         defines { "NDEBUG" }
         flags { "Optimize", "ExtraWarnings" }    

   -- GLEW Library         
   project "glew"
      targetdir (BUILD_DIR .. "/lib/%{cfg.buildcfg}")
      location (BUILD_DIR .. "/glew")
      kind "StaticLib"
      language "C"
      includedirs {"./ext/glew/include"}
      files {"./ext/glew/src/glew.c"}
      defines { "GLEW_STATIC" }

      configuration "Debug"
         defines { "DEBUG" }
         flags { "Symbols", "ExtraWarnings" }

      configuration "Release"
         defines { "NDEBUG" }
         flags { "Optimize", "ExtraWarnings" }    
