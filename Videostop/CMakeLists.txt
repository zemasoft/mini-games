project(Videostop VERSION 1.0.0 LANGUAGES C)

option(VIDEOSTOP_USE_FREEALUT "Use FreeALUT" ON)

option(VIDEOSTOP_WARNIGS_AS_ERRORS "Treat warnings as errors" OFF)
option(VIDEOSTOP_CLANGFORMAT "Setup clangformat target" OFF)

include(Utils)

if(VIDEOSTOP_USE_FREEALUT)
  find_package(ALUT REQUIRED MODULE)  # FreeALUT
endif()

find_package(FreeGLUT REQUIRED CONFIG)

add_executable(Videostop
  src/Config.c
  src/Graphics.c
  src/Input.c
  src/Logic.c
  src/Sound.c
  src/Videostop.c
  src/World.c

  src/Config.h
  src/Graphics.h
  src/Input.h
  src/Logic.h
  src/Sound.h
  src/World.h
)

set_target_compile_options(Videostop
  ${VIDEOSTOP_WARNIGS_AS_ERRORS}
)

target_include_directories(Videostop
  PRIVATE
    src
)

target_link_libraries(Videostop
  PRIVATE
    FreeGLUT::freeglut
    m
)

if(VIDEOSTOP_USE_FREEALUT)
  target_compile_definitions(Videostop
    PRIVATE
      USE_FREEALUT
  )

  target_include_directories(Videostop
    PRIVATE
      ${ALUT_INCLUDE_DIRS}
  )

  target_link_libraries(Videostop
    PRIVATE
      ${ALUT_LIBRARIES}
  )
endif()

set_target_properties(Videostop
  PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/Videostop
)

if(VIDEOSTOP_CLANGFORMAT)
  include(ClangFormat)

  target_clangformat_setup(Videostop)
endif()
