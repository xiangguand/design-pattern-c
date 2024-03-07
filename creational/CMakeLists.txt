FILE(GLOB TEST_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
)

foreach(file ${TEST_FILES})
    get_filename_component(barefile ${file} NAME_WE)
    add_executable(${barefile} ${file} ${SOURCE_FILE})
    target_include_directories(${barefile} PUBLIC ${DEPEN_DIR})
    target_link_libraries(${barefile} PUBLIC 
        m
    )
endforeach()