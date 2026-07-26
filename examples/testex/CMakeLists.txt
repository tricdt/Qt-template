project(TestEx LANGUAGES CXX)
set(SOURCES main.cpp widget.cpp widget.h
    mainwindow.h mainwindow.cpp
    examplepage/T_BasePage.h examplepage/T_BasePage.cpp
    examplepage/T_BaseComponents.h examplepage/T_BaseComponents.cpp
)
set(QRC_SOURCES testex.qrc)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_executable(${PROJECT_NAME} WIN32 ${SOURCES} ${QRC_SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE
    QtExtra::QtTestLib
    QtExtra::QtExtraAux 
    QtExtra::QtGeometry
    QtExtra::QtWidgetsExtra
)

if(WIN32)
    get_filename_component(QT_INSTALL_PATH "${Qt6Core_DIR}/../../.." ABSOLUTE)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/platforms"
        VERBATIM
    )

    if(EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindowsd.dll")
        set(PLATFORM_DLL_NAME "qwindows$<IF:$<CONFIG:Debug>,d,>.dll")
    else()
        set(PLATFORM_DLL_NAME "qwindows.dll")
    endif()

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${QT_INSTALL_PATH}/plugins/platforms/${PLATFORM_DLL_NAME}"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/platforms/"
        VERBATIM
    )

    foreach(QT_LIB Core Gui Widgets)
        if(EXISTS "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}d.dll")
            set(LIB_DLL_NAME "Qt6${QT_LIB}$<IF:$<CONFIG:Debug>,d,>.dll")
        else()
            set(LIB_DLL_NAME "Qt6${QT_LIB}.dll")
        endif()

        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${QT_INSTALL_PATH}/bin/${LIB_DLL_NAME}"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
            VERBATIM
        )
    endforeach()
endif()