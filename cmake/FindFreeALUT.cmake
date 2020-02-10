find_package(OpenAL MODULE REQUIRED)

set(FreeALUT_ROOT
  ${EXTERNAL_DIR}
  CACHE
  PATH
  "FreeALUT root directory"
)

find_library(FreeALUT_LIBRARY
  NAMES
    alut
  PATHS
    ${FreeALUT_ROOT}
  PATH_SUFFIXES
    lib
  NO_DEFAULT_PATH
)

mark_as_advanced(FreeALUT_LIBRARY)

find_path(FreeALUT_INCLUDE_DIR
  NAMES
    .
  PATHS
    ${FreeALUT_ROOT}
  PATH_SUFFIXES
    include
  NO_DEFAULT_PATH
)

mark_as_advanced(FreeALUT_INCLUDE_DIR)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(FreeALUT
  DEFAULT_MSG
  FreeALUT_LIBRARY
  FreeALUT_INCLUDE_DIR
)

if(FreeALUT_FOUND AND NOT TARGET FreeALUT::FreeALUT)
  add_library(FreeALUT::FreeALUT
    UNKNOWN
    IMPORTED
  )

  set_target_properties(FreeALUT::FreeALUT
    PROPERTIES
      IMPORTED_LINK_INTERFACE_LANGUAGES C
      INTERFACE_INCLUDE_DIRECTORIES "${OPENAL_INCLUDE_DIR};${FreeALUT_INCLUDE_DIR}"
  )

  set_target_properties(FreeALUT::FreeALUT
    PROPERTIES
      IMPORTED_LOCATION ${FreeALUT_LIBRARY}
      IMPORTED_LINK_INTERFACE_LIBRARIES ${OPENAL_LIBRARY}
  )
endif()
