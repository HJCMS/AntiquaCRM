##
# Generated by ./createsubtargets.sh
##
SET (PROJECT_INCLUDE_DIRS
 ${PROJECT_INCLUDE_DIRS}
 ${CMAKE_CURRENT_BINARY_DIR}/core
 ${CMAKE_CURRENT_SOURCE_DIR}/core
 ${CMAKE_CURRENT_BINARY_DIR}/core/antiquacore
 ${CMAKE_CURRENT_SOURCE_DIR}/core/antiquacore
 ${CMAKE_CURRENT_BINARY_DIR}/core/sqlcore
 ${CMAKE_CURRENT_SOURCE_DIR}/core/sqlcore
 ${CMAKE_CURRENT_BINARY_DIR}/custom
 ${CMAKE_CURRENT_SOURCE_DIR}/custom
 ${CMAKE_CURRENT_BINARY_DIR}/custom/assistant
 ${CMAKE_CURRENT_SOURCE_DIR}/custom/assistant
 ${CMAKE_CURRENT_BINARY_DIR}/custom/autofill
 ${CMAKE_CURRENT_SOURCE_DIR}/custom/autofill
 ${CMAKE_CURRENT_BINARY_DIR}/custom/completer
 ${CMAKE_CURRENT_SOURCE_DIR}/custom/completer
 ${CMAKE_CURRENT_BINARY_DIR}/custom/deliverservice
 ${CMAKE_CURRENT_SOURCE_DIR}/custom/deliverservice
 ${CMAKE_CURRENT_BINARY_DIR}/custom/imaging
 ${CMAKE_CURRENT_SOURCE_DIR}/custom/imaging
 ${CMAKE_CURRENT_BINARY_DIR}/custom/networking
 ${CMAKE_CURRENT_SOURCE_DIR}/custom/networking
 ${CMAKE_CURRENT_BINARY_DIR}/custom/printing
 ${CMAKE_CURRENT_SOURCE_DIR}/custom/printing
 ${CMAKE_CURRENT_BINARY_DIR}/custom/settings
 ${CMAKE_CURRENT_SOURCE_DIR}/custom/settings
 ${CMAKE_CURRENT_BINARY_DIR}/custom/storage
 ${CMAKE_CURRENT_SOURCE_DIR}/custom/storage
 ${CMAKE_CURRENT_BINARY_DIR}/custom/utils
 ${CMAKE_CURRENT_SOURCE_DIR}/custom/utils
 ${CMAKE_CURRENT_BINARY_DIR}/i18n
 ${CMAKE_CURRENT_SOURCE_DIR}/i18n
 ${CMAKE_CURRENT_BINARY_DIR}/icons
 ${CMAKE_CURRENT_SOURCE_DIR}/icons
 ${CMAKE_CURRENT_BINARY_DIR}/inventory
 ${CMAKE_CURRENT_SOURCE_DIR}/inventory
 ${CMAKE_CURRENT_BINARY_DIR}/inventory/books
 ${CMAKE_CURRENT_SOURCE_DIR}/inventory/books
 ${CMAKE_CURRENT_BINARY_DIR}/inventory/customers
 ${CMAKE_CURRENT_SOURCE_DIR}/inventory/customers
 ${CMAKE_CURRENT_BINARY_DIR}/inventory/orders
 ${CMAKE_CURRENT_SOURCE_DIR}/inventory/orders
 ${CMAKE_CURRENT_BINARY_DIR}/inventory/prints
 ${CMAKE_CURRENT_SOURCE_DIR}/inventory/prints
 ${CMAKE_CURRENT_BINARY_DIR}/inventory/providers
 ${CMAKE_CURRENT_SOURCE_DIR}/inventory/providers
 ${CMAKE_CURRENT_BINARY_DIR}/inventory/views
 ${CMAKE_CURRENT_SOURCE_DIR}/inventory/views
 ${CMAKE_CURRENT_BINARY_DIR}/plugins
 ${CMAKE_CURRENT_SOURCE_DIR}/plugins
 ${CMAKE_CURRENT_BINARY_DIR}/plugins/antiqua
 ${CMAKE_CURRENT_SOURCE_DIR}/plugins/antiqua
 ${CMAKE_CURRENT_BINARY_DIR}/plugins/antiqua/abebooks
 ${CMAKE_CURRENT_SOURCE_DIR}/plugins/antiqua/abebooks
 ${CMAKE_CURRENT_BINARY_DIR}/plugins/antiqua/booklooker
 ${CMAKE_CURRENT_SOURCE_DIR}/plugins/antiqua/booklooker
 ${CMAKE_CURRENT_BINARY_DIR}/plugins/antiqua/whsoft
 ${CMAKE_CURRENT_SOURCE_DIR}/plugins/antiqua/whsoft
 ${CMAKE_CURRENT_BINARY_DIR}/xml
 ${CMAKE_CURRENT_SOURCE_DIR}/xml
)

INCLUDE_DIRECTORIES (${PROJECT_INCLUDE_DIRS})

ADD_SUBDIRECTORY (core)
ADD_SUBDIRECTORY (custom)
ADD_SUBDIRECTORY (i18n)
ADD_SUBDIRECTORY (inventory)
ADD_SUBDIRECTORY (xml)
ADD_SUBDIRECTORY (plugins)
ADD_SUBDIRECTORY (icons)

SET(_link_sub_libs
SqlCore
AntiquaCore
Settings
Completer
Networking
Utils
Imaging
Assistant
Autofill
Printing
StorageLocation
DeliverService
InventoryBooks
InventoryPrints
InventoryOrders
InventoryProviders
InventoryCustomers
InventoryViews
)

