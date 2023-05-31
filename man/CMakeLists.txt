# Converts manual pages using scdoc and then compresses them
# Creates a static library to represent the manual pages
add_custom_target(ManualPages
		COMMENT "Compiling and compressing manual pages"

		COMMAND scdoc < ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.1.scd > ${PROJECT_NAME}.1
		COMMAND scdoc < ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.5.scd > ${PROJECT_NAME}.5

		COMMAND gzip -f ${PROJECT_NAME}.1 ${PROJECT_NAME}.5

		DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.1.scd ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.5.scd
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_dependencies(${PROJECT_NAME} ManualPages)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.1.gz
		DESTINATION ${CMAKE_INSTALL_MANDIR}/man1)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.5.gz
		DESTINATION ${CMAKE_INSTALL_MANDIR}/man5)
