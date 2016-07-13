
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT ( client )

set(CMAKE_BUILD_TYPE "debug")

# Settings for Visual Studio 7 and above.  
IF(MSVC)
    # replace /MDd with /MTd
    STRING(REPLACE "/MD"  "/MT"  CMAKE_C_FLAGS_RELEASE          ${CMAKE_C_FLAGS_RELEASE})
    STRING(REPLACE "/MD"  "/MT"  CMAKE_C_FLAGS_RELWITHDEBINFO   ${CMAKE_C_FLAGS_RELWITHDEBINFO})
    STRING(REPLACE "/MDd" "/MTd" CMAKE_C_FLAGS_DEBUG            ${CMAKE_C_FLAGS_DEBUG})
    STRING(REPLACE "/MDd" "/MTd" CMAKE_C_FLAGS_DEBUG_INIT       ${CMAKE_C_FLAGS_DEBUG_INIT})

    STRING(REPLACE "/MD"  "/MT"  CMAKE_CXX_FLAGS_RELEASE        ${CMAKE_CXX_FLAGS_RELEASE})
    STRING(REPLACE "/MD"  "/MT"  CMAKE_CXX_FLAGS_RELWITHDEBINFO ${CMAKE_CXX_FLAGS_RELWITHDEBINFO})
    STRING(REPLACE "/MDd" "/MTd" CMAKE_CXX_FLAGS_DEBUG          ${CMAKE_CXX_FLAGS_DEBUG})
    STRING(REPLACE "/MDd" "/MTd" CMAKE_CXX_FLAGS_DEBUG_INIT     ${CMAKE_CXX_FLAGS_DEBUG_INIT})

    # generate map files, set stack size (see bug#20815)
    SET(thread_stack_size 1048576)
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /STACK:${thread_stack_size}")
    ADD_DEFINITIONS(-DPTHREAD_STACK_MIN=${thread_stack_size})

    # remove support for Exception handling
   # STRING(REPLACE "/GX"   "" CMAKE_CXX_FLAGS            ${CMAKE_CXX_FLAGS})
   # STRING(REPLACE "/EHsc" "" CMAKE_CXX_FLAGS            ${CMAKE_CXX_FLAGS})
  #  STRING(REPLACE "/EHsc" "" CMAKE_CXX_FLAGS_INIT       ${CMAKE_CXX_FLAGS_INIT})
  #  STRING(REPLACE "/EHsc" "" CMAKE_CXX_FLAGS_DEBUG_INIT ${CMAKE_CXX_FLAGS_DEBUG_INIT})
    
    # Mark 32 bit executables large address aware so they can 
    # use > 2GB address space
    IF(CMAKE_SIZEOF_VOID_P MATCHES 4)
      SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LARGEADDRESSAWARE")
    ENDIF(CMAKE_SIZEOF_VOID_P MATCHES 4)
    
    # Disable automatic manifest generation.
    STRING(REPLACE "/MANIFEST" "/MANIFEST:NO" CMAKE_EXE_LINKER_FLAGS 
    	   ${CMAKE_EXE_LINKER_FLAGS})
    # Explicitly disable it since it is the default for newer versions of VS
    STRING(REGEX MATCH "MANIFEST:NO" tmp_manifest ${CMAKE_EXE_LINKER_FLAGS})
    IF(NOT tmp_manifest)
        SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /MANIFEST:NO")
    ENDIF(NOT tmp_manifest)
ENDIF(MSVC)
message(STATUS "CMAKE_EXE_LINKER_FLAGS=${CMAKE_EXE_LINKER_FLAGS}----")

IF(WIN32)
  ADD_DEFINITIONS("-D_WINDOWS -D__WIN__ -D_CRT_SECURE_NO_DEPRECATE")
  if(${CMAKE_EXE_LINKER_FLAGS} MATCHES ".*X86.*")
	message(STATUS "++++X86++++")
	SET(PLATOS "win32")
  ELSE()
	SET(PLATOS "win")
  ENDIF()
  
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj ")
ELSE()
  SET(PLATOS "linux")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
ENDIF(WIN32)
add_definitions(-DLUA_WIDESTRING=1)
add_definitions(-DLUA_WIDESTRING_FILE=1)
add_definitions(-DTIXML_USE_STL)
#add_definitions(--std=c++0x)
if ("${CMAKE_BUILD_TYPE}" STREQUAL "debug" OR  "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	add_definitions(-D_DEBUG)
	add_definitions(-Ddebug)
	add_definitions(-DDebug)
	set( debug TRUE)
	message(STATUS "++++_DEBUG++++${CMAKE_BUILD_TYPE}")
else()
	add_definitions(-DNDEBUG)
	message(STATUS "++++NDEBUG++++${CMAKE_BUILD_TYPE}")
endif()

#add_definitions(-DLUAPLUS_EXCEPTIONS)
macro(source_group_by_dir source_files cur_dir)
    if(MSVC)
        set(sgbd_cur_group_dir ${cur_dir})
		set(sgbd_cur_dir ${CMAKE_CURRENT_SOURCE_DIR})
        foreach(sgbd_file ${${source_files}})
            string(REGEX REPLACE ${sgbd_cur_dir}/\(.*\) \\1 sgbd_fpath ${sgbd_file})
			string(REGEX REPLACE ${sgbd_cur_group_dir}/\(.*\) \\1 sgbd_group_fpath ${sgbd_file})
            string(REGEX REPLACE "\(.*\)/.*" \\1 sgbd_group_name ${sgbd_fpath})
			string(REGEX REPLACE "\(.*\)/.*" \\1 sgbd_group_real_name ${sgbd_group_fpath})
            string(COMPARE EQUAL ${sgbd_fpath} ${sgbd_group_name} sgbd_nogroup)
            string(REPLACE "/" "\\" sgbd_group_real_name ${sgbd_group_real_name})

            if(sgbd_nogroup)
                set(sgbd_group_real_name "\\")
            endif(sgbd_nogroup)
			#message(STATUS "\n${sgbd_group_real_name} --- ${sgbd_file}")
            source_group("${sgbd_group_real_name}" FILES "${sgbd_file}")
        endforeach(sgbd_file)
    endif(MSVC)
endmacro(source_group_by_dir)

#set(SHARED_DIR ${PROJECT_SOURCE_DIR}/../thrid_party/libuv)
#message(STATUS "SHARED_DIR=${SHARED_DIR}")
#set(SHARED_INCLUDE_DIR ${SHARED_DIR}/include)


file(GLOB_RECURSE SRCS "." "*.cpp")
file(GLOB_RECURSE HEADS "." "*.h")
file(GLOB_RECURSE HEADHPPS "." "*.hpp")
file(GLOB_RECURSE SRCCCS "." "*.cc")

set(SRCS ${SRCS}  ${HEADS} ${HEADHPPS} ${SRCCCS} )

#source_group_by_dir(SRCS ${CMAKE_CURRENT_SOURCE_DIR})
#set(PUB_HEADS ${SHARED_INCLUDE_DIR}/INetInterface.h	)
#source_group("PublicIncludes" FILES ${PUB_HEADS})

#file(GLOB_RECURSE COMMON_HEADS "${SHARED_INCLUDE_DIR}/Common/*.h")
#message(STATUS "COMMON_HEADS=${COMMON_HEADS}")
#source_group_by_dir(COMMON_HEADS ${SHARED_INCLUDE_DIR})

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../thrid_party/libuv/include
					)
if(MSVC)
#INCLUDE_DIRECTORIES(${SHARED_INCLUDE_DIR}/ThreadData)
#include_directories(${SHARED_INCLUDE_DIR}/hiredis_win)
else()
#include_directories(${SHARED_INCLUDE_DIR}/hiredis_linux)
#INCLUDE_DIRECTORIES(${SHARED_INCLUDE_DIR}/ThreadData)
endif()

#set(PUB_HEADS ${SHARED_INCLUDE_DIR}/INetInterface.h	)
#source_group("PublicIncludes" FILES ${PUB_HEADS})
#set(SHARED_LIBS_PATH ${SHARED_DIR}/lib/${PLATOS})
#configure_file (
#  "${PROJECT_SOURCE_DIR}/Config.h.in"
#  "${PROJECT_SOURCE_DIR}/Config.h"
#  )
if(MSVC)
else()

#link_directories(/usr/lib/mysql)
#link_directories(/usr/lib64/mysql)
#link_directories(/usr/local/lib/mysql)
endif()
link_directories(${PROJECT_SOURCE_DIR}/../../thrid_party/libuv/Debug/lib)
if(NOT MSVC)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} ")
endif()
#link_directories(${SHARED_LIBS_PATH})

ADD_EXECUTABLE(${PROJECT_NAME}  ${SRCS})

set_target_properties( ${PROJECT_NAME}
		PROPERTIES
		OUTPUT_NAME "${PROJECT_NAME}"
		DEBUG_POSTFIX "d"
    )
INSTALL(TARGETS ${PROJECT_NAME}
		RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin
		LIBRARY DESTINATION ${PROJECT_SOURCE_DIR}/lib
		ARCHIVE DESTINATION ${PROJECT_SOURCE_DIR}/lib
		)

if( NOT WIN32 )
TARGET_LINK_LIBRARIES(${PROJECT_NAME}  
uv rt dl pthread)
else()
TARGET_LINK_LIBRARIES(${PROJECT_NAME}
libuv.lib   
advapi32.lib
iphlpapi.lib
psapi.lib
shell32.lib
userenv.lib
ws2_32.lib
)
endif()






#message(STATUS "CMAKE_CURRENT_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}")
#message(STATUS "PROJECT_BINARY_DIR=${PROJECT_BINARY_DIR}")
#message(STATUS "PROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}")
#message(STATUS "CMAKE_SHARED_LIBRARY_SUFFIX=${CMAKE_SHARED_LIBRARY_SUFFIX}")
#message(STATUS "CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
#message(STATUS "CMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}")
#message(STATUS "CMAKE_C_FLAGS=${CMAKE_C_FLAGS}")
#message(STATUS "CMAKE_CXX_FLAGS_DEBUG=${CMAKE_CXX_FLAGS_DEBUG}")
#message(STATUS "CMAKE_C_FLAGS_DEBUG=${CMAKE_C_FLAGS_DEBUG}")
#message(STATUS "CMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}")
#message(STATUS "CMAKE_C_COMPILER=${CMAKE_C_COMPILER}")



