project(SlidingPuzzle VERSION 1.0.0 LANGUAGES C)

option(SLIDINGPUZZLE_USE_FREEGLUT "Use FreeGLUT" ON)
option(SLIDINGPUZZLE_USE_GLFW "Use GLFW" OFF)
option(SLIDINGPUZZLE_USE_SDL2 "Use SDL2" OFF)

option(SLIDINGPUZZLE_USE_FREEGLUT_FOR_TEXT "Use FreeGLUT for text" OFF)

option(SLIDINGPUZZLE_USE_FREEALUT_FOR_AUDIO "Use FreeALUT for audio" ON)
option(SLIDINGPUZZLE_USE_SDL2_FOR_AUDIO "Use SDL2 for audio" OFF)

option(SLIDINGPUZZLE_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)

find_package(WhereAmI MODULE REQUIRED)

if(SLIDINGPUZZLE_USE_FREEALUT_FOR_AUDIO)
  find_package(FreeALUT MODULE REQUIRED)
endif()

if(SLIDINGPUZZLE_USE_FREEGLUT OR SLIDINGPUZZLE_USE_FREEGLUT_FOR_TEXT)
  find_package(FreeGLUT MODULE REQUIRED)
endif()

if(SLIDINGPUZZLE_USE_GLFW)
  find_package(GLFW MODULE REQUIRED)
endif()

if(SLIDINGPUZZLE_USE_SDL2 OR SLIDINGPUZZLE_USE_SDL2_FOR_AUDIO)
  find_package(SDL2 MODULE REQUIRED)
endif()

if(UNIX)
  set(CMAKE_INSTALL_RPATH $ORIGIN/../../lib)
endif()

add_executable(SlidingPuzzle
  ../zge/src/assert.c
  ../zge/src/audio.c
  ../zge/src/core.c
  ../zge/src/memory.c
  src/Audio.c
  src/Audio.h
  src/Config.c
  src/Config.h
  src/Graphics.c
  src/Graphics.h
  src/Input.c
  src/Input.h
  src/Logic.c
  src/Logic.h
  src/SlidingPuzzle.c
  src/World.c
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
    ../zge/include
    src
)

target_link_libraries(SlidingPuzzle
  PRIVATE
    WhereAmI::WhereAmI
)

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

install(
  FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/README.md
  DESTINATION
    mini-games/SlidingPuzzle
)

if(SLIDINGPUZZLE_USE_FREEALUT_FOR_AUDIO)
  target_compile_definitions(SlidingPuzzle
    PRIVATE
      USE_FREEALUT_FOR_AUDIO
  )

  target_link_libraries(SlidingPuzzle
    PRIVATE
      FreeALUT::FreeALUT
  )

  if(WIN32)
    install(
      FILES
        ${FreeALUT_LIBRARY}
      DESTINATION
        mini-games/SlidingPuzzle
    )
  elseif(UNIX)
    file(GLOB FreeALUT_LIBRARY_AND_LINKS "${FreeALUT_LIBRARY}.*")

    install(
      FILES
        ${FreeALUT_LIBRARY_AND_LINKS}
      DESTINATION
        mini-games/lib
    )
  endif()
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

  if(WIN32)
    install(
      FILES
        ${FreeGLUT_LIBRARY}
      DESTINATION
        mini-games/SlidingPuzzle
    )
  elseif(UNIX)
    file(GLOB FreeGLUT_LIBRARY_AND_LINKS "${FreeGLUT_LIBRARY}.*")

    install(
      FILES
        ${FreeGLUT_LIBRARY_AND_LINKS}
      DESTINATION
        mini-games/lib
    )
  endif()
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

  if(WIN32)
    install(
      FILES
        ${GLFW_LIBRARY}
      DESTINATION
        mini-games/SlidingPuzzle
    )
  elseif(UNIX)
    file(GLOB GLFW_LIBRARY_AND_LINKS "${GLFW_LIBRARY}.*")

    install(
      FILES
        ${GLFW_LIBRARY_AND_LINKS}
      DESTINATION
        mini-games/lib
    )
  endif()
endif()

if(SLIDINGPUZZLE_USE_SDL2)
  target_compile_definitions(SlidingPuzzle
    PRIVATE
      USE_SDL2
  )
endif()

if(SLIDINGPUZZLE_USE_SDL2_FOR_AUDIO)
  target_compile_definitions(SlidingPuzzle
    PRIVATE
      USE_SDL2_FOR_AUDIO
  )
endif()

if(SLIDINGPUZZLE_USE_SDL2 OR SLIDINGPUZZLE_USE_SDL2_FOR_AUDIO)
  target_link_libraries(SlidingPuzzle
    PRIVATE
      SDL2::SDL2
  )

  if(WIN32)
    install(
      FILES
        ${SDL2_LIBRARY}
      DESTINATION
        mini-games/SlidingPuzzle
    )
  elseif(UNIX)
    file(GLOB SDL2_LIBRARY_AND_LINKS "${SDL2_LIBRARY}.*")

    install(
      FILES
        ${SDL2_LIBRARY_AND_LINKS}
      DESTINATION
        mini-games/lib
    )
  endif()
endif()
