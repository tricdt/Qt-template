cmake_minimum_required(VERSION 3.22)

project(QtExtra
    VERSION 1.0.0
    DESCRIPTION "Advanced C++/Qt Extra Utilities Monorepo"
    LANGUAGES CXX
)

# Modern CMake policies
if(POLICY CMP0071)
    cmake_policy(SET CMP0071 NEW)
endif()
if(POLICY CMP0087)
    cmake_policy(SET CMP0087 NEW)
endif()

# set old policy for CMAKE_CXX_SCAN_FOR_MODULES for clang
cmake_policy(SET CMP0155 OLD)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} CONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG} "${CMAKE_BINARY_DIR}/bin")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONFIG} "${CMAKE_BINARY_DIR}/bin")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONFIG} "${CMAKE_BINARY_DIR}/lib")
endforeach()

# Automatically include ${CMAKE_CURRENT_SOURCE_DIR} and ${CMAKE_CURRENT_BINARY_DIR}
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

# Set include dir src (used by target_include_directories in src/log4qt)
set(QTEXTRA_BUILD_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# in-source builds should be avoided
set(QTEXTRA_MODULE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${QTEXTRA_MODULE_DIR}")
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(QtExtraConfigMacros)
include(CPM)

qtextra_setup_options()
qtextra_setup_build()

set(COMPONENT_TARGETS_ENABLED NO) # Note: DO not set! CK
set(PROJECT_NAMESPACE ${PROJECT_NAME})
set(PROJECT_VERSION_INCLUDE_DIR ${PROJECT_BINARY_DIR}/PackageProjectInclude)
include_directories(${PROJECT_VERSION_INCLUDE_DIR})

set(NAMESPACE_HEADER_LOCATION ${CMAKE_CURRENT_LIST_DIR}/include)
set(INCLUDE_INSTALL_DIR include/${PROJECT_NAMESPACE}-${PROJECT_VERSION}) # XXX VERSION_SUFFIX
string(TOLOWER ${PROJECT_NAMESPACE} PROJECT_HEADER_LOCATION)
# TODO: needed to be lower case? CK

# add_subdirectory(src/qtextraaux)
# add_subdirectory(src/qtgeometry)
# add_subdirectory(src/qtwidgetsextra)
# add_subdirectory(src/testlib)

foreach(_module IN LISTS QTEXTRA_AVAILABLE_MODULES)
    if(BUILD_${_module})
        string(TOLOWER "${_module}" _module_lower)
        if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/${_module_lower}/CMakeLists.txt")
            add_subdirectory("src/${_module_lower}")
        endif()
    endif()
endforeach()

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# github workflow files
add_custom_target(githubworkflows SOURCES
    ".github/workflows/cmake-ubuntu.yml"
    ".github/workflows/cmake-windows.yml"
    ".github/workflows/cmake-macos.yml")

# project documentation
add_custom_target(documentation SOURCES
    "Readme.md"
    "ChangeLog.md"
    "Configuration.md")


# ---- Create an installable target ----
# this allows users to install and find the libraries via `find_package()`.

# the location where the project's version header will be placed should match the project's regular
# header paths
string(TOLOWER ${PROJECT_NAMESPACE}/version.h PROJECT_VERSION_HEADER)
string(TOLOWER ${PROJECT_NAMESPACE}/${PROJECT_NAMESPACE}_export.h PROJECT_EXPORT_HEADER)
# TODO: needed to be lower case? CK

# Note: This needs to be done after all install(TARGETS ...) commands!
# generate and install export targets file
install(
    EXPORT ${PROJECT_NAMESPACE}Targets
    FILE ${PROJECT_NAMESPACE}Targets.cmake
    NAMESPACE ${PROJECT_NAMESPACE}::
    DESTINATION lib/cmake/${PROJECT_NAMESPACE}-${PROJECT_VERSION} # XXX VERSION_SUFFIX
)

# Packaging
# https://cmake.org/cmake/help/latest/module/CPack.html
set( CPACK_PACKAGE_NAME ${PROJECT_NAME} )
set( CPACK_PACKAGE_VENDOR "MEON Medical" ) # Github project owner
set( CPACK_PACKAGE_DESCRIPTION_SUMMARY "QtExtra for the Qt cross-platform application framework" )
set( CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/tricdt/qt-template" )
set( CPACK_PACKAGE_CONTACT      "https://github.com/tricdt/qt-template" )
set( CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR} )
set( CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR} )
set( CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH} )
set( CPACK_PACKAGE_VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH} )
set( CPACK_PACKAGE_INSTALL_DIRECTORY ${PROJECT_NAME} )
set( CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE" )
set( CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/Readme.md" )

set( CPACK_COMPONENT_Library_DISPLAY_NAME "QtExtra Library" )
set( CPACK_COMPONENT_Library_DESCRIPTION "The QtExtra binary library." )
set( CPACK_COMPONENT_Library_REQUIRED 1 )
set( CPACK_COMPONENT_Devel_DISPLAY_NAME "QtExtra Development Files" )
set( CPACK_COMPONENT_Devel_DESCRIPTION "Development files for compiling against QtExtra." )
set( CPACK_COMPONENT_Devel_REQUIRED 0 )

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")

    if(NOT CPACK_PACKAGE_ARCHITECTURE)
        # Note: the architecture should default to the local architecture, but it
        # in fact comes up empty.  We call `uname -m` to ask the kernel instead.
        EXECUTE_PROCESS( COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE CPACK_PACKAGE_ARCHITECTURE )
    endif()

    set( CPACK_INCLUDE_TOPLEVEL_DIRECTORY 1 )
    set( CPACK_PACKAGE_RELEASE 1 )


    # RPM - https://cmake.org/cmake/help/latest/cpack_gen/rpm.html
    set( CPACK_RPM_PACKAGE_RELEASE ${CPACK_PACKAGE_RELEASE} )
    set( CPACK_RPM_PACKAGE_ARCHITECTURE ${CPACK_PACKAGE_ARCHITECTURE} )
    set( CPACK_RPM_PACKAGE_DESCRIPTION ${CPACK_PACKAGE_DESCRIPTION_SUMMARY} )
    set( CPACK_RPM_PACKAGE_URL ${CPACK_PACKAGE_HOMEPAGE_URL} )
    set( CPACK_RPM_PACKAGE_LICENSE "APACHE-2" )
    set( CPACK_RPM_COMPONENT_INSTALL 1 )
    set( CPACK_RPM_MAIN_COMPONENT "Library" )
    set( CPACK_RPM_COMPRESSION_TYPE "xz" )
    set( CPACK_RPM_PACKAGE_AUTOPROV 1 )

    set( CPACK_RPM_Library_PACKAGE_ARCHITECTURE ${CPACK_PACKAGE_ARCHITECTURE} )
    set( CPACK_RPM_Library_PACKAGE_NAME ${CPACK_PACKAGE_NAME} )
    set( CPACK_RPM_Library_FILE_NAME
         "${CPACK_RPM_Library_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_RPM_PACKAGE_RELEASE}.${CPACK_RPM_Library_PACKAGE_ARCHITECTURE}.rpm" )
    set( CPACK_RPM_Library_PACKAGE_SUMMARY ${CPACK_COMPONENT_Library_DESCRIPTION} )

    set( CPACK_RPM_Devel_PACKAGE_REQUIRES "cmake >= ${CMAKE_MINIMUM_REQUIRED_VERSION},QtExtra >= ${CPACK_PACKAGE_VERSION}" )
    set( CPACK_RPM_Devel_PACKAGE_SUMMARY ${CPACK_COMPONENT_Devel_DESCRIPTION} )
    set( CPACK_RPM_Devel_PACKAGE_ARCHITECTURE "noarch" )  # only contains headers and cmake files
    set( CPACK_RPM_Devel_PACKAGE_NAME "${CPACK_PACKAGE_NAME}-devel" )
    set( CPACK_RPM_Devel_FILE_NAME
         "${CPACK_RPM_Devel_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_RPM_PACKAGE_RELEASE}.${CPACK_RPM_Devel_PACKAGE_ARCHITECTURE}.rpm" )


    # DEB - https://cmake.org/cmake/help/latest/cpack_gen/deb.html
    set( CPACK_DEBIAN_PACKAGE_RELEASE ${CPACK_PACKAGE_RELEASE} )
    set( CPACK_DEBIAN_PACKAGE_HOMEPAGE ${CPACK_PACKAGE_HOMEPAGE_URL} )
    set( CPACK_DEB_COMPONENT_INSTALL 1 )
    set( CPACK_DEBIAN_COMPRESSION_TYPE "xz")

    if(CPACK_PACKAGE_ARCHITECTURE STREQUAL "x86_64")
        set( CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64" )  # DEB doesn't always use the kernel's arch name
    else()
        set( CPACK_DEBIAN_PACKAGE_ARCHITECTURE ${CPACK_PACKAGE_ARCHITECTURE} )
    endif()

    set( CPACK_DEBIAN_FILE_NAME "DEB-DEFAULT" ) # Use default naming scheme

    set( CPACK_DEBIAN_LIBRARY_PACKAGE_NAME ${CPACK_PACKAGE_NAME} )
    set( CPACK_DEBIAN_LIBRARY_PACKAGE_SHLIBDEPS 1 )

    set( CPACK_DEBIAN_DEVEL_PACKAGE_DEPENDS "cmake (>= ${CMAKE_MINIMUM_REQUIRED_VERSION}), QtExtra (>= ${CPACK_PACKAGE_VERSION})" )
    set( CPACK_DEBIAN_DEVEL_PACKAGE_NAME "${CPACK_PACKAGE_NAME}-dev" )

elseif(CMAKE_HOST_WIN32)
    set( CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON )
    set( CPACK_NSIS_DISPLAY_NAME ${PROJECT_NAME} )
    set( CPACK_NSIS_PACKAGE_NAME ${PROJECT_NAME} )
    set( CPACK_NSIS_URL_INFO_ABOUT ${CPACK_PACKAGE_HOMEPAGE_URL} )
endif()

# This must always be last!
include( CPack )


# # Create a proper FindQtExtra - module
# set(CMAKECONFIG_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/QtExtra")
# install(EXPORT qtextra_targets
#     DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
#     NAMESPACE QtExtra::
#     FILE QtExtraTargets.cmake
#     COMPONENT Devel
# )

# # Include CMakePackageConfigHelpers before using its commands
# include(CMakePackageConfigHelpers)

# configure_package_config_file(
#     "${QTEXTRA_MODULE_DIR}/QtExtraConfig.cmake.in"
#     "${PROJECT_BINARY_DIR}/QtExtraConfig.cmake"
#     INSTALL_DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
# )
# write_basic_package_version_file("${PROJECT_BINARY_DIR}/QtExtraConfigVersion.cmake"
#     VERSION ${PROJECT_VERSION}
#     COMPATIBILITY AnyNewerVersion
# )
#if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
#    set(DEBUG_SUFFIX)
#    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
#        set(DEBUG_SUFFIX "d")
#    endif ()
#    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
#    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
#        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
#        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
#            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
#        endif ()
#    endif ()
#    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
#        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#                COMMAND ${CMAKE_COMMAND} -E make_directory
#                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
#        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#                COMMAND ${CMAKE_COMMAND} -E copy
#                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
#                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
#    endif ()
#    foreach (QT_LIB Core Gui Widgets)
#        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#                COMMAND ${CMAKE_COMMAND} -E copy
#                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
#                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
#    endforeach (QT_LIB)
#endif ()
