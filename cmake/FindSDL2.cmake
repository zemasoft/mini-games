find_package(OpenGL MODULE REQUIRED)

set(SDL2_ROOT
  ${EXTERNAL_DIR}
  CACHE
  PATH
  "SDL2 root directory"
)

find_library(SDL2_LIBRARY
  NAMES
    SDL2
  PATHS
    ${SDL2_ROOT}
  PATH_SUFFIXES
    lib
  NO_DEFAULT_PATH
)

mark_as_advanced(SDL2_LIBRARY)

find_path(SDL2_INCLUDE_DIR
  NAMES
    .
  PATHS
    ${SDL2_ROOT}
  PATH_SUFFIXES
    include
  NO_DEFAULT_PATH
)

mark_as_advanced(SDL2_INCLUDE_DIR)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SDL2
  DEFAULT_MSG
  SDL2_LIBRARY
  SDL2_INCLUDE_DIR
)

if(SDL2_FOUND AND NOT TARGET SDL2::SDL2)
  add_library(SDL2::SDL2
    UNKNOWN
    IMPORTED
  )

  set_target_properties(SDL2::SDL2
    PROPERTIES
      IMPORTED_LINK_INTERFACE_LANGUAGES C
      INTERFACE_INCLUDE_DIRECTORIES "${OPENGL_INCLUDE_DIR};${SDL2_INCLUDE_DIR}"
  )

  set_target_properties(SDL2::SDL2
    PROPERTIES
      IMPORTED_LOCATION ${SDL2_LIBRARY}
      IMPORTED_LINK_INTERFACE_LIBRARIES ${OPENGL_LIBRARY}
  )
endif()
