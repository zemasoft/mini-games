project(Snake VERSION 1.0.0 LANGUAGES C)

option(SNAKE_USE_FREEALUT "Use FreeALUT" ON)
option(SNAKE_USE_FREEGLUT "Use FreeGLUT" OFF)
option(SNAKE_USE_GLFW "Use GLFW" ON)
option(SNAKE_USE_SDL2 "Use SDL2" OFF)

option(SNAKE_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)

if(SNAKE_USE_FREEALUT)
  find_package(FreeALUT MODULE REQUIRED)
endif()

if(SNAKE_USE_FREEGLUT)
  find_package(FreeGLUT CONFIG REQUIRED)
endif()

if(SNAKE_USE_GLFW)
  find_package(OpenGL MODULE REQUIRED)
  find_package(glfw3 CONFIG REQUIRED)
endif()

if(SNAKE_USE_SDL2)
  find_package(SDL2 MODULE REQUIRED)
endif()

add_executable(Snake
  src/Config.c
  src/Graphics.c
  src/Input.c
  src/Logic.c
  src/Snake.c
  src/Sound.c
  src/Tools.c
  src/World.c

  src/Config.h
  src/Graphics.h
  src/Input.h
  src/Logic.h
  src/Sound.h
  src/Tools.h
  src/World.h
)

if(MSVC)
  target_compile_options(Snake
    PRIVATE
      /W4
  )

  if(SNAKE_WARNINGS_AS_ERRORS)
    target_compile_options(Snake
      PRIVATE
        /WX
    )
  endif()
else()
  target_compile_options(Snake
    PRIVATE
      -std=c99
      -Wall
      -Wextra
      -Wpedantic
      -Wconversion
      -Wsign-conversion
  )

  if(SNAKE_WARNINGS_AS_ERRORS)
    target_compile_options(Snake
      PRIVATE
        -Werror
    )
  endif()
endif()

target_include_directories(Snake
  PRIVATE
    src
)

target_link_libraries(Snake
  PRIVATE
    m
)

if(SNAKE_USE_FREEALUT)
  target_compile_definitions(Snake
    PRIVATE
      USE_FREEALUT
  )

  target_link_libraries(Snake
    PRIVATE
      FreeALUT::FreeALUT
  )
endif()

if(SNAKE_USE_FREEGLUT)
  target_compile_definitions(Snake
    PRIVATE
      USE_FREEGLUT
  )

  target_link_libraries(Snake
    PRIVATE
      FreeGLUT::freeglut
  )
endif()

if(SNAKE_USE_GLFW)
  target_compile_definitions(Snake
    PRIVATE
      USE_GLFW
  )

  target_link_libraries(Snake
    PRIVATE
      OpenGL::OpenGL
      glfw
  )
endif()

if(SNAKE_USE_SDL2)
  target_compile_definitions(Snake
    PRIVATE
      USE_SDL2
  )

  target_link_libraries(Snake
    PRIVATE
      SDL2::SDL2
  )
endif()

set_target_properties(Snake
  PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/Snake
)

install(
  TARGETS
    Snake
  DESTINATION
    bin
)
