project(galGuiTest)

if (UNIX AND NOT APPLE)
	find_package(OpenGL REQUIRED COMPONENTS OpenGL)
	find_package(PkgConfig REQUIRED)
	pkg_check_modules(gtk3 REQUIRED IMPORTED_TARGET gtk+-3.0)
endif()

add_executable(
		${PROJECT_NAME}
		main.cpp
)

target_link_libraries(
		${PROJECT_NAME}
		PkgConfig::gtk3
		OpenGL::GL
)

target_link_libraries(
		${PROJECT_NAME}
		galGui
)
