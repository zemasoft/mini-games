project(Videostop VERSION 1.0.0 LANGUAGES C)

option(VIDEOSTOP_USE_FREEALUT "Use FreeALUT" ON)
option(VIDEOSTOP_USE_FREEGLUT "Use FreeGLUT" ON)
option(VIDEOSTOP_USE_FREEGLUT_FOR_TEXT "Use FreeGLUT for text" OFF)
option(VIDEOSTOP_USE_GLFW "Use GLFW" OFF)
option(VIDEOSTOP_USE_SDL2 "Use SDL2" OFF)

option(VIDEOSTOP_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)

find_package(WhereAmI MODULE REQUIRED)

if(VIDEOSTOP_USE_FREEALUT)
  find_package(FreeALUT MODULE REQUIRED)
endif()

if(VIDEOSTOP_USE_FREEGLUT OR VIDEOSTOP_USE_FREEGLUT_FOR_TEXT)
  find_package(FreeGLUT MODULE REQUIRED)
endif()

if(VIDEOSTOP_USE_GLFW)
  find_package(GLFW MODULE REQUIRED)
endif()

if(VIDEOSTOP_USE_SDL2)
  find_package(SDL2 MODULE REQUIRED)
endif()

if(UNIX)
  set(CMAKE_INSTALL_RPATH $ORIGIN/../../lib)
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
      -std=c99
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
    WhereAmI::WhereAmI
    m
)

if(WIN32)
  add_custom_command(
    TARGET
      Videostop
    PRE_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:Videostop>/assets
  )

  install(
    TARGETS
      Videostop
    DESTINATION
      mini-games/Videostop
  )
elseif(UNIX)
  set_target_properties(Videostop
    PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY bin
  )

  add_custom_command(
    TARGET
      Videostop
    PRE_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:Videostop>/../assets
  )

  install(
    TARGETS
      Videostop
    DESTINATION
      mini-games/Videostop/bin
  )
endif()

install(
  DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
  DESTINATION
    mini-games/Videostop
)

if(VIDEOSTOP_USE_FREEALUT)
  target_compile_definitions(Videostop
    PRIVATE
      USE_FREEALUT
  )

  target_link_libraries(Videostop
    PRIVATE
      FreeALUT::FreeALUT
  )

  install(
    FILES
      ${FreeALUT_LIBRARY}
    DESTINATION
      mini-games/lib
  )
endif()

if(VIDEOSTOP_USE_FREEGLUT)
  target_compile_definitions(Videostop
    PRIVATE
      USE_FREEGLUT
  )
endif()

if(VIDEOSTOP_USE_FREEGLUT_FOR_TEXT)
  target_compile_definitions(Videostop
    PRIVATE
      USE_FREEGLUT_FOR_TEXT
  )
endif()

if(VIDEOSTOP_USE_FREEGLUT OR VIDEOSTOP_USE_FREEGLUT_FOR_TEXT)
  target_link_libraries(Videostop
    PRIVATE
      FreeGLUT::FreeGLUT
  )

  install(
    FILES
      ${FreeGLUT_LIBRARY}
    DESTINATION
      mini-games/lib
  )
endif()

if(VIDEOSTOP_USE_GLFW)
  target_compile_definitions(Videostop
    PRIVATE
      USE_GLFW
  )

  target_link_libraries(Videostop
    PRIVATE
      GLFW::GLFW
  )

  install(
    FILES
      ${GLFW_LIBRARY}
    DESTINATION
      mini-games/lib
  )
endif()

if(VIDEOSTOP_USE_SDL2)
  target_compile_definitions(Videostop
    PRIVATE
      USE_SDL2
  )

  target_link_libraries(Videostop
    PRIVATE
      SDL2::SDL2
  )

  install(
    FILES
      ${SDL2_LIBRARY}
    DESTINATION
      mini-games/lib
  )
endif()
