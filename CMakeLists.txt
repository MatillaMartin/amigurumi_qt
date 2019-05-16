cmake_minimum_required(VERSION 3.1.0)

project(Amigurumi)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

# help find qt
set (CMAKE_PREFIX_PATH "C:\\Qt\\5.12.2\\msvc2017_64")

# Find the QtWidgets library
find_package(Qt5Widgets CONFIG REQUIRED)

# Populate a CMake variable with the sources
set(SOURCES
	ami/main.cpp
	ami/mainwindow.cpp
	ami/LibraryViewWidget.cpp
	ami/EditorViewWidget.cpp
	ami/PatternEditorWidget.cpp
	ami/PatternGraph.cpp
)

set(HEADERS
	ami/mainwindow.h
	ami/LibraryViewWidget.h
	ami/EditorViewWidget.h
	ami/PatternEditorWidget.h
	ami/PatternGraph.h
	ami/Operation.h
	ami/Pattern.h
)

set(UIS
	ami/MainWindow.ui
	ami/LibraryViewWidget.ui
	ami/EditorViewWidget.ui
	ami/PatternEditorWidget.ui
)

set(INCLUDES 
	ami
)

set(RESOURCES
	ami/application.qrc
)

qt5_wrap_ui( QT_UIS ${UIS} )
qt5_add_resources(QT_RESOURCES ${RESOURCES})

add_executable(amigurumi WIN32 ${SOURCES} ${HEADERS} ${QT_RESOURCES} ${QT_UIS})
include_directories(amigurumi ${INCLUDES})
target_link_libraries(amigurumi Qt5::Widgets)

set (QtDLLs $<TARGET_FILE:Qt5::Widgets> $<TARGET_FILE:Qt5::Core> $<TARGET_FILE:Qt5::Gui>)

add_custom_command(
    TARGET amigurumi POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
		${QtDLLs}
        $<TARGET_FILE_DIR:amigurumi>
)