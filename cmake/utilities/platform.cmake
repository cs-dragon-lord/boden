set(BDN_AVAILABLE_PLATFORMS "linux;mac;win32;ios;android;winstore;winuwp;webems;unix" CACHE STRIGN "List of possible build targets")

# Auto detect target platform

if( IOS )
    set( BDN_DETECTED_TARGET "ios" )
elseif( CMAKE_SYSTEM_NAME STREQUAL "Linux" )
    set( BDN_DETECTED_TARGET "linux" )
elseif( CMAKE_SYSTEM_NAME STREQUAL "Darwin" )
    set( BDN_DETECTED_TARGET "mac" )
elseif( ANDROID )
    set( BDN_DETECTED_TARGET "android" )
elseif( CMAKE_SYSTEM_NAME STREQUAL "Windows" )
    set( BDN_DETECTED_TARGET "win32" )
elseif( CMAKE_SYSTEM_NAME STREQUAL "WindowsStore" )
    if(CMAKE_SYSTEM_VERSION VERSION_LESS "10.0")
        message(FATAL_ERROR "Windows Store is only supported for Windows 10 and higher")
    else()
        set( BDN_DETECTED_TARGET "winuwp" )
    endif()
elseif( CMAKE_SYSTEM_NAME STREQUAL "Emscripten" )
    set( BDN_DETECTED_TARGET "webems" )
else()
    # assume some Unix variant
    set( BDN_DETECTED_TARGET "unix" )
endif()

# Select target platform ( auto detect, or use users selection )

if(NOT BDN_TARGET)
    set(BDN_TARGET ${BDN_DETECTED_TARGET})
endif()


# Global options based on selected platform

set(BDN_USES_PTHREAD No)
set(BDN_USES_FK No)
set(BDN_USES_JAVA No)
set(BDN_USES_GTK No)
set(BDN_NEEDS_TO_BE_SHARED_LIBRARY No)
set(BDN_NEEDS_TO_BE_STATIC_LIBRARY No)

set(BDN_PLATFORM_IOS No)
set(BDN_PLATFORM_OSX No)
set(BDN_PLATFORM_ANDROID No)
set(BDN_PLATFORM_LINUX No)
set(BDN_PLATFORM_UNIX No)
set(BDN_PLATFORM_WIN32 No)
set(BDN_PLATFORM_WINUWP No)
set(BDN_PLATFORM_WEBEMS No)

if(BDN_TARGET STREQUAL "ios")
    set( BDN_USES_PTHREAD Yes )
    set( BDN_USES_FK Yes )
    set( BDN_PLATFORM_FAMILY "posix")
    set( BDN_NEEDS_TO_BE_STATIC_LIBRARY Yes )
    set( BDN_PLATFORM_IOS Yes )
elseif(BDN_TARGET STREQUAL "mac")
    set( BDN_USES_PTHREAD Yes )
    set( BDN_USES_FK Yes )
    set( BDN_PLATFORM_FAMILY "posix")
    set( BDN_PLATFORM_OSX Yes )
elseif(BDN_TARGET STREQUAL "android")
    set( BDN_USES_PTHREAD Yes )
    set( BDN_USES_JAVA Yes )
    set( BDN_PLATFORM_FAMILY "posix")
    set( BDN_NEEDS_TO_BE_SHARED_LIBRARY Yes )
    set( BDN_PLATFORM_ANDROID Yes )
elseif(BDN_TARGET STREQUAL "linux")
    set( BDN_USES_PTHREAD Yes )
    set( BDN_USES_GTK Yes )
    set( BDN_PLATFORM_FAMILY "posix")
    set( BDN_PLATFORM_LINUX Yes )
elseif(BDN_TARGET STREQUAL "unix")
    set( BDN_USES_PTHREAD Yes )
    set( BDN_PLATFORM_FAMILY "posix")
    set( BDN_PLATFORM_UNIX Yes )
elseif(BDN_TARGET STREQUAL "win32")
    set( BDN_PLATFORM_FAMILY "windows")
    set( BDN_PLATFORM_WIN32 Yes )
elseif(BDN_TARGET STREQUAL "winuwp")
    set( BDN_PLATFORM_FAMILY "windows")
    set( BDN_NEEDS_TO_BE_SHARED_LIBRARY Yes)
    set( BDN_PLATFORM_WINUWP Yes )
elseif(BDN_TARGET STREQUAL "webems")
    set( BDN_PLATFORM_WEBEMS Yes )
endif()

if( "${BDN_PLATFORM_FAMILY}" STREQUAL "posix" )
    set( BDN_PLATFORM_FAMILY_POSIX Yes )
elseif( "${BDN_PLATFORM_FAMILY}" STREQUAL "windows" )
    set( BDN_PLATFORM_FAMILY_WINDOWS Yes )
endif()

message(STATUS "Boden platform:")
message(STATUS "  Name: ${BDN_TARGET}")
message(STATUS "  Family: ${BDN_PLATFORM_FAMILY}")
message(STATUS "Boden configuration:")
message(STATUS "  Uses pthread: ${BDN_USES_PTHREAD}")
message(STATUS "  Uses foundation kit: ${BDN_USES_FK}")
message(STATUS "  Uses java: ${BDN_USES_JAVA}")
message(STATUS "  Uses gtk: ${BDN_USES_GTK}")
message(STATUS "  Force shared: ${BDN_NEEDS_TO_BE_SHARED_LIBRARY}")
message(STATUS "  Force static: ${BDN_NEEDS_TO_BE_STATIC_LIBRARY}")

if(BDN_PLATFORM_OSX)
    if("${CMAKE_OSX_DEPLOYMENT_TARGET}" STREQUAL "")
        message(STATUS "  Mac OS X SDK: <unknown>")
    else()
        get_filename_component(sdk_version ${CMAKE_OSX_SYSROOT} NAME)
        message(STATUS "  Mac OS X SDK: ${sdk_version} (${CMAKE_OSX_SYSROOT})")
    endif()
    
    
    if("${CMAKE_OSX_DEPLOYMENT_TARGET}" STREQUAL "")
        message(STATUS "  Deployment target: <default>")
    else()
        message(STATUS "  Deployment target: ${CMAKE_OSX_DEPLOYMENT_TARGET}")
    endif()
endif()

# Make options globally available

set(BDN_TARGET ${BDN_TARGET} CACHE INTERNAL "Boden target name")
set(BDN_PLATFORM_FAMILY ${BDN_PLATFORM_FAMILY} CACHE INTERNAL "Boden target family")
set(BDN_USES_PTHREAD ${BDN_USES_PTHREAD} CACHE INTERNAL "Boden uses pthread")
set(BDN_USES_FK ${BDN_USES_FK} CACHE INTERNAL "Boden uses Foundation Kit")
set(BDN_USES_JAVA ${BDN_USES_JAVA} CACHE INTERNAL "Boden uses java")
set(BDN_USES_GTK ${BDN_USES_GTK} CACHE INTERNAL "Boden uses gtk")
set(BDN_NEEDS_TO_BE_SHARED_LIBRARY ${BDN_NEEDS_TO_BE_SHARED_LIBRARY} CACHE INTERNAL "boden needs to be a shared library")
set(BDN_NEEDS_TO_BE_STATIC_LIBRARY ${BDN_NEEDS_TO_BE_STATIC_LIBRARY} CACHE INTERNAL "boden needs to be a static library")

set(BDN_PLATFORM_IOS            ${BDN_PLATFORM_IOS} CACHE INTERNAL "")
set(BDN_PLATFORM_OSX            ${BDN_PLATFORM_OSX} CACHE INTERNAL "")
set(BDN_PLATFORM_ANDROID        ${BDN_PLATFORM_ANDROID} CACHE INTERNAL "")
set(BDN_PLATFORM_LINUX          ${BDN_PLATFORM_LINUX} CACHE INTERNAL "")
set(BDN_PLATFORM_UNIX           ${BDN_PLATFORM_UNIX} CACHE INTERNAL "")
set(BDN_PLATFORM_WIN32          ${BDN_PLATFORM_WIN32} CACHE INTERNAL "")
set(BDN_PLATFORM_WINUWP         ${BDN_PLATFORM_WINUWP} CACHE INTERNAL "")
set(BDN_PLATFORM_WEBEMS         ${BDN_PLATFORM_WEBEMS} CACHE INTERNAL "")

set(BDN_PLATFORM_FAMILY_POSIX   ${BDN_PLATFORM_FAMILY_POSIX} CACHE INTERNAL "")
set(BDN_PLATFORM_FAMILY_WINDOWS ${BDN_PLATFORM_FAMILY_WINDOWS} CACHE INTERNAL "")

mark_as_advanced(BDN_PLATFORM_IOS)
mark_as_advanced(BDN_PLATFORM_OSX)
mark_as_advanced(BDN_PLATFORM_ANDROID)
mark_as_advanced(BDN_PLATFORM_LINUX)
mark_as_advanced(BDN_PLATFORM_UNIX)
mark_as_advanced(BDN_PLATFORM_WIN32)
mark_as_advanced(BDN_PLATFORM_WINUWP)
mark_as_advanced(BDN_PLATFORM_WEBEMS )

mark_as_advanced(BDN_PLATFORM_FAMILY_POSIX)
mark_as_advanced(BDN_PLATFORM_FAMILY_WINDOWS)

set_property(CACHE BDN_TARGET PROPERTY STRINGS ${BDN_AVAILABLE_PLATFORMS})
mark_as_advanced(BDN_AVAILABLE_PLATFORMS)

macro(add_universal_library TARGET)
    if(BDN_PLATFORM_ANDROID)
        # Static libs don't work well on android at the moment due to problems in build.py with dependency generation
        add_library(${TARGET} SHARED ${ARGN})
    else()
        add_library(${TARGET} ${ARGN})
    endif()
endmacro()

macro(add_universal_executable TARGET CONSOLE_APP)
    if(BDN_PLATFORM_IOS)
        add_executable(${TARGET} MACOSX_BUNDLE ${ARGN} )
    elseif(BDN_PLATFORM_OSX)
        if(${CONSOLE_APP})
            add_executable(${TARGET} ${ARGN} )
        else()
            add_executable(${TARGET} MACOSX_BUNDLE ${ARGN} )
        endif()
    elseif(BDN_PLATFORM_ANDROID)
        if(${BAUER_RUN})
            add_executable(${TARGET} ${ARGN} )
        else()
            add_library(${TARGET} SHARED ${ARGN} )
        endif()
    elseif(BDN_PLATFORM_LINUX)
        add_executable(${TARGET} ${ARGN} )
    elseif(BDN_PLATFORM_UNIX)
        add_executable(${TARGET} ${ARGN} )
    elseif(BDN_PLATFORM_WIN32)
        if(${CONSOLE_APP})
            add_executable(${TARGET} ${ARGN} )
        else()
            add_executable(${TARGET} WIN32 ${ARGN} )
        endif()
    elseif(BDN_PLATFORM_WINUWP)
        add_executable(${TARGET} WIN32 ${ARGN} )
    elseif(BDN_PLATFORM_WEBEMS)
        add_executable(${TARGET} ${ARGN} )
    endif()

    if(${CONSOLE_APP})
        target_compile_definitions(${TARGET} PRIVATE "BDN_COMPILING_COMMANDLINE_APP")
    endif()

endmacro()
