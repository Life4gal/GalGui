
file(
		GLOB_RECURSE
		INTERNAL_HEADERS
		CONFIGURE_DEPENDS
		"${CMAKE_CURRENT_SOURCE_DIR}/include/internal/*.hpp"
)

#file(
#		GLOB_RECURSE
#		INTERNAL_SOURCES
#		CONFIGURE_DEPENDS
#		"${CMAKE_CURRENT_SOURCE_DIR}/source/internal/*.cpp"
#)

add_library(
		galGui
#		PRIVATE ${INTERNAL_SOURCES}
		${INTERNAL_HEADERS}
)

target_sources(
		galGui
#		PRIVATE ${INTERNAL_SOURCES}
		PUBLIC ${INTERNAL_HEADERS}
		)

target_include_directories(
		galGui PUBLIC
		include
		galLibrary
)

target_compile_features(
		galGui PUBLIC
		cxx_std_17
)

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_compile_options(
		galGui PRIVATE
		$<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic -ftemplate-backtrace-limit=0>
		$<$<CXX_COMPILER_ID:Clang>:-Wall -Wpedantic -ftemplate-backtrace-limit=0>
		$<$<CXX_COMPILER_ID:AppleClang>:-Wall -Wpedantic -ftemplate-backtrace-limit=0>
		$<$<CXX_COMPILER_ID:MSVC>:/W4 /wd4244 /wd4305 /wd4996 /wd4267 /wd4018 /wd5030 /wd4201 /utf-8>
)
