SET (PROJECT_INCLUDE_DIRS
	${CMAKE_CURRENT_BINARY_DIR}/widgets
	${CMAKE_CURRENT_SOURCE_DIR}/widgets
	${CMAKE_CURRENT_BINARY_DIR}/utils/imgedit
	${CMAKE_CURRENT_SOURCE_DIR}/utils/imgedit
	${CMAKE_CURRENT_BINARY_DIR}/utils/deliveryservice
	${CMAKE_CURRENT_SOURCE_DIR}/utils/deliveryservice
	${CMAKE_CURRENT_BINARY_DIR}/utils/storage
	${CMAKE_CURRENT_SOURCE_DIR}/utils/storage
	${CMAKE_CURRENT_BINARY_DIR}/utils/keywordedit
	${CMAKE_CURRENT_SOURCE_DIR}/utils/keywordedit
	${CMAKE_CURRENT_BINARY_DIR}/utils/settings
	${CMAKE_CURRENT_SOURCE_DIR}/utils/settings
	${CMAKE_CURRENT_BINARY_DIR}/utils/cache
	${CMAKE_CURRENT_SOURCE_DIR}/utils/cache
	${CMAKE_CURRENT_BINARY_DIR}/tabs/views
	${CMAKE_CURRENT_SOURCE_DIR}/tabs/views
	${CMAKE_CURRENT_BINARY_DIR}/tabs/books
	${CMAKE_CURRENT_SOURCE_DIR}/tabs/books
	${CMAKE_CURRENT_BINARY_DIR}/lib
	${CMAKE_CURRENT_SOURCE_DIR}/lib
	${PROJECT_INCLUDE_DIRS}
)

INCLUDE_DIRECTORIES (${PROJECT_INCLUDE_DIRS})

SET(PROJECT_LIBRARIES
	AntiquaWidgets
	AntiquaImgEdit
	DeliveryService
	AntiquaStorage
	AntiquaKeywords
	AntiquaSettings
	AntiquaCache
	AntiquaViews
	AntiquaBooks
	AntiquaCRM
)

