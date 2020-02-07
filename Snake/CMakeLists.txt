project(Snake VERSION 1.0.0 LANGUAGES C)

option(SNAKE_USE_FREEALUT "Use FreeALUT" ON)

option(SNAKE_WARNIGS_AS_ERRORS "Treat warnings as errors" OFF)
option(SNAKE_CLANGFORMAT "Setup clangformat target" OFF)

include(Utils)

if(SNAKE_USE_FREEALUT)
  find_package(ALUT REQUIRED MODULE)  # FreeALUT
endif()

find_package(OpenGL REQUIRED MODULE)
find_package(glfw3 REQUIRED CONFIG)

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

set_target_compile_options(Snake
  ${SNAKE_WARNIGS_AS_ERRORS}
)

target_include_directories(Snake
  PRIVATE
    src
)

target_link_libraries(Snake
  PRIVATE
    OpenGL::OpenGL
    glfw
    m
)

if(SNAKE_USE_FREEALUT)
  target_compile_definitions(Snake
    PRIVATE
      USE_FREEALUT
  )

  target_include_directories(Snake
    PRIVATE
      ${ALUT_INCLUDE_DIRS}
  )

  target_link_libraries(Snake
    PRIVATE
      ${ALUT_LIBRARIES}
  )
endif()

set_target_properties(Snake
  PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/Snake
)

if(SNAKE_CLANGFORMAT)
  include(ClangFormat)

  target_clangformat_setup(Snake)
endif()
