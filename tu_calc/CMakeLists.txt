# Required minimum cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET (OUTPUT_ELF "tu_calc")

SET (SRC_FILES
	tu_calc.c
	)

INCLUDE_DIRECTORIES (
				.
				${GLIB2_INCLUDE_DIRS}
				)

link_libraries(${GLIB2_LIBRARIES} m)

ADD_LIBRARY(${OUTPUT_ELF} SHARED ${SRC_FILES})

set(TUCALC_MAJOR_VERSION "0")
set(TUCALC_MINOR_VERSION "8")
set(TUCALC_PATCH_VERSION "0")

set_target_properties (${OUTPUT_ELF} PROPERTIES
						VERSION "${TUCALC_MAJOR_VERSION}.${TUCALC_MINOR_VERSION}.${TUCALC_PATCH_VERSION}"
						SOVERSION "${TUCALC_MAJOR_VERSION}")

install(TARGETS ${OUTPUT_ELF}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/tu_calc.h
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})