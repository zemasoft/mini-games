project(Snake VERSION 1.0.0 LANGUAGES C)

option(SNAKE_WARNIGS_AS_ERRORS "Treat warnings as errors" OFF)
option(SNAKE_CLANGFORMAT "Setup clangformat target" OFF)

include(Utils)

find_package(OpenGL REQUIRED MODULE)
find_package(GLFW3 REQUIRED MODULE)
find_package(ALUT REQUIRED MODULE)  # FreeALUT

add_executable(Snake
  src/GameState.c
  src/Graphics.c
  src/Input.c
  src/Logic.c
  src/Snake.c
  src/Sound.c

  src/GameState.h
  src/Graphics.h
  src/Input.h
  src/Logic.h
  src/Sound.h
)

set_target_compile_options(Snake
  ${SNAKE_WARNIGS_AS_ERRORS}
)

target_include_directories(Snake
  PRIVATE
    ${ALUT_INCLUDE_DIRS}
    src
)

target_link_libraries(Snake
  PRIVATE
    OpenGL::OpenGL
    ${GLFW3_LIBRARY}
    ${ALUT_LIBRARIES}
    #m
)

set_target_properties(Snake
  PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/Snake
)

if(SNAKE_CLANGFORMAT)
  include(ClangFormat)

  target_clangformat_setup(Snake)
endif()
