find_package(Doxygen)
if (DOXYGEN_FOUND)
    add_custom_target(docs ALL
        COMMAND ${DOXYGEN_EXECUTABLE}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/docs
        COMMENT "Generating Doxygen documentation"
    )
else()
    message(WARNING "DOXYGEN is not found")
endif()