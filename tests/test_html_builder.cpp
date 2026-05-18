#include <catch2/catch_test_macros.hpp>

#include "docgen/generator/HtmlBuilder.h"

using namespace docgen;

TEST_CASE("HtmlBuilder creates document skeleton", "[generator][html_builder]") {
    HtmlBuilder builder;
    const auto html = builder.beginDocument("Title").endDocument().build();
    REQUIRE(html.find("<!DOCTYPE html>") != std::string::npos);
    REQUIRE(html.find("<title>Title</title>") != std::string::npos);
    REQUIRE(html.find("</html>") != std::string::npos);
}

TEST_CASE("HtmlBuilder escapes document title", "[generator][html_builder]") {
    HtmlBuilder builder;
    const auto html = builder.beginDocument("A < B & C").endDocument().build();
    REQUIRE(html.find("<title>A &lt; B &amp; C</title>") != std::string::npos);
}

TEST_CASE("HtmlBuilder adds heading with requested level", "[generator][html_builder]") {
    HtmlBuilder builder;
    const auto html = builder.beginDocument("x").addHeading("Section", 2).endDocument().build();
    REQUIRE(html.find("<h2>Section</h2>") != std::string::npos);
}

TEST_CASE("HtmlBuilder falls back to h1 for invalid heading level", "[generator][html_builder]") {
    HtmlBuilder builder;
    const auto html = builder.beginDocument("x").addHeading("Invalid", 9).endDocument().build();
    REQUIRE(html.find("<h1>Invalid</h1>") != std::string::npos);
}

TEST_CASE("HtmlBuilder adds escaped paragraph", "[generator][html_builder]") {
    HtmlBuilder builder;
    const auto html = builder.beginDocument("x").addParagraph("a < b").endDocument().build();
    REQUIRE(html.find("<p>a &lt; b</p>") != std::string::npos);
}

TEST_CASE("HtmlBuilder adds navigation for files", "[generator][html_builder]") {
    FileDocumentation file;
    file.fileName = "main.cpp";

    HtmlBuilder builder;
    const auto html = builder.beginDocument("x").addNavigation({file}).endDocument().build();
    REQUIRE(html.find("<nav>") != std::string::npos);
    REQUIRE(html.find("main.cpp") != std::string::npos);
}

TEST_CASE("HtmlBuilder escapes file names in navigation", "[generator][html_builder]") {
    FileDocumentation file;
    file.fileName = "a<b>.cpp";

    HtmlBuilder builder;
    const auto html = builder.beginDocument("x").addNavigation({file}).endDocument().build();
    REQUIRE(html.find("a&lt;b&gt;.cpp") != std::string::npos);
}

TEST_CASE("HtmlBuilder renders documented functions", "[generator][html_builder]") {
    FileDocumentation file;
    file.fileName = "math.cpp";
    file.functions.push_back({"add", "int add(int a, int b);", "Adds values"});

    HtmlBuilder builder;
    const auto html = builder.beginDocument("x").addFileDocumentation(file).endDocument().build();
    REQUIRE(html.find("Функции") != std::string::npos);
    REQUIRE(html.find("add") != std::string::npos);
    REQUIRE(html.find("Adds values") != std::string::npos);
}

TEST_CASE("HtmlBuilder renders documented classes", "[generator][html_builder]") {
    FileDocumentation file;
    file.fileName = "parser.h";
    file.classes.push_back({"Parser", "class Parser {", "Parser class"});

    HtmlBuilder builder;
    const auto html = builder.beginDocument("x").addFileDocumentation(file).endDocument().build();
    REQUIRE(html.find("Классы") != std::string::npos);
    REQUIRE(html.find("Parser") != std::string::npos);
    REQUIRE(html.find("Parser class") != std::string::npos);
}

TEST_CASE("HtmlBuilder renders message for files without documented entities", "[generator][html_builder]") {
    FileDocumentation file;
    file.fileName = "empty.cpp";

    HtmlBuilder builder;
    const auto html = builder.beginDocument("x").addFileDocumentation(file).endDocument().build();
    REQUIRE(html.find("Документированные классы и функции не найдены") != std::string::npos);
}

TEST_CASE("HtmlBuilder escapes function and class content", "[generator][html_builder]") {
    FileDocumentation file;
    file.fileName = "escape.cpp";
    file.functions.push_back({"less", "bool less(int a, int b);", "Returns a < b & true"});
    file.classes.push_back({"Box", "class Box<T> {", "Stores <T>"});

    HtmlBuilder builder;
    const auto html = builder.beginDocument("x").addFileDocumentation(file).endDocument().build();
    REQUIRE(html.find("a &lt; b &amp; true") != std::string::npos);
    REQUIRE(html.find("class Box&lt;T&gt; {") != std::string::npos);
}
