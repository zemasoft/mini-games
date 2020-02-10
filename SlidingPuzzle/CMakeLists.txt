project(SlidingPuzzle VERSION 1.0.0 LANGUAGES C)

option(SLIDINGPUZZLE_USE_FREEALUT "Use FreeALUT" ON)
option(SLIDINGPUZZLE_USE_FREEGLUT "Use FreeGLUT" ON)
option(SLIDINGPUZZLE_USE_GLFW "Use GLFW" OFF)

option(SLIDINGPUZZLE_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)
option(SLIDINGPUZZLE_CLANGFORMAT "Setup clangformat target" OFF)

if(SLIDINGPUZZLE_USE_FREEALUT)
  find_package(FreeALUT MODULE REQUIRED)
endif()

if(SLIDINGPUZZLE_USE_FREEGLUT)
  find_package(FreeGLUT CONFIG REQUIRED)
endif()

if(SLIDINGPUZZLE_USE_GLFW)
  find_package(OpenGL MODULE REQUIRED)
  find_package(glfw3 CONFIG REQUIRED)
endif()

add_executable(SlidingPuzzle
  src/Config.c
  src/Graphics.c
  src/Input.c
  src/Logic.c
  src/SlidingPuzzle.c
  src/Sound.c
  src/World.c

  src/Config.h
  src/Graphics.h
  src/Input.h
  src/Logic.h
  src/Sound.h
  src/World.h
)

if(MSVC)
  target_compile_options(SlidingPuzzle
    PRIVATE
      /W4
  )

  if(SLIDINGPUZZLE_WARNINGS_AS_ERRORS)
    target_compile_options(SlidingPuzzle
      PRIVATE
        /WX
    )
  endif()
else()
  target_compile_options(SlidingPuzzle
    PRIVATE
      -Wall
      -Wextra
      -Wpedantic
      -Wconversion
      -Wsign-conversion
  )

  if(SLIDINGPUZZLE_WARNINGS_AS_ERRORS)
    target_compile_options(SlidingPuzzle
      PRIVATE
        -Werror
    )
  endif()
endif()

target_include_directories(SlidingPuzzle
  PRIVATE
    src
)

if(SLIDINGPUZZLE_USE_FREEALUT)
  target_compile_definitions(SlidingPuzzle
    PRIVATE
      USE_FREEALUT
  )

  target_link_libraries(SlidingPuzzle
    PRIVATE
      FreeALUT::FreeALUT
  )
endif()

if(SLIDINGPUZZLE_USE_FREEGLUT)
  target_compile_definitions(SlidingPuzzle
    PRIVATE
      USE_FREEGLUT
  )

  target_link_libraries(SlidingPuzzle
    PRIVATE
      FreeGLUT::freeglut
  )
endif()

if(SLIDINGPUZZLE_USE_GLFW)
  target_compile_definitions(SlidingPuzzle
    PRIVATE
      USE_GLFW
  )

  target_link_libraries(SlidingPuzzle
    PRIVATE
      OpenGL::OpenGL
      glfw
  )
endif()

set_target_properties(SlidingPuzzle
  PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/SlidingPuzzle
)

if(SLIDINGPUZZLE_CLANGFORMAT)
  include(ClangFormat)

  target_clangformat_setup(SlidingPuzzle)
endif()
