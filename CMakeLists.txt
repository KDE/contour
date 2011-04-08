project(contour)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${nepomuk_kde_SOURCE_DIR}/cmake)

# set the version to be used by ContourConfig.cmake and the lib version
# ===============================================================================================
set(CMAKE_CONTOUR_VERSION_MAJOR 0)
set(CMAKE_CONTOUR_VERSION_MINOR 1)
set(CMAKE_CONTOUR_VERSION_RELEASE 0)
set(CMAKE_CONTOUR_VERSION "${CMAKE_CONTOUR_VERSION_MAJOR}.${CMAKE_CONTOUR_VERSION_MINOR}.${CMAKE_CONTOUR_VERSION_RELEASE}")


# search required packages
# ===============================================================================================
set(KDE_MIN_VERSION "4.5.00")
find_package(KDE4 4.5.00 REQUIRED)
include(KDE4Defaults)
find_package(KDE4Workspace)
include(MacroLibrary)

find_package(SharedDesktopOntologies 0.5)
set(SOPRANO_MIN_VERSION "2.5.0")
find_package(Soprano)
find_package(Nepomuk)

macro_log_feature(SOPRANO_FOUND "Soprano" "Support for the Nepomuk semantic desktop system" "http://soprano.sourceforge.net" TRUE "${SOPRANO_MIN_VERSION}" "")
macro_log_feature(SHAREDDESKTOPONTOLOGIES_FOUND "Shared desktop ontologies" "Desktop ontologies" "http://oscaf.sourceforge.net" TRUE ${SDO_MIN_VERSION} "Ontologies necessary for the Nepomuk semantic desktop.")
macro_log_feature(Nepomuk_FOUND "Nepomuk" "Nepomuk Libraries" "kdelibs" TRUE "" "The basis for this package.")


# generate ContourConfig.cmake for easy utilisation of the package by other cmake build systems
# ===============================================================================================
configure_file(ContourConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/ContourConfig.cmake @ONLY)
configure_file(ContourConfigVersion.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/ContourConfigVersion.cmake @ONLY)
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/ContourConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/ContourConfigVersion.cmake
  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/cmake/Contour)


# build stuff
# ===============================================================================================
include_directories(
  ${contour_SOURCE_DIR}/location
  ${contour_SOURCE_DIR}/recommendation
  ${contour_BINARY_DIR}
  ${QT_INCLUDES}
  ${KDE4_INCLUDES}
  ${NEPOMUK_INCLUDE_DIR}
  ${SOPRANO_INCLUDE_DIR}
#hack to find qt-mobility includes. TODO: get cmake macro
  ${INCLUDE_INSTALL_DIR}/QtMobility
  ${INCLUDE_INSTALL_DIR}/QtMobility/QtMobility
)

set(recommendationmanager_SRC
  recommendation/recommendationaction.cpp
  recommendation/recommendation.cpp
  recommendation/recommendationmanager.cpp
  recommendation/dbusoperators.cpp
  location/locationmanager.cpp
  main.cpp
)

qt4_add_dbus_adaptor(
  recommendationmanager_SRC
  "recommendation/org.kde.contour.RecommendationManager.xml"
  "recommendationmanager.h"
  "Contour::RecommendationManager"
  "recommendationmanageradaptor")

find_file(kext_onto_src kext.trig PATH_SUFFIXES "ontology/kde" PATHS ${CMAKE_INSTALL_PREFIX}/share ENV XDG_DATA_DIRS)
soprano_add_ontology(recommendationmanager_SRC ${kext_onto_src} "KExt" "Nepomuk::Vocabulary" "trig")

kde4_add_executable(contour
  ${recommendationmanager_SRC}
)

target_link_libraries(
  contour
  ${QT_QTCORE_LIBRARY}
  ${KDE4_KDECORE_LIBS}
  ${SOPRANO_LIBRARIES}
  ${NEPOMUK_LIBRARIES}
  ${NEPOMUK_QUERY_LIBRARIES}
  kworkspace
  QtLocation
)

add_subdirectory(plasma)

macro_display_feature_log()
