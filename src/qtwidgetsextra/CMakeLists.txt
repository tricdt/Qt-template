set(LIB_MAJOR_VERSION "1")
set(LIB_MINOR_VERSION "0")
set(LIB_PATCH_VERSION "0")
set(LIB_VERSION_STRING "${LIB_MAJOR_VERSION}.${LIB_MINOR_VERSION}.${LIB_PATCH_VERSION}")
project(QtWidgetsExtra VERSION 1.0.0 LANGUAGES CXX)

# find_package(Qt6 COMPONENTS Core REQUIRED)
if(BUILD_SHARED_LIBS)
    set(QTWIDGETSEXTRA_LIB_TYPE SHARED)
else()
    set(QTWIDGETSEXTRA_LIB_TYPE STATIC)
endif()

FILE(GLOB ORIGIN *.h *.cpp *.qrc)
FILE(GLOB WIDGETS widgets/*.h widgets/*.cpp)
FILE(GLOB STYLES styles/*.h styles/*.cpp)
FILE(GLOB UTILS utils/*.h utils/*.cpp)
FILE(GLOB VIEWS itemviews/views/*.h itemviews/views/*.cpp)
FILE(GLOB MODELS itemviews/models/*.h itemviews/models/*.cpp)
FILE(GLOB DELEGATES itemviews/delegates/*.h itemviews/delegates/*.cpp)

add_library(${PROJECT_NAME}
    ${QTWIDGETSEXTRA_LIB_TYPE}
    ${ORIGIN}
    ${WIDGETS}
    ${VIEWS}
    ${MODELS}
    ${DELEGATES}
    ${STYLES}
    ${UTILS}
)
add_library(QtExtra::QtWidgetsExtra ALIAS ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "${PROJECT_NAME}"
    FOLDER "Modules"
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${QTEXTRA_BUILD_INCLUDE_DIR}>
    $<BUILD_INTERFACE:${NAMESPACE_HEADER_LOCATION}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    Qt6::Core Qt6::Widgets QtExtra::QtExtraAux QtExtra::QtGeometry
    PRIVATE
    $<BUILD_INTERFACE:QtExtra::BuildFlags>
)

target_sources(${PROJECT_NAME} PRIVATE
    command/qtwindowstackchangecommand.h command/qtwindowstackchangecommand.cpp
    widgets/qtscrollpage.h widgets/qtscrollpage.cpp
    widgets/qtwindow.h widgets/qtwindow.cpp widgets/qtwindow_p.h
    styles/qtwindowstyle.h styles/qtwindowstyle.cpp
    widgets/qtnavigationbar.h widgets/qtnavigationbar.cpp
    widgets/qtcentralstackedwidget.h widgets/qtcentralstackedwidget.cpp
    widgets/qtthemeanimationwidget.h widgets/qtthemeanimationwidget.cpp
    widgets/qtsuggestbox.h widgets/qtsuggestbox.cpp
    widgets/qtactioncommander.h widgets/qtactioncommander.cpp
    utils/qtnavigationnode.h utils/qtnavigationnode.cpp
)


# Apply compile definitions from parent
target_compile_definitions(${PROJECT_NAME}
    PRIVATE
    ${QTEXTRA_QT_DEFINITIONS}
    ${QTEXTRA_VERSION_DEFINITIONS}
    ${QTEXTRA_COMPILE_DEFINITIONS}
)

if(QTEXTRA_COMPILE_OPTIONS)
    target_compile_options(${PROJECT_NAME} PRIVATE ${QTEXTRA_COMPILE_OPTIONS})
endif()

if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(${PROJECT_NAME} PUBLIC QTWIDGETSEXTRA_STATIC)
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE QTWIDGETSEXTRA_LIBRARY)
endif()

# Install the library and export targets
install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAMESPACE}Targets
    COMPONENT Library
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# Install headers
install(DIRECTORY ${QTEXTRA_BUILD_INCLUDE_DIR}/${PROJECT_NAME}/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
    FILES_MATCHING REGEX "^[^.]+$"
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
    FILES_MATCHING PATTERN "*.h"
)

