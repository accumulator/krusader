project(krusader)

cmake_minimum_required (VERSION 2.8.12 FATAL_ERROR)
set (QT_MIN_VERSION "5.2.0")

set(VERSION "2.4.0-beta3" )
set(RELEASE_NAME "Single Step")

find_package(ECM 1.1.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(ECMOptionalAddSubdirectory)
include(ECMInstallIcons)
include(ECMSetupVersion)
include(ECMMarkNonGuiExecutable)
include(ECMGenerateHeaders)
include(GenerateExportHeader)

include(CMakePackageConfigHelpers)
include(FeatureSummary)
include(WriteBasicConfigVersionFile)
include(CheckFunctionExists)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)

find_package(Qt5 CONFIG REQUIRED
  Core
  DBus
  Widgets
  Script
  Sql
  Test
  Xml
)

find_package(KF5 REQUIRED COMPONENTS
  Archive
  Config
  DocTools
  IconThemes
  KDELibs4Support
  KHtml
  Parts
  Wallet
)

include_directories(${Plasma_INCLUDE_DIR} ${QT5_INCLUDES})

set(CMAKE_VERBOSE_MAKEFILE OFF)

check_cxx_compiler_flag("-Wno-deprecated-declarations" CXX_SUPPORTS_NO_DEPRECATED_DECLARATIONS)

if (CXX_SUPPORTS_NO_DEPRECATED_DECLARATIONS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")
#  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-overloaded-virtual")
endif ()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

add_definitions(${QT5_DEFINITIONS} ${Plasma_DEFINITIONS})
add_definitions( "-DKRARC_ENABLED" )

if(ENABLE_SYNCHRONIZER)
    add_definitions( "-DENABLE_SYNCHRONIZER" )
endif(ENABLE_SYNCHRONIZER)

find_package(ACL)
if(ACL_FOUND)
    add_definitions(-DHAVE_POSIX_ACL)
endif(ACL_FOUND)

#include(ConfigureChecks.cmake)

add_subdirectory(krusader)
add_subdirectory(pics)
add_subdirectory(doc-extras)
add_subdirectory(krArc)
add_subdirectory(iso)
add_subdirectory(doc)
#add_subdirectory(virt) - not used

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
