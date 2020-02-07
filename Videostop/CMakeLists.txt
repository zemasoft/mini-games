project(Videostop VERSION 1.0.0 LANGUAGES C)

option(VIDEOSTOP_USE_FREEALUT "Use FreeALUT" ON)
option(VIDEOSTOP_USE_FREEGLUT "Use FreeGLUT" ON)

option(VIDEOSTOP_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)
option(VIDEOSTOP_CLANGFORMAT "Setup clangformat target" OFF)

if(VIDEOSTOP_USE_FREEALUT)
  find_package(ALUT REQUIRED MODULE)  # FreeALUT
endif()

if(VIDEOSTOP_USE_FREEGLUT)
  find_package(FreeGLUT REQUIRED CONFIG)
endif()

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

if(MSVC)
  target_compile_options(Videostop
    PRIVATE
      /W4
  )

  if(VIDEOSTOP_WARNINGS_AS_ERRORS)
    target_compile_options(Videostop
      PRIVATE
        /WX
    )
  endif()
else()
  target_compile_options(Videostop
    PRIVATE
      -Wall
      -Wextra
      -Wpedantic
      -Wconversion
      -Wsign-conversion
  )

  if(VIDEOSTOP_WARNINGS_AS_ERRORS)
    target_compile_options(Videostop
      PRIVATE
        -Werror
    )
  endif()
endif()

target_include_directories(Videostop
  PRIVATE
    src
)

target_link_libraries(Videostop
  PRIVATE
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

if(VIDEOSTOP_USE_FREEGLUT)
  target_compile_definitions(Videostop
    PRIVATE
      USE_FREEGLUT
  )

  target_link_libraries(Videostop
    PRIVATE
      FreeGLUT::freeglut
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
