PROJECT(ALE)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8 FATAL_ERROR)

MESSAGE(STATUS "Install directory $ENV{CMAKE_PREFIX_PATH}")

# Remove deprecated warnings from MSVC.
IF (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
  ADD_DEFINITIONS(/DWIN32)
  ADD_DEFINITIONS(/DBSPF_WIN32)
  ADD_DEFINITIONS(/D_CRT_SECURE_NO_WARNINGS)
  ADD_DEFINITIONS(/W3)
  ADD_DEFINITIONS(/wd4800)
  ADD_DEFINITIONS(/wd4244)
  ADD_DEFINITIONS(/wd4996)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
ENDIF()


# Compiler flags.
# Using Clang.
IF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    MESSAGE(STATUS "Using Clang")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")        ## Optimize
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftemplate-depth=1024")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")        ## Optimize
    MESSAGE(STATUS "FLAGS ${CMAKE_CXX_FLAGS}")
# Using GCC.
ELSEIF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    MESSAGE(STATUS "Using GCC")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")        ## Optimize
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")        ## Optimize
    MESSAGE(STATUS "FLAGS ${CMAKE_CXX_FLAGS}")
# Using Intel C++.
ELSEIF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    MESSAGE(STATUS "Using Intel C++")
# Using Visual Studio C++.
ELSEIF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    MESSAGE(STATUS "Using Visual Studio C++")
ENDIF()


INCLUDE_DIRECTORIES(.)


# Add source files.
FILE(GLOB top_files *.cpp *.cxx *.c *.hpp *.h *.hxx)
FILE(GLOB agents_files agents/*.cpp agents/*.cxx agents/*.c agents/*.hpp agents/*.h agents/*.hxx)
FILE(GLOB common_files common/*.cpp common/*.cxx common/*.c common/*.hpp common/*.h common/*.hxx)
FILE(GLOB controllers_files controllers/*.cpp controllers/*.cxx controllers/*.c controllers/*.hpp controllers/*.h controllers/*.hxx)
FILE(GLOB_RECURSE emucore_files emucore/*.cpp emucore/*.cxx emucore/*.c emucore/*.hpp emucore/*.h emucore/*.hxx)
FILE(GLOB environment_files environment/*.cpp environment/*.cxx environment/*.c environment/*.hpp environment/*.h environment/*.hxx)
FILE(GLOB_RECURSE games_files games/*.cpp games/*.cxx games/*.c games/*.hpp games/*.h games/*.hxx)
FILE(GLOB os_files os_dependent/*.cpp os_dependent/*.cxx os_dependent/*.c os_dependent/*.hpp os_dependent/*.h os_dependent/*.hxx)
FILE(GLOB zlib_files zlib/*.cpp zlib/*.cxx zlib/*.c zlib/*.hpp zlib/*.h zlib/*.hxx)


# Define executable.
ADD_EXECUTABLE(
    ale 
    ${top_files} 
    ${agents_files} 
    ${common_files} 
    ${controllers_files} 
    ${emucore_files} 
    ${environment_files} 
    ${games_files} 
    ${os_files} 
    ${zlib_files}
)

ADD_LIBRARY(
    xitari
    ale_interface.hpp 
    ${agents_files} 
    ${common_files} 
    ${controllers_files} 
    ${emucore_files} 
    ${environment_files} 
    ${games_files} 
    ${os_files} 
    ${zlib_files}
)

# Generate a xitari shared library on Linux.
IF (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
ADD_LIBRARY(
    xitari_shared
    SHARED
    ale_interface.hpp
    ${agents_files}
    ${common_files}
    ${controllers_files}
    ${emucore_files}
    ${environment_files}
    ${games_files}
    ${os_files}
    ${zlib_files}
)
SET_TARGET_PROPERTIES(xitari_shared PROPERTIES OUTPUT_NAME xitari)
ENDIF()

SET(INCDIR "${CMAKE_INSTALL_PREFIX}/include/xitari")
SET(LIBDIR "${CMAKE_INSTALL_PREFIX}/lib")

MESSAGE(STATUS "INCLUDE_DIR ${INCDIR}")
MESSAGE(STATUS "LIBRARY_DIR ${LIBDIR}")

INSTALL(
  FILES
  ale_interface.hpp
  DESTINATION "${INCDIR}"
)

IF(APPLE)
  INSTALL(FILES
          libxitari.a
          DESTINATION "${LIBDIR}")
  ELSE()
    INSTALL(FILES
            libxitari.a
            libxitari.so
            DESTINATION "${LIBDIR}")
ENDIF()

TARGET_LINK_LIBRARIES(ale)

FIND_PACKAGE(Threads)

SOURCE_GROUP(top FILES ${top_files})
SOURCE_GROUP(agents FILES ${agents_files})
SOURCE_GROUP(common FILES ${common_files})
SOURCE_GROUP(controllers FILES ${controllers_files})
SOURCE_GROUP(emucore FILES ${emucore_files})
SOURCE_GROUP(environment FILES ${environment_files})
SOURCE_GROUP(games FILES ${games_files})
SOURCE_GROUP(os FILES ${os_files})
SOURCE_GROUP(zlib FILES ${zlib_files})

