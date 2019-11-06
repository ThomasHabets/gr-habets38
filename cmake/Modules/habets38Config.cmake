INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_HABETS38 habets38)

FIND_PATH(
    HABETS38_INCLUDE_DIRS
    NAMES habets38/api.h
    HINTS $ENV{HABETS38_DIR}/include
        ${PC_HABETS38_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    HABETS38_LIBRARIES
    NAMES gnuradio-habets38
    HINTS $ENV{HABETS38_DIR}/lib
        ${PC_HABETS38_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/habets38Target.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(HABETS38 DEFAULT_MSG HABETS38_LIBRARIES HABETS38_INCLUDE_DIRS)
MARK_AS_ADVANCED(HABETS38_LIBRARIES HABETS38_INCLUDE_DIRS)
