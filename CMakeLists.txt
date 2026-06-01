cmake_minimum_required(VERSION 3.10)

project(DocGenLite VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(DOCGEN_CORE_SOURCES
    src/core/DocumentationGenerator.cpp
    src/filesystem/FileScanner.cpp
    src/filesystem/FileIterator.cpp
    src/parser/ParserFactory.cpp
    src/parser/CppParser.cpp
    src/parser/HeaderParser.cpp
    src/generator/GenerationStrategyFactory.cpp
    src/generator/SimpleHtmlStrategy.cpp
    src/generator/AdvancedHtmlStrategy.cpp
    src/generator/HtmlBuilder.cpp
    src/observer/GenerationSubject.cpp
    src/utils/StringUtils.cpp
)

add_library(docgen_core STATIC ${DOCGEN_CORE_SOURCES})

target_include_directories(docgen_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(docgen_lite src/main.cpp)
target_link_libraries(docgen_lite PRIVATE docgen_core)

enable_testing()

# Tests are built into ${CMAKE_BINARY_DIR}/tests to keep them separated from the main application.
find_package(Catch2 QUIET)

if(Catch2_FOUND)
    message(STATUS "Using system Catch2 package")
else()
    message(STATUS "System Catch2 was not found. Using local lightweight Catch2-compatible test runner.")
    add_library(docgen_minicatch2 INTERFACE)
    target_include_directories(docgen_minicatch2 INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/minicatch2
    )
    add_library(Catch2::Catch2 ALIAS docgen_minicatch2)
endif()

set(TEST_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests)

set(UNIT_TEST_SOURCES
    tests/test_main.cpp
    tests/test_string_utils.cpp
    tests/test_file_iterator.cpp
    tests/test_file_scanner.cpp
    tests/test_parser_factory.cpp
    tests/test_cpp_parser.cpp
    tests/test_html_builder.cpp
    tests/test_generation_strategy.cpp
    tests/test_observer.cpp
    tests/test_documentation_generator.cpp
)

add_executable(docgen_unit_tests ${UNIT_TEST_SOURCES})
set_target_properties(docgen_unit_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIRECTORY}
)
target_link_libraries(docgen_unit_tests PRIVATE docgen_core Catch2::Catch2)
target_include_directories(docgen_unit_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tests)

set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${UNIT_TEST_SOURCES})

function(add_docgen_unit_tests target_name)
    foreach(test_source IN LISTS ARGN)
        file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/${test_source}" test_case_lines
            REGEX "^[ \t]*TEST_CASE\\(\"[^\"]+\""
        )

        foreach(test_case_line IN LISTS test_case_lines)
            string(REGEX MATCH "TEST_CASE\\(\"([^\"]+)\"" unused "${test_case_line}")
            set(test_name "${CMAKE_MATCH_1}")
            string(REGEX REPLACE "[^A-Za-z0-9_]+" "_" test_id "${test_name}")
            add_test(NAME "unit.${test_id}" COMMAND ${target_name} "${test_name}")
            set_tests_properties("unit.${test_id}" PROPERTIES LABELS unit)
        endforeach()
    endforeach()
endfunction()

add_docgen_unit_tests(docgen_unit_tests ${UNIT_TEST_SOURCES})

function(add_docgen_scenario target_name source_file)
    add_executable(${target_name} ${source_file})
    set_target_properties(${target_name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIRECTORY}
    )
    target_link_libraries(${target_name} PRIVATE docgen_core)
    target_include_directories(${target_name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tests)
    add_test(NAME ${target_name} COMMAND ${target_name})
endfunction()

add_docgen_scenario(scenario_simple_generation tests/scenarios/scenario_simple_generation.cpp)
add_docgen_scenario(scenario_advanced_generation tests/scenarios/scenario_advanced_generation.cpp)
add_docgen_scenario(scenario_empty_project tests/scenarios/scenario_empty_project.cpp)
add_docgen_scenario(scenario_unsupported_files tests/scenarios/scenario_unsupported_files.cpp)
