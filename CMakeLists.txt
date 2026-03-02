cmake_minimum_required(VERSION 3.19)
project(ToDoList LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Sql)

qt_standard_project_setup()

qt_add_executable(ToDoList
    WIN32 MACOSX_BUNDLE
    src/cpp/main.cpp
    src/cpp/mainwindow.cpp
    src/cpp/addtaskdialog.cpp
    src/cpp/databasemanager.cpp
    src/cpp/tasksqlmodel.cpp
    src/cpp/finddialog.cpp

    src/headers/mainwindow.h
    src/headers/addtaskdialog.h
    src/headers/databasemanager.h
    src/headers/tasksqlmodel.h
    src/headers/finddialog.h

    src/forms/mainwindow.ui
    src/forms/addtaskdialog.ui
    src/forms/finddialog.ui
)

target_include_directories(ToDoList PRIVATE src/headers)

target_link_libraries(ToDoList
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Sql
)

include(GNUInstallDirs)

install(TARGETS ToDoList
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET ToDoList
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
