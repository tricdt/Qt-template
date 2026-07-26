# ==============================================================================
# QtExtraConfigMacros.cmake - Modern CMake Build Configuration & Flag Management
# ==============================================================================

# ------------------------------------------------------------------------------
# 1. BUILD OPTIONS SETUP
# ------------------------------------------------------------------------------
macro(qtextra_setup_options)
    # Global build options
    option(BUILD_SHARED_LIBS "Build all libraries as Dynamic/Shared (DLL) by default" ON)
    option(BUILD_TOOLS "Build additional helper tools" ${PROJECT_IS_TOP_LEVEL})
    option(BUILD_EXAMPLES "Build example applications" ${PROJECT_IS_TOP_LEVEL})
    option(BUILD_PEDANTIC "Enable strict/pedantic compiler warnings" OFF)

    set(QTEXTRA_AVAILABLE_MODULES
        QTEXTRAAUX
        QTGEOMETRY
        QTCOREEXTRA
        QTPLUGINSEXTRA
        QTPROPERTYBROWSER
        QTWIDGETSEXTRA
        QTSQLEXTRA
        QTSQLWIDGETS
        QTLAYOUTSEXTRA
        QTTEXTEXTRA
        QTSPELLCHECKING
        QTTESTLIB
        CACHE INTERNAL "List of all available QtExtra sub-modules"
    )

    set(QTEXTRA_AVAILABLE_EXAMPLES
        DEMO_CORE
        DEMO_WIDGETS
        DEMO_SQL
        CACHE INTERNAL "List of available example apps"
    )

    set(BUILD_LIBS "ALL" CACHE STRING "List of sub-libraries to build (ALL, NONE, or custom list)")
    set(BUILD_EXAMPLES_LIST "ALL" CACHE STRING "List of examples to build (ALL, NONE, or list)")

    set_property(CACHE BUILD_LIBS PROPERTY STRINGS "ALL" "NONE" ${QTEXTRA_AVAILABLE_MODULES})
    set_property(CACHE BUILD_EXAMPLES_LIST PROPERTY STRINGS "ALL" "NONE" ${QTEXTRA_AVAILABLE_EXAMPLES})

    set(_BUILD_LIBS_CHANGED FALSE)
    if(NOT DEFINED CACHE{_PREV_BUILD_LIBS} OR NOT "${BUILD_LIBS}" STREQUAL "${_PREV_BUILD_LIBS}")
        set(_BUILD_LIBS_CHANGED TRUE)
        set(_PREV_BUILD_LIBS "${BUILD_LIBS}" CACHE INTERNAL "Previous value of BUILD_LIBS")
    endif()

    set(_BUILD_EXAMPLES_CHANGED FALSE)
    if(NOT DEFINED CACHE{_PREV_BUILD_EXAMPLES_LIST} OR NOT "${BUILD_EXAMPLES_LIST}" STREQUAL "${_PREV_BUILD_EXAMPLES_LIST}")
        set(_BUILD_EXAMPLES_CHANGED TRUE)
        set(_PREV_BUILD_EXAMPLES_LIST "${BUILD_EXAMPLES_LIST}" CACHE INTERNAL "Previous value of BUILD_EXAMPLES_LIST")
    endif()

    foreach(_module IN LISTS QTEXTRA_AVAILABLE_MODULES)
        string(TOLOWER "${_module}" _module_lower)
        set(_default_build_state ON)
        if(BUILD_LIBS STREQUAL "NONE")
            set(_default_build_state OFF)
        elseif(NOT BUILD_LIBS STREQUAL "ALL" AND NOT _module IN_LIST BUILD_LIBS)
            set(_default_build_state OFF)
        endif()

        if(_BUILD_LIBS_CHANGED)
            set(BUILD_${_module} ${_default_build_state} CACHE BOOL "Build the ${_module_lower} library" FORCE)
        else()
            option(BUILD_${_module} "Build the ${_module_lower} library" ${_default_build_state})
        endif()
    endforeach()

    foreach(_example IN LISTS QTEXTRA_AVAILABLE_EXAMPLES)
        string(TOLOWER "${_example}" _example_lower)

        set(_default_ex_state ON)
        if(NOT BUILD_EXAMPLES OR BUILD_EXAMPLES_LIST STREQUAL "NONE")
            set(_default_ex_state OFF)
        elseif(NOT BUILD_EXAMPLES_LIST STREQUAL "ALL" AND NOT _example IN_LIST BUILD_EXAMPLES_LIST)
            set(_default_ex_state OFF)
        endif()

        # CHỈ FORCE GHI ĐÈ KHI DROPDOWN EXAMPLES THAY ĐỔI HOẶC TẮT CỜ TỔNG
        if(_BUILD_EXAMPLES_CHANGED OR NOT BUILD_EXAMPLES)
            set(BUILD_EXAMPLE_${_example} ${_default_ex_state} CACHE BOOL "Build example ${_example_lower}" FORCE)
        else()
            option(BUILD_EXAMPLE_${_example} "Build example ${_example_lower}" ${_default_ex_state})
        endif()
    endforeach()

endmacro()

# ------------------------------------------------------------------------------
# 2. TARGET BUILD FLAGS INITIALIZATION (MODERN TARGET-CENTRIC APPROACH)
# ------------------------------------------------------------------------------
macro(qtextra_initialize_build_flags)
    # Create an interface target that acts as a configuration package
    if(NOT TARGET qtextra_build_flags)
        add_library(qtextra_build_flags INTERFACE)
        add_library(QtExtra::BuildFlags ALIAS qtextra_build_flags)
    endif()

    # Enforce C++17 standard and hidden symbol visibility (best practice for dynamic libraries)
    target_compile_features(qtextra_build_flags INTERFACE cxx_std_17)

    set_target_properties(qtextra_build_flags PROPERTIES
        INTERFACE_CXX_EXTENSIONS OFF
        INTERFACE_C_VISIBILITY_PRESET hidden
        INTERFACE_CXX_VISIBILITY_PRESET hidden
        INTERFACE_VISIBILITY_INLINES_HIDDEN ON
    )

    # Disable Qt keywords to prevent naming conflicts with signals/slots
    target_compile_definitions(qtextra_build_flags INTERFACE
        QT_NO_KEYWORDS
    )

    # Set basic warning flags according to the compiler (using Generator Expressions)
    target_compile_options(qtextra_build_flags INTERFACE
        # MSVC specific flags
        $<$<CXX_COMPILER_ID:MSVC>:
        /MP # Multi-processor compilation
        /W4 # Warning level 4
        >
        # GCC and Clang specific flags
        $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:
        -Wall
        -Wextra
        >
    )
endmacro()

# ------------------------------------------------------------------------------
# 3. LOAD LOCAL ENVIRONMENT OVERRIDES
# ------------------------------------------------------------------------------
macro(qtextra_load_optional_build_flags)
    if(DEFINED ENV{QTEXTRA_LOCAL_CMAKE_RULES})
        file(TO_CMAKE_PATH "$ENV{QTEXTRA_LOCAL_CMAKE_RULES}" LOCAL_RULES)
        if(EXISTS "${LOCAL_RULES}")
            message(STATUS "[QtExtra] Loading local build rules from: ${LOCAL_RULES}")
            include("${LOCAL_RULES}")
        else()
            message(WARNING "[QtExtra] File specified in QTEXTRA_LOCAL_CMAKE_RULES not found: ${LOCAL_RULES}")
        endif()
    endif()
endmacro()

# ------------------------------------------------------------------------------
# 4. ENABLE STRICT/PEDANTIC COMPILER FLAGS
# ------------------------------------------------------------------------------
macro(qtextra_enable_pedantic_flags)
    if(NOT TARGET qtextra_build_flags)
        message(FATAL_ERROR "[QtExtra] Call qtextra_initialize_build_flags before enabling pedantic flags!")
    endif()

    if(QT_VERSION_MAJOR EQUAL 5)
        target_compile_definitions(qtextra_build_flags INTERFACE QT_STRICT_ITERATORS)
    endif()

    # Pedantic flags common to both GCC and Clang
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options(-Wno-cast-function-type)
        target_compile_options(qtextra_build_flags INTERFACE
            -pedantic
            -pedantic-errors
            -Wformat
            -Werror=format-security
            -Wsuggest-override
            -Wextra-semi
            -Winline
            -Wmissing-declarations
            -Wredundant-decls
            -Wnon-virtual-dtor
            -Woverloaded-virtual
        )
    endif()

    # GCC-specific flags
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(qtextra_build_flags INTERFACE
            -Wsuggest-attribute=pure
            -Wsuggest-attribute=const
            -Wsuggest-attribute=noreturn
            -Wsuggest-attribute=malloc
            -Wduplicated-branches
            -Wduplicated-cond
            -Wshadow=local
            -Wlogical-op
            -Wclass-memaccess
        )
        # Clang-specific flags
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_compile_options(qtextra_build_flags INTERFACE
            -Weverything
            -Wno-c++98-compat
            -Wno-c++98-compat-pedantic
            -Wno-global-constructors
            -Wno-deprecated-copy-with-user-provided-dtor
            -Wno-signed-enum-bitfield
            -Wno-padded
            -Wno-gnu-zero-variadic-macro-arguments
        )
    endif()

endmacro()


# ------------------------------------------------------------------------------
# 5. FINALIZE BUILD FLAGS (OPTIMIZATION & SANITIZERS)
# ------------------------------------------------------------------------------
macro(qtextra_finalize_build_flags)
    message(STATUS "[QtExtra] Build Type: ${CMAKE_BUILD_TYPE}")

    # Define Debug macros
    target_compile_definitions(qtextra_build_flags INTERFACE
        $<$<CONFIG:Debug>:ITEM_STATISTICS=1>
    )

    # Optimization flags (-O0 for Debug, -O3 for Release/RelWithDebInfo)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(qtextra_build_flags INTERFACE
            $<$<CONFIG:Debug>:-O0>
            $<$<NOT:$<CONFIG:Debug>>:-O3>
            -ffast-math
        )
    endif()

    # Include ECM Sanitizers if enabled
    if(ECM_ENABLE_SANITIZERS)
        include(ECMEnableSanitizers)
    endif()
endmacro()

# ------------------------------------------------------------------------------
# 6. MAIN ORCHESTRATOR MACRO
# ------------------------------------------------------------------------------
macro(qtextra_setup_build)
    # We need C++20 and don't want to export all symbols, only the ones we mark for export
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
    set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
    set(CMAKE_CXX_VISIBILITY_PRESET hidden)

    # Enable automatic handling of Qt MOC, UIC, and RCC
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTORCC ON)

    # Min. Qt version is 6.5
    set(QT_MIN_VERSION 6.5.0)

    if(NOT Qt6_FOUND)
        find_package(Qt6 COMPONENTS Core Widgets REQUIRED)
    endif()

    message(STATUS "Qt version is ${Qt6Core_VERSION}")
    if(Qt6Core_VERSION VERSION_LESS ${QT_MIN_VERSION})
        message(FATAL_ERROR "Minimum supported Qt version is ${QT_MIN_VERSION}")
    endif()
    set(QT_VERSION_MAJOR 6)

    set(QTEXTRA_QT_DEFINITIONS
        QT_USE_QSTRINGBUILDER
        QT_NO_CAST_TO_ASCII
        #QT_NO_CAST_FROM_ASCII
        QT_NO_URL_CAST_FROM_STRING
        QT_DEPRECATED_WARNINGS
        QT_DISABLE_DEPRECATED_BEFORE=0x060400
        UNICODE
        _UNICODE
    )
    if(NOT MSVC AND NOT MINGW)
        list(APPEND QTEXTRA_QT_DEFINITIONS QT_STRICT_ITERATORS)
    endif()

    set(QTEXTRA_VERSION_DEFINITIONS
        QTEXTRA_VERSION_STR="${PROJECT_VERSION}"
        QTEXTRA_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
        QTEXTRA_VERSION_MINOR=${PROJECT_VERSION_MINOR}
        QTEXTRA_VERSION_PATCH=${PROJECT_VERSION_PATCH}
    )


    # Compiler-specific compile options - will be applied to targets
    set(QTEXTRA_COMPILE_OPTIONS "")
    set(QTEXTRA_COMPILE_DEFINITIONS "")

    # We need gcc >= 10 or clang >= 10 or msvc >= 19.29 (VS 2019 16.11) for C++20
    if(MINGW)
        if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 10.0)
            message(FATAL_ERROR "MinGW GCC 10.0 or higher required for C++20!")
        endif()

        list(APPEND QTEXTRA_COMPILE_OPTIONS
            -Wall
            -Wextra
            -Wformat=2
            -Wa,-mbig-obj
        )

        if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 4.9.99)
            list(APPEND QTEXTRA_COMPILE_OPTIONS
                -Wsuggest-override
            )
        endif()

        list(APPEND QTEXTRA_COMPILE_DEFINITIONS
            UNICODE
            _UNICODE
            WIN32_LEAN_AND_MEAN
            _WIN32_WINNT=0x0601
        )
        set(CMAKE_DEBUG_POSTFIX _d)

    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 10.0)
            message(FATAL_ERROR "gcc 10.0 or higher required for C++20!")
        endif()
        list(APPEND QTEXTRA_COMPILE_OPTIONS
            -Wall
            -Wextra
        )
        if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 4.9.99)
            list(APPEND QTEXTRA_COMPILE_OPTIONS
                -Wsuggest-override
            )
        endif()

    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 10.0)
            message(FATAL_ERROR "llvm/clang 10.0 or higher required for C++20!")
        endif()
        list(APPEND QTEXTRA_COMPILE_OPTIONS
            -Wall
            -Wextra
        )

    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        if(MSVC_VERSION LESS 1929)
            message(FATAL_ERROR "msvc 19.29 (Visual Studio 2019 16.11) or higher required for C++20!")
        endif()
        # disable common warnings in Qt/stdlib
        # disable 4127: conditional expression is constant
        #         4512: assignment operator could not be generated
        #         4267: conversion from 'size_t' to 'type', possible loss of data
        list(APPEND QTEXTRA_COMPILE_OPTIONS
            /W4
            /wd4127
            /wd4512
            /wd4267
            /wd4913
            /w34265
            /we4265
            /permissive-
        )
        list(APPEND QTEXTRA_COMPILE_DEFINITIONS
            UNICODE
            NOMINMAX
        )
        set(CMAKE_DEBUG_POSTFIX _d)
    endif()


    # Manage build directory for autogenerated files
    set(CMAKE_GLOBAL_AUTOGEN_TARGET OFF)
    set(AUTOGEN_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/autogen")

    # Organize targets into IDE folders (Visual Studio / CLion / Xcode)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    # Initialize build flag targets and execute options setup sequence
    qtextra_initialize_build_flags()

    if(BUILD_PEDANTIC)
        message(STATUS "[QtExtra] Enabling pedantic compiler flags")
        qtextra_enable_pedantic_flags()
    endif()

    qtextra_load_optional_build_flags()
    qtextra_finalize_build_flags()
endmacro()
