project(activitymanager-plugin-qtmobilitycontactsfeeder)

# This should be REQUIRED once we get rid of the hack below
find_package(QtMobility)

message("Qt Mobility is here: ${QTMOBILITY_INCLUDE_DIR}")

configure_file(config-qtc.h.cmake
   ${CMAKE_CURRENT_BINARY_DIR}/config-qtc.h )

include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/foreign
   ${CMAKE_CURRENT_BINARY_DIR}
   ${KDE4_INCLUDES}
#hack to find qt-mobility includes. TODO: get cmake macro
   ${INCLUDE_INSTALL_DIR}/QtMobility
   ${INCLUDE_INSTALL_DIR}/QtMobility/QtMobility
#this uses the cmake macro
   ${QTMOBILITY_INCLUDE_DIR}
   ${QTMOBILITY_INCLUDE_DIR}/../QtContacts
   ${QTMOBILITY_INCLUDE_DIR}/../QtMessaging
   )

set(
   qtmobilitycontactsfeeder_SRCS
   QtContactsFeederPlugin.cpp
   QtContactsFeeder.cpp
   QtContactsScorer.cpp
   QtContactsManager.cpp
   foreign/Plugin.cpp
   foreign/SharedInfo.cpp
   )

find_file(kext_onto_src kext.trig PATH_SUFFIXES "ontology/kde" PATHS ${CMAKE_INSTALL_PREFIX}/share ENV XDG_DATA_DIRS)

soprano_add_ontology(qtmobilitycontactsfeeder_SRCS
    ${kext_onto_src}
    "KExt" "Nepomuk::Vocabulary" "trig"
    )

soprano_add_ontology(qtmobilitycontactsfeeder_SRCS
    ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/nie/nco.trig
    "NCO" "Nepomuk::Vocabulary" "trig"
    )

soprano_add_ontology(qtmobilitycontactsfeeder_SRCS
    ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/nepomuk/nao.trig
    "NAO" "Nepomuk::Vocabulary" "trig"
    )

soprano_add_ontology(qtmobilitycontactsfeeder_SRCS
    ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/nie/nie.trig
    "NIE" "Nepomuk::Vocabulary" "trig"
    )

kde4_add_plugin(
   activitymanager_plugin_qtmobilitycontactsfeeder
   ${qtmobilitycontactsfeeder_SRCS}
   )

target_link_libraries(
   activitymanager_plugin_qtmobilitycontactsfeeder
   ${KDE4_KDECORE_LIBS}
   ${KDE4_KDEUI_LIBS}
   ${SOPRANO_LIBRARIES}
   ${NEPOMUK_LIBRARIES}
   ${NEPOMUK_QUERY_LIBRARIES}
   QtContacts
   QtMessaging
   )

install(TARGETS activitymanager_plugin_qtmobilitycontactsfeeder DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES activitymanager-plugin-qtmobilitycontactsfeeder.desktop DESTINATION ${SERVICES_INSTALL_DIR})

