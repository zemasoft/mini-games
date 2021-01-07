project(ZGE VERSION 1.0.0 LANGUAGES C)

option(ZGE_USE_FREEGLUT "Use FreeGLUT" ON)
option(ZGE_USE_GLFW "Use GLFW" OFF)
option(ZGE_USE_SDL2 "Use SDL2" OFF)

option(ZGE_USE_FREEGLUT_FOR_TEXT "Use FreeGLUT for text" OFF)

option(ZGE_USE_FREEALUT_FOR_AUDIO "Use FreeALUT for audio" ON)
option(ZGE_USE_SDL2_FOR_AUDIO "Use SDL2 for audio" OFF)

option(ZGE_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)

if(ZGE_USE_FREEALUT_FOR_AUDIO)
  find_package(FreeALUT MODULE REQUIRED)
endif()

if(ZGE_USE_FREEGLUT OR ZGE_USE_FREEGLUT_FOR_TEXT)
  find_package(FreeGLUT MODULE REQUIRED)
endif()

if(ZGE_USE_GLFW)
  find_package(GLFW MODULE REQUIRED)
endif()

if(ZGE_USE_SDL2 OR ZGE_USE_SDL2_FOR_AUDIO)
  find_package(SDL2 MODULE REQUIRED)
endif()

add_library(zge
  STATIC
  include/zge/assert.h
  include/zge/audio.h
  include/zge/core.h
  include/zge/memory.h
  src/assert.c
  src/audio.c
  src/core.c
  src/memory.c
)

if(MSVC)
  target_compile_options(zge
    PRIVATE
      /W4
  )

  if(ZGE_WARNINGS_AS_ERRORS)
    target_compile_options(zge
      PRIVATE
        /WX
    )
  endif()
else()
  target_compile_options(zge
    PRIVATE
      -std=c99
      -Wall
      -Wextra
      -Wpedantic
      -Wconversion
      -Wsign-conversion
  )

  if(ZGE_WARNINGS_AS_ERRORS)
    target_compile_options(zge
      PRIVATE
        -Werror
    )
  endif()
endif()

target_include_directories(zge
  PUBLIC
    include
)

if(ZGE_USE_FREEALUT_FOR_AUDIO)
  target_compile_definitions(zge
    PUBLIC
      USE_FREEALUT_FOR_AUDIO
  )

  target_link_libraries(zge
    PUBLIC
      FreeALUT::FreeALUT
  )

  if(WIN32)
    install(
      FILES
        ${FreeALUT_LIBRARY}
      DESTINATION
        mini-games
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

if(ZGE_USE_FREEGLUT)
  target_compile_definitions(zge
    PUBLIC
      USE_FREEGLUT
  )
endif()

if(ZGE_USE_FREEGLUT_FOR_TEXT)
  target_compile_definitions(zge
    PUBLIC
      USE_FREEGLUT_FOR_TEXT
  )
endif()

if(ZGE_USE_FREEGLUT OR ZGE_USE_FREEGLUT_FOR_TEXT)
  target_link_libraries(zge
    PUBLIC
      FreeGLUT::FreeGLUT
  )

  if(WIN32)
    install(
      FILES
        ${FreeGLUT_LIBRARY}
      DESTINATION
        mini-games
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

if(ZGE_USE_GLFW)
  target_compile_definitions(zge
    PUBLIC
      USE_GLFW
  )

  target_link_libraries(zge
    PUBLIC
      GLFW::GLFW
  )

  if(WIN32)
    install(
      FILES
        ${GLFW_LIBRARY}
      DESTINATION
        mini-games
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

if(ZGE_USE_SDL2)
  target_compile_definitions(zge
    PUBLIC
      USE_SDL2
  )
endif()

if(ZGE_USE_SDL2_FOR_AUDIO)
  target_compile_definitions(zge
    PUBLIC
      USE_SDL2_FOR_AUDIO
  )
endif()

if(ZGE_USE_SDL2 OR ZGE_USE_SDL2_FOR_AUDIO)
  target_link_libraries(zge
    PUBLIC
      SDL2::SDL2
  )

  if(WIN32)
    install(
      FILES
        ${SDL2_LIBRARY}
      DESTINATION
        mini-games
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
