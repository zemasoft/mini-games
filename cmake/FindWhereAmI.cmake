set(WhereAmI_ROOT
  $ENV{DEPS_DIR}
  CACHE
  PATH
  "WhereAmI root directory"
)

find_library(WhereAmI_LIBRARY
  NAMES
    whereami
  PATHS
    ${WhereAmI_ROOT}
  PATH_SUFFIXES
    lib
  NO_DEFAULT_PATH
)

mark_as_advanced(WhereAmI_LIBRARY)

find_path(WhereAmI_INCLUDE_DIR
  NAMES
    .
  PATHS
    ${WhereAmI_ROOT}
  PATH_SUFFIXES
    include
  NO_DEFAULT_PATH
)

mark_as_advanced(WhereAmI_INCLUDE_DIR)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(WhereAmI
  DEFAULT_MSG
  WhereAmI_LIBRARY
  WhereAmI_INCLUDE_DIR
)

if(WhereAmI_FOUND AND NOT TARGET WhereAmI::WhereAmI)
  add_library(WhereAmI::WhereAmI
    UNKNOWN
    IMPORTED
  )

  set_target_properties(WhereAmI::WhereAmI
    PROPERTIES
      IMPORTED_LINK_INTERFACE_LANGUAGES C
      INTERFACE_INCLUDE_DIRECTORIES ${WhereAmI_INCLUDE_DIR}
  )

  set_target_properties(WhereAmI::WhereAmI
    PROPERTIES
      IMPORTED_LOCATION ${WhereAmI_LIBRARY}
  )
endif()
