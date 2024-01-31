# Required minimum cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET (OUTPUT_ELF "tu_jp_factory")

SET (SRC_FILES
	factory/dlinkedlist/TU_JP_D_LinkedList.c
	factory/fl/TU_JP_Console.c
	factory/fl/TU_JP_crc16.c
	factory/fl/TU_JP_crc32.c
	factory/fl/TU_JP_Timer.c
	factory/fl/TU_JP_Message.c
#	factory/std/TU_JP_BitStream.c
	factory/std/TU_JP_Queue.c
	factory/std/TU_JP_Semaphore.c
	factory/std/TU_JP_Signal.c
	factory/std/TU_JP_Time.c
	factory/std/TU_JP_ArithmeticOperation.c
	factory/std/TU_JP_TypeCast.c
   )

INCLUDE_DIRECTORIES (
				./include
				./factory/dlinkedlist
				./factory/dlinkedtree
				./factory/fl
				./factory/std
				${GLIB2_INCLUDE_DIRS}
				)

ADD_LIBRARY(${OUTPUT_ELF} SHARED ${SRC_FILES})

set(TU_FACTORY_MAJOR_VERSION "0")
set(TU_FACTORY_MINOR_VERSION "0")
set(TU_FACTORY_PATCH_VERSION "0")

set_target_properties (${OUTPUT_ELF} PROPERTIES
						VERSION "${TU_FACTORY_MAJOR_VERSION}.${TU_FACTORY_MINOR_VERSION}.${TU_FACTORY_PATCH_VERSION}"
						SOVERSION "${TU_FACTORY_MAJOR_VERSION}")

target_link_libraries(${OUTPUT_ELF} ${LIB} glib-2.0)

install(TARGETS ${OUTPUT_ELF}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

INSTALL(FILES
			${CMAKE_CURRENT_SOURCE_DIR}/include/TU_JP_CommonSt.h
			${CMAKE_CURRENT_SOURCE_DIR}/factory/dlinkedlist/TU_JP_Lib_C_D_LinkedList.h
			${CMAKE_CURRENT_SOURCE_DIR}/factory/dlinkedlist/TU_JP_D_LinkedList.h
			${CMAKE_CURRENT_SOURCE_DIR}/factory/dlinkedtree/TU_JP_Lib_C_D_LinkedTree.h
			${CMAKE_CURRENT_SOURCE_DIR}/factory/fl/TU_JP_Lib_C_FL.h
			${CMAKE_CURRENT_SOURCE_DIR}/factory/fl/TU_JP_crc32.h
			${CMAKE_CURRENT_SOURCE_DIR}/factory/fl/TU_JP_Timer.h
			${CMAKE_CURRENT_SOURCE_DIR}/factory/fl/TU_JP_Message.h
			${CMAKE_CURRENT_SOURCE_DIR}/factory/fl/TU_JP_Console.h
			${CMAKE_CURRENT_SOURCE_DIR}/factory/fl/TU_JP_crc16.h
			${CMAKE_CURRENT_SOURCE_DIR}/factory/fl/TU_JP_String.h
			${CMAKE_CURRENT_SOURCE_DIR}/factory/std/TU_JP_Lib_C_Std.h
			${CMAKE_CURRENT_SOURCE_DIR}/factory/std/TU_JP_TypeCast.h
			${CMAKE_CURRENT_SOURCE_DIR}/factory/std/TU_JP_ArithmeticOperation.h
			${CMAKE_CURRENT_SOURCE_DIR}/factory/std/TU_JP_BitStream.h
			${CMAKE_CURRENT_SOURCE_DIR}/factory/std/TU_JP_Semaphore.h
			${CMAKE_CURRENT_SOURCE_DIR}/factory/std/TU_JP_Queue.h
			${CMAKE_CURRENT_SOURCE_DIR}/factory/std/TU_JP_Signal.h
			${CMAKE_CURRENT_SOURCE_DIR}/factory/std/TU_JP_Time.h
		DESTINATION
			${CMAKE_INSTALL_INCLUDEDIR})