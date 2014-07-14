# Findgtest.cmake - Find gogole test
# Modified from FindGlog.cmake by alexs.mac@gmail.com (Alex Stewart)
#
# Findgtest.cmake - Find Google test library.
#
# This module defines the following variables:
#
# GTEST_FOUND: TRUE iff gtest is found.
# GTEST_INCLUDE_DIRS: Include directories for gtest.
# GTEST_LIBRARIES: Libraries required to link gtest.
#
# The following variables control the behaviour of this module:
#
# GTEST_INCLUDE_DIR_HINTS: List of additional directories in which to
#                          search for gtest includes, e.g: /foo/include.
# GTEST_LIBRARY_DIR_HINTS: List of additional directories in which to
#                          search for gtest libraries, e.g: /bar/lib.
#
# The following variables are also defined by this module, but in line with
# CMake recommended FindPackage() module style should NOT be referenced directly
# by callers (use the plural variables detailed above instead).  These variables
# do however affect the behaviour of the module via FIND_[PATH/LIBRARY]() which
# are NOT re-called (i.e. search for library is not repeated) if these variables
# are set with valid values _in the CMake cache_. This means that if these
# variables are set directly in the cache, either by the user in the CMake GUI,
# or by the user passing -DVAR=VALUE directives to CMake when called (which
# explicitly defines a cache variable), then they will be used verbatim,
# bypassing the HINTS variables and other hard-coded search locations.
#
# GTEST_INCLUDE_DIR: Include directory for gtest, not including the
#                    include directory of any dependencies.
# GTEST_LIBRARY: gtest library, not including the libraries of any
#                dependencies.

# Called if we failed to find gtest or any of it's required dependencies,
# unsets all public (designed to be used externally) variables and reports
# error message at priority depending upon [REQUIRED/QUIET/<NONE>] argument.
macro(GTEST_REPORT_NOT_FOUND REASON_MSG)
    unset(GTEST_FOUND)
    unset(GTEST_INCLUDE_DIRS)
    unset(GTEST_LIBRARIES)
    # Make results of search visible in the CMake GUI if gtest has not
    # been found so that user does not have to toggle to advanced view.
    mark_as_advanced(CLEAR GTEST_INCLUDE_DIR GTEST_LIBRARY)
    # Note <package>_FIND_[REQUIRED/QUIETLY] variables defined by FindPackage()
    # use the camelcase library name, not uppercase.
    if(Gtest_FIND_QUIETLY)
        message(STATUS "Failed to find gtest - " ${REASON_MSG} ${ARGN})
    elseif(Gtest_FIND_REQUIRED)
        message(FATAL_ERROR "Failed to find gtest - " ${REASON_MSG} ${ARGN})
    else()
        # Neither QUIETLY nor REQUIRED, use no priority which emits a message
        # but continues configuration and allows generation.
        message("-- Failed to find gtest - " ${REASON_MSG} ${ARGN})
    endif()
endmacro(GTEST_REPORT_NOT_FOUND)

# Search user-installed locations first, so that we prefer user installs
# to system installs where both exist.
list(APPEND GTEST_CHECK_INCLUDE_DIRS
    /usr/local/include
    /usr/local/homebrew/include # Mac OS X
    /opt/local/var/macports/software # Mac OS X.
    /opt/local/include
    /usr/include)
list(APPEND GTEST_CHECK_LIBRARY_DIRS
    /usr/local/lib
    /usr/local/homebrew/lib # Mac OS X.
    /opt/local/lib
    /usr/lib)

# Check general hints
if(GTEST_HINTS AND EXISTS ${GTEST_HINTS})
    set(GTEST_INCLUDE_DIR_HINTS ${GTEST_HINTS}/include)
    set(GTEST_LIBRARY_DIR_HINTS ${GTEST_HINTS}/lib/.libs)
if(GTEST_HINTS AND EXISTS ${GTEST_HINTS})

# Search supplied hint directories first if supplied.
find_path(GTEST_INCLUDE_DIR
    NAMES gtest/gtest.h
    PATHS ${GTEST_INCLUDE_DIR_HINTS}
          ${GTEST_CHECK_INCLUDE_DIRS}
    NO_DEFAULT_PATH)
if(NOT GTEST_INCLUDE_DIR OR NOT EXISTS ${GTEST_INCLUDE_DIR})
GTEST_REPORT_NOT_FOUND("Could not find gtest include directory, "
    "set GTEST_INCLUDE_DIR to directory containing gtest/gtest.h")
endif(NOT GTEST_INCLUDE_DIR OR NOT EXISTS ${GTEST_INCLUDE_DIR})

find_library(GTEST_LIBRARY
    NAMES gtest
    PATHS ${GTEST_LIBRARY_DIR_HINTS}
          ${GTEST_CHECK_LIBRARY_DIRS}
    NO_DEFAULT_PATH)
if(NOT GTEST_LIBRARY OR NOT EXISTS ${GTEST_LIBRARY})
GTEST_REPORT_NOT_FOUND("Could not find gtest library, "
    "set GTEST_LIBRARY to full path to libgtest.")
else(NOT GTEST_LIBRARY OR NOT EXISTS ${GTEST_LIBRARY})
    string(REGEX MATCH ".*/" GTEST_LIBRARY_DIR ${GTEST_LIBRARY})
endif(NOT GTEST_LIBRARY OR NOT EXISTS ${GTEST_LIBRARY})

# Mark internally as found, then verify. GTEST_REPORT_NOT_FOUND() unsets
# if called.
set(GTEST_FOUND TRUE)

# Catch case when caller has set GTEST_INCLUDE_DIR in the cache / GUI and
# thus FIND_[PATH/LIBRARY] are not called, but specified locations are
# invalid, otherwise we would report the library as found.
if(GTEST_INCLUDE_DIR AND NOT EXISTS ${GTEST_INCLUDE_DIR}/gtest/gtest.h)
GTEST_REPORT_NOT_FOUND("Caller defined GTEST_INCLUDE_DIR:"
    " ${GTEST_INCLUDE_DIR} does not contain gtest/gtest.h header.")
endif(GTEST_INCLUDE_DIR AND NOT EXISTS ${GTEST_INCLUDE_DIR}/gtest/gtest.h)

# TODO: This regex for gtest library is pretty primitive, we use lowercase
#       for comparison to handle Windows using CamelCase library names, could
#       this check be better?
string(TOLOWER "${GTEST_LIBRARY}" LOWERCASE_GTEST_LIBRARY)
if(GTEST_LIBRARY AND NOT "${LOWERCASE_GTEST_LIBRARY}" MATCHES ".*gtest[^/]*")
GTEST_REPORT_NOT_FOUND("Caller defined GTEST_LIBRARY: "
    "${GTEST_LIBRARY} does not match gtest.")
endif(GTEST_LIBRARY AND NOT "${LOWERCASE_GTEST_LIBRARY}" MATCHES ".*gtest[^/]*")

# Set standard CMake FindPackage variables if found.
if(GTEST_FOUND)
    set(GTEST_INCLUDE_DIRS ${GTEST_INCLUDE_DIR})
    file(GLOB GTEST_LIBRARIES ${GTEST_LIBRARY_DIR}lib*.so)
endif(GTEST_FOUND)

# Handle REQUIRED / QUIET optional arguments.
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Gtest DEFAULT_MSG
    GTEST_INCLUDE_DIRS GTEST_LIBRARIES)

# Only mark internal variables as advanced if we found gtest, otherwise
# leave them visible in the standard GUI for the user to set manually.
if(GTEST_FOUND)
    mark_as_advanced(FORCE GTEST_INCLUDE_DIR GTEST_LIBRARY)
endif(GTEST_FOUND)
