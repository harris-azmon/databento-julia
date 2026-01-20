#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "databento::databento" for configuration "Release"
set_property(TARGET databento::databento APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(databento::databento PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libdatabento.a"
  )

list(APPEND _cmake_import_check_targets databento::databento )
list(APPEND _cmake_import_check_files_for_databento::databento "${_IMPORT_PREFIX}/lib/libdatabento.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
