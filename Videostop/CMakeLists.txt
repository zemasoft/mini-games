project(Videostop VERSION 1.0.0 LANGUAGES C)

option(VIDEOSTOP_WARNIGS_AS_ERRORS "Treat warnings as errors" OFF)
option(VIDEOSTOP_CLANGFORMAT "Setup clangformat target" OFF)

include(Utils)

find_package(OpenGL REQUIRED MODULE)
find_package(GLUT REQUIRED MODULE)  # FreeGLUT
find_package(OpenAL REQUIRED MODULE)

link_directories(
  ../freealut/build/src
)

add_executable(Videostop
  src/GameState.c
  src/Graphics.c
  src/Input.c
  src/Logic.c
  src/Videostop.c
  src/Sound.c

  src/GameState.h
  src/Graphics.h
  src/Input.h
  src/Logic.h
  src/Sound.h
)

set_target_compile_options(Videostop
  ${VIDEOSTOP_WARNIGS_AS_ERRORS}
)

target_include_directories(Videostop
  PRIVATE
    ../freealut/include
    src
)

target_link_libraries(Videostop
  PRIVATE
    OpenGL::OpenGL
    ${GLUT_LIBRARY}
    ${OPENAL_LIBRARY}
    alut
    m
)

set_target_properties(Videostop
  PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/Videostop
)

if(VIDEOSTOP_CLANGFORMAT)
  include(ClangFormat)

  target_clangformat_setup(Videostop)
endif()
