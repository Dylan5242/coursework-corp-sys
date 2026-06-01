#include <catch2/catch_test_macros.hpp>

#include "docgen/observer/GenerationSubject.h"

#include <vector>

using namespace docgen;

// Unit tests for observer registration and notification delivery.
class CollectingObserver final : public IGenerationObserver {
public:
    void onProgress(const std::string& message) override {
        messages.push_back(message);
    }

    std::vector<std::string> messages;
};

TEST_CASE("GenerationSubject can notify without observers", "[observer]") {
    GenerationSubject subject;
    REQUIRE_NOTHROW(subject.notify("message"));
}

TEST_CASE("GenerationSubject ignores null observers", "[observer]") {
    GenerationSubject subject;
    subject.addObserver(nullptr);
    REQUIRE_NOTHROW(subject.notify("message"));
}

TEST_CASE("GenerationSubject notifies one observer", "[observer]") {
    GenerationSubject subject;
    CollectingObserver observer;

    subject.addObserver(&observer);
    subject.notify("Scanning files...");

    REQUIRE(observer.messages.size() == 1);
    REQUIRE(observer.messages[0] == "Scanning files...");
}

TEST_CASE("GenerationSubject notifies several observers", "[observer]") {
    GenerationSubject subject;
    CollectingObserver first;
    CollectingObserver second;

    subject.addObserver(&first);
    subject.addObserver(&second);
    subject.notify("Done");

    REQUIRE(first.messages.size() == 1);
    REQUIRE(second.messages.size() == 1);
    REQUIRE(first.messages[0] == "Done");
    REQUIRE(second.messages[0] == "Done");
}

TEST_CASE("GenerationSubject keeps notification order", "[observer]") {
    GenerationSubject subject;
    CollectingObserver observer;

    subject.addObserver(&observer);
    subject.notify("first");
    subject.notify("second");
    subject.notify("third");

    REQUIRE(observer.messages.size() == 3);
    REQUIRE(observer.messages[0] == "first");
    REQUIRE(observer.messages[1] == "second");
    REQUIRE(observer.messages[2] == "third");
}

TEST_CASE("GenerationSubject allows the same observer to receive repeated messages", "[observer]") {
    GenerationSubject subject;
    CollectingObserver observer;

    subject.addObserver(&observer);
    subject.notify("step");
    subject.notify("step");

    REQUIRE(observer.messages.size() == 2);
    REQUIRE(observer.messages[0] == "step");
    REQUIRE(observer.messages[1] == "step");
}
