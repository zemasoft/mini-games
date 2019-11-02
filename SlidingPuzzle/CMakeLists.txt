project(SlidingPuzzle VERSION 1.0.0 LANGUAGES C)

option(SLIDINGPUZZLE_WARNIGS_AS_ERRORS "Treat warnings as errors" OFF)
option(SLIDINGPUZZLE_CLANGFORMAT "Setup clangformat target" OFF)

include(Utils)

find_package(OpenGL REQUIRED MODULE)
find_package(GLUT REQUIRED MODULE)  # FreeGLUT
find_package(ALUT REQUIRED MODULE)  # FreeALUT

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

set_target_compile_options(SlidingPuzzle
  ${SLIDINGPUZZLE_WARNIGS_AS_ERRORS}
)

target_include_directories(SlidingPuzzle
  PRIVATE
    ${OPENGL_INCLUDE_DIR}
    ${GLUT_INCLUDE_DIR}
    ${ALUT_INCLUDE_DIRS}
    src
)

target_link_libraries(SlidingPuzzle
  PRIVATE
    OpenGL::OpenGL
    GLUT::GLUT
    ${ALUT_LIBRARIES}
)

set_target_properties(SlidingPuzzle
  PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/SlidingPuzzle
)

if(SLIDINGPUZZLE_CLANGFORMAT)
  include(ClangFormat)

  target_clangformat_setup(SlidingPuzzle)
endif()
