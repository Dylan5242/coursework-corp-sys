cmake_minimum_required(VERSION 3.10)

project(DocGenLite VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCES
    src/main.cpp
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

add_executable(docgen_lite ${SOURCES})

target_include_directories(docgen_lite PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
