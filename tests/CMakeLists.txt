# Tests are built into ${CMAKE_BINARY_DIR}/tests to keep them separated from the main application.

find_package(Catch2 QUIET)

if(Catch2_FOUND)
    message(STATUS "Using system Catch2 package")
else()
    message(STATUS "System Catch2 was not found. Using local lightweight Catch2-compatible test runner.")
    add_library(docgen_minicatch2 INTERFACE)
    target_include_directories(docgen_minicatch2 INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/minicatch2
    )
    add_library(Catch2::Catch2 ALIAS docgen_minicatch2)
endif()

set(TEST_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests)

set(UNIT_TEST_SOURCES
    test_main.cpp
    test_string_utils.cpp
    test_file_iterator.cpp
    test_file_scanner.cpp
    test_parser_factory.cpp
    test_cpp_parser.cpp
    test_html_builder.cpp
    test_generation_strategy.cpp
    test_observer.cpp
    test_documentation_generator.cpp
)

add_executable(docgen_unit_tests ${UNIT_TEST_SOURCES})
set_target_properties(docgen_unit_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIRECTORY}
)
target_link_libraries(docgen_unit_tests PRIVATE docgen_core Catch2::Catch2)
target_include_directories(docgen_unit_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_test(NAME docgen_unit_tests COMMAND docgen_unit_tests)

function(add_docgen_scenario target_name source_file)
    add_executable(${target_name} ${source_file})
    set_target_properties(${target_name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIRECTORY}
    )
    target_link_libraries(${target_name} PRIVATE docgen_core)
    target_include_directories(${target_name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    add_test(NAME ${target_name} COMMAND ${target_name})
endfunction()

add_docgen_scenario(scenario_simple_generation scenarios/scenario_simple_generation.cpp)
add_docgen_scenario(scenario_advanced_generation scenarios/scenario_advanced_generation.cpp)
add_docgen_scenario(scenario_empty_project scenarios/scenario_empty_project.cpp)
add_docgen_scenario(scenario_unsupported_files scenarios/scenario_unsupported_files.cpp)
