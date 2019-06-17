cmake_minimum_required(VERSION 3.1.0)

project(Amigurumi)

# require std 14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set cmake find_package directory
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

# help find qt
set (CMAKE_PREFIX_PATH "C:\\Qt\\5.12.2\\msvc2017_64")

# Find the QtWidgets library
find_package(Qt5 COMPONENTS Core Widgets Xml REQUIRED)

# Find GLM library
set(GLM_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}/thirdparty/glm")
find_package(GLM REQUIRED)


# Populate a CMake variable with the sources
set(SOURCES
	ami/Amigurumi.cpp
	ami/EditorViewWidget.cpp
	ami/LibraryLoader.cpp
	ami/LibraryViewWidget.cpp
	ami/main.cpp
	ami/mainwindow.cpp
	ami/Operation.cpp
	ami/PatternEditorItem.cpp
	ami/PatternEditorWidget.cpp
	ami/PatternGraph.cpp
)

set(HEADERS
	ami/Amigurumi.h
	ami/mainwindow.h
	ami/EditorViewWidget.h
	ami/LibraryLoader.h
	ami/LibraryViewWidget.h
	ami/Operation.h
	ami/PatternEditorItem.h
	ami/PatternEditorWidget.h
	ami/PatternGraph.h
	ami/Pattern.h
)

set(UIS
	ami/EditorViewWidget.ui
	ami/LibraryViewWidget.ui
	ami/MainWindow.ui
)

set(INCLUDES 
	.
)

set(RESOURCES
	ami/application.qrc
)

qt5_wrap_ui( QT_UIS ${UIS} )
qt5_add_resources(QT_RESOURCES ${RESOURCES})

# too remove console add WIN32 to add_executable
add_executable(amigurumi ${SOURCES} ${HEADERS} ${QT_RESOURCES} ${QT_UIS})
target_include_directories(amigurumi PUBLIC ${INCLUDES} ${GLM_INCLUDE_DIRS})
target_link_libraries(amigurumi Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Xml)

set (QtDLLs $<TARGET_FILE:Qt5::Widgets> $<TARGET_FILE:Qt5::Core> $<TARGET_FILE:Qt5::Gui> $<TARGET_FILE:Qt5::Xml>)

add_custom_command(
    TARGET amigurumi POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
		${QtDLLs}
        $<TARGET_FILE_DIR:amigurumi>
)
