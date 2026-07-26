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
FILE(GLOB COMMANDS command/*.h command/*.cpp)

add_library(${PROJECT_NAME}
    ${QTWIDGETSEXTRA_LIB_TYPE}
    ${ORIGIN}
    ${WIDGETS}
    ${VIEWS}
    ${MODELS}
    ${DELEGATES}
    ${STYLES}
    ${UTILS}
    ${COMMANDS}
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
    widgets/qtskeleton.h widgets/qtskeleton.cpp
    widgets/qtdivider.h widgets/qtdivider.cpp
    widgets/qttag.h widgets/qttag.cpp
    widgets/qtsplitbutton.h widgets/qtsplitbutton.cpp
    widgets/qtpersonpicture.h widgets/qtpersonpicture.cpp
    widgets/qtpagination.h widgets/qtpagination.cpp
    widgets/qtflyout.h widgets/qtflyout.cpp
    widgets/qtsteps.h widgets/qtsteps.cpp
    widgets/qttimeline.h widgets/qttimeline.cpp
    widgets/qtcommandbar.h widgets/qtcommandbar.cpp
    widgets/qtratingcontrol.h widgets/qtratingcontrol.cpp
    widgets/qtnumberbox.h widgets/qtnumberbox.cpp
    widgets/qtwizard.h widgets/qtwizard.cpp
    widgets/qtnotificationcenter.h widgets/qtnotificationcenter.cpp
    widgets/qtmarkdownviewer.h widgets/qtmarkdownviewer.cpp
    widgets/qtcaptcha.h widgets/qtcaptcha.cpp
    widgets/qtdropdownbutton.h widgets/qtdropdownbutton.cpp
    widgets/qtselectorbar.h widgets/qtselectorbar.cpp
    widgets/qtvirtuallist.h widgets/qtvirtuallist.cpp
    widgets/qtsplashscreen.h widgets/qtsplashscreen.cpp
    widgets/qtsheetpanel.h widgets/qtsheetpanel.cpp
    widgets/qttransfer.h widgets/qttransfer.cpp
    widgets/qtspotlight.h widgets/qtspotlight.cpp
    widgets/qtcountdown.h widgets/qtcountdown.cpp
    widgets/qtpopconfirm.h widgets/qtpopconfirm.cpp
    widgets/qtwatermark.h widgets/qtwatermark.cpp
    widgets/qtsplitter.h widgets/qtsplitter.cpp
    widgets/qtqrcode.h widgets/qtqrcode.cpp
    widgets/qtfloatbutton.h widgets/qtfloatbutton.cpp
    widgets/qtemojipicker.h widgets/qtemojipicker.cpp
    widgets/qtinfobar.h widgets/qtinfobar.cpp
    widgets/qtautocomplete.h widgets/qtautocomplete.cpp
    widgets/qttreeselect.h widgets/qttreeselect.cpp
    widgets/qtuploadarea.h widgets/qtuploadarea.cpp
    widgets/qtcopybutton.h widgets/qtcopybutton.cpp

    widgets/qtpasswordbox.h widgets/qtpasswordbox.cpp
    widgets/qtteachingtip.h widgets/qtteachingtip.cpp
    widgets/qtinfobadge.h widgets/qtinfobadge.cpp
    widgets/qttablewidget.h widgets/qttablewidget.cpp
    widgets/qtgroupbox.h widgets/qtgroupbox.cpp
    widgets/qtroller.h widgets/qtroller.cpp
    widgets/qtrollerpicker.h widgets/qtrollerpicker.cpp
    widgets/qtrollerpickercontainer.h widgets/qtrollerpickercontainer.cpp
    widgets/qtmessagedialog.h widgets/qtmessagedialog.cpp
    widgets/qtdrawerarea.h widgets/qtdrawerarea.cpp
    widgets/qtdrawercontainer.h widgets/qtdrawercontainer.cpp
    widgets/qtdrawerheader.h widgets/qtdrawerheader.cpp
    widgets/qtcontentdialog.h widgets/qtcontentdialog.cpp
    widgets/qtkeybinder.h widgets/qtkeybinder.cpp
    widgets/qtkeybindercontainer.h widgets/qtkeybindercontainer.cpp
    widgets/qtdoublespinbox.h widgets/qtdoublespinbox.cpp
    widgets/qtmaskwidget.h widgets/qtmaskwidget.cpp
    widgets/qtinputdialog.h widgets/qtinputdialog.cpp
    widgets/qtcalendar.h widgets/qtcalendar.cpp
    widgets/qtcalendarpicker.h widgets/qtcalendarpicker.cpp
    widgets/qtcalendarpickercontainer.h widgets/qtcalendarpickercontainer.cpp
    widgets/qtlineedit.h widgets/qtlineedit.cpp
    widgets/qtcolorpreview.h widgets/qtcolorpreview.cpp
    widgets/qtcolorpicker.h widgets/qtcolorpicker.cpp
    widgets/qtcolordialog.h widgets/qtcolordialog.cpp
    widgets/qtlcdnumber.h widgets/qtlcdnumber.cpp
    widgets/qtpromotioncard.h widgets/qtpromotioncard.cpp
    widgets/qtpromotionview.h widgets/qtpromotionview.cpp
    widgets/qttreeview.h widgets/qttreeview.cpp
    widgets/qttableview.h widgets/qttableview.cpp
    widgets/qtlistview.h widgets/qtlistview.cpp
    widgets/qtdialog.h widgets/qtdialog.cpp
    widgets/qtpivot.h widgets/qtpivot.cpp
    widgets/qtpushbutton.h widgets/qtpushbutton.cpp
    widgets/qttooltip.h widgets/qttooltip.cpp
    widgets/qtscrollarea.h widgets/qtscrollarea.cpp
    widgets/qtbreadcrumbbar.h widgets/qtbreadcrumbbar.cpp
    widgets/qtcustomtabwidget.h widgets/qtcustomtabwidget.cpp
    widgets/qtcustomwidget.h widgets/qtcustomwidget.cpp
    widgets/qttabbar.h widgets/qttabbar.cpp
    widgets/qttabwidget.h widgets/qttabwidget_p.h widgets/qttabwidget.cpp
    itemviews/views/qtpivotview.h itemviews/views/qtpivotview.cpp
    itemviews/views/qtnavigationview.h itemviews/views/qtnavigationview.cpp
    itemviews/models/qtnavigationmodel.h itemviews/models/qtnavigationmodel.cpp
    itemviews/models/qtfootermodel.h itemviews/models/qtfootermodel.cpp
    itemviews/models/qtbreadcrumbbarmodel.h itemviews/models/qtbreadcrumbbarmodel.cpp
    itemviews/models/qtpivotmodel.h itemviews/models/qtpivotmodel.cpp
    itemviews/models/qtcolordisplaymodel.h itemviews/models/qtcolordisplaymodel.cpp
    itemviews/models/qtcalendarmodel.h itemviews/models/qtcalendarmodel.cpp
    itemviews/models/qtcalendartitlemodel.h itemviews/models/qtcalendartitlemodel.cpp
    itemviews/delegates/qtfooterdelegate.h itemviews/delegates/qtfooterdelegate.cpp
    itemviews/delegates/qtbreadcrumbbardelegate.h itemviews/delegates/qtbreadcrumbbardelegate.cpp
    itemviews/delegates/qtcolordisplaydelegate.h itemviews/delegates/qtcolordisplaydelegate.cpp
    itemviews/delegates/qtcalendartitledelegate.h itemviews/delegates/qtcalendartitledelegate.cpp
    itemviews/delegates/qtcalendardelegate.h itemviews/delegates/qtcalendardelegate.cpp
    styles/qtnavigationstyle.h styles/qtnavigationstyle.cpp
    styles/qttabbarstyle.h styles/qttabbarstyle.cpp
    styles/qtpivotstyle.h styles/qtpivotstyle.cpp
    styles/qtlistviewstyle.h styles/qtlistviewstyle.cpp
    styles/qttableviewstyle.h styles/qttableviewstyle.cpp
    styles/qttreeviewstyle.h styles/qttreeviewstyle.cpp
    styles/qtlcdnumberstyle.h styles/qtlcdnumberstyle.cpp
    styles/qtcolorvaluesliderstyle.h styles/qtcolorvaluesliderstyle.cpp
    styles/qtlineeditstyle.h styles/qtlineeditstyle.cpp
    styles/qtgroupboxstyle.h styles/qtgroupboxstyle.cpp
    styles/qttablewidgetstyle.h styles/qttablewidgetstyle.cpp
    command/qtnavigationroutecommand.h command/qtnavigationroutecommand.cpp
    utils/qtintvalidator.h utils/qtintvalidator.cpp
    utils/qtkeybinderfnmonitor.h utils/qtkeybinderfnmonitor.mm
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

