project(SlidingPuzzle VERSION 1.0.0 LANGUAGES C)

option(SLIDINGPUZZLE_USE_FREEALUT "Use FreeALUT" ON)
option(SLIDINGPUZZLE_USE_FREEGLUT "Use FreeGLUT" ON)
option(SLIDINGPUZZLE_USE_FREEGLUT_FOR_TEXT "Use FreeGLUT for text" OFF)
option(SLIDINGPUZZLE_USE_GLFW "Use GLFW" OFF)
option(SLIDINGPUZZLE_USE_SDL2 "Use SDL2" OFF)

option(SLIDINGPUZZLE_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)

find_package(WhereAmI MODULE REQUIRED)

if(SLIDINGPUZZLE_USE_FREEALUT)
  find_package(FreeALUT MODULE REQUIRED)
endif()

if(SLIDINGPUZZLE_USE_FREEGLUT OR SLIDINGPUZZLE_USE_FREEGLUT_FOR_TEXT)
  find_package(FreeGLUT MODULE REQUIRED)
endif()

if(SLIDINGPUZZLE_USE_GLFW)
  find_package(GLFW MODULE REQUIRED)
endif()

if(SLIDINGPUZZLE_USE_SDL2)
  find_package(SDL2 MODULE REQUIRED)
endif()

if(UNIX)
  set(CMAKE_INSTALL_RPATH $ORIGIN/../../lib)
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
      -std=c99
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

target_link_libraries(SlidingPuzzle
  PRIVATE
    WhereAmI::WhereAmI
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
endif()

if(SLIDINGPUZZLE_USE_FREEGLUT_FOR_TEXT)
  target_compile_definitions(SlidingPuzzle
    PRIVATE
      USE_FREEGLUT_FOR_TEXT
  )
endif()

if(SLIDINGPUZZLE_USE_FREEGLUT OR SLIDINGPUZZLE_USE_FREEGLUT_FOR_TEXT)
  target_link_libraries(SlidingPuzzle
    PRIVATE
      FreeGLUT::FreeGLUT
  )
endif()

if(SLIDINGPUZZLE_USE_GLFW)
  target_compile_definitions(SlidingPuzzle
    PRIVATE
      USE_GLFW
  )

  target_link_libraries(SlidingPuzzle
    PRIVATE
      GLFW::GLFW
  )
endif()

if(SLIDINGPUZZLE_USE_SDL2)
  target_compile_definitions(SlidingPuzzle
    PRIVATE
      USE_SDL2
  )

  target_link_libraries(SlidingPuzzle
    PRIVATE
      SDL2::SDL2
  )
endif()

if(WIN32)
  add_custom_command(
    TARGET
      SlidingPuzzle
    PRE_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:SlidingPuzzle>/assets
  )

  install(
    TARGETS
      SlidingPuzzle
    DESTINATION
      mini-games/SlidingPuzzle
  )
elseif(UNIX)
  set_target_properties(SlidingPuzzle
    PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY bin
  )

  add_custom_command(
    TARGET
      SlidingPuzzle
    PRE_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:SlidingPuzzle>/../assets
  )

  install(
    TARGETS
      SlidingPuzzle
    DESTINATION
      mini-games/SlidingPuzzle/bin
  )
endif()

install(
  DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
  DESTINATION
    mini-games/SlidingPuzzle
)
