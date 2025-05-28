#include "gtest/gtest.h"
#include "../src/utils/StringUtils.h"
#include <string>
#include <vector>

TEST(StringUtilsTest, Trim) {
    // Verify that leading and trailing whitespace is removed
    ASSERT_EQ(StringUtils::trim("   hello   "), "hello");

    // Verify that whitespace within the string is not removed
    ASSERT_EQ(StringUtils::trim("  hello world  "), "hello world");

    // Verify that the function works correctly with empty strings
    ASSERT_EQ(StringUtils::trim(""), "");

    // Verify that the function works correctly with strings containing only whitespace
    ASSERT_EQ(StringUtils::trim("   "), "");
}

TEST(StringUtilsTest, ToLower) {
    // Verify that uppercase characters are converted to lowercase
    ASSERT_EQ(StringUtils::toLower("Hello World"), "hello world");

    // Verify that lowercase characters are not changed
    ASSERT_EQ(StringUtils::toLower("hello world"), "hello world");

    // Verify that non-alphabetic characters are not changed
    ASSERT_EQ(StringUtils::toLower("Hello 123 World!"), "hello 123 world!");

    // Verify that the function works correctly with empty strings
    ASSERT_EQ(StringUtils::toLower(""), "");
}

TEST(StringUtilsTest, ToUpper) {
    // Verify that lowercase characters are converted to uppercase
    ASSERT_EQ(StringUtils::toUpper("Hello World"), "HELLO WORLD");

    // Verify that uppercase characters are not changed
    ASSERT_EQ(StringUtils::toUpper("HELLO WORLD"), "HELLO WORLD");

    // Verify that non-alphabetic characters are not changed
    ASSERT_EQ(StringUtils::toUpper("Hello 123 World!"), "HELLO 123 WORLD!");

    // Verify that the function works correctly with empty strings
    ASSERT_EQ(StringUtils::toUpper(""), "");
}

TEST(StringUtilsTest, Split) {
    // Verify that the string is split correctly into a vector of substrings
    std::vector<std::string> expected = {"hello", "world"};
    ASSERT_EQ(StringUtils::split("hello,world", ','), expected);

    // Verify that the function works correctly with empty strings
    std::vector<std::string> expectedEmpty = {""};
    ASSERT_EQ(StringUtils::split("", ','), expectedEmpty);

    // Verify that the function works correctly with delimiters at the beginning, end, and multiple consecutive delimiters
    std::vector<std::string> expectedDelimiters = {"", "hello", "", "world", ""};
    ASSERT_EQ(StringUtils::split(",hello,,world,", ','), (std::vector<std::string>({"", "hello", "", "world", ""})));
}

TEST(StringUtilsTest, Join) {
    // Verify that the vector of strings is joined correctly into a single string
    std::vector<std::string> elements = {"hello", "world"};
    ASSERT_EQ(StringUtils::join(elements, ','), "hello,world");

    // Verify that the function works correctly with an empty vector
    std::vector<std::string> emptyElements = {};
    ASSERT_EQ(StringUtils::join(emptyElements, ','), "");

    // Verify that the function works correctly with a single-element vector
    std::vector<std::string> singleElement = {"hello"};
    ASSERT_EQ(StringUtils::join(singleElement, ','), "hello");
}

TEST(StringUtilsTest, StartsWith) {
    // Verify that the function returns true when the string starts with the prefix
    ASSERT_TRUE(StringUtils::startsWith("hello world", "hello"));

    // Verify that the function returns false when the string does not start with the prefix
    ASSERT_FALSE(StringUtils::startsWith("hello world", "world"));

    // Verify that the function works correctly with empty strings and prefixes
    ASSERT_TRUE(StringUtils::startsWith("", ""));
    ASSERT_FALSE(StringUtils::startsWith("hello", ""));
    ASSERT_FALSE(StringUtils::startsWith("", "hello"));
}

TEST(StringUtilsTest, EndsWith) {
    // Verify that the function returns true when the string ends with the suffix
    ASSERT_TRUE(StringUtils::endsWith("hello world", "world"));

    // Verify that the function returns false when the string does not end with the suffix
    ASSERT_FALSE(StringUtils::endsWith("hello world", "hello"));

    // Verify that the function works correctly with empty strings and suffixes
    ASSERT_TRUE(StringUtils::endsWith("", ""));
    ASSERT_FALSE(StringUtils::endsWith("hello", ""));
    ASSERT_FALSE(StringUtils::endsWith("", "hello"));
}

TEST(StringUtilsTest, Contains) {
    // Verify that the function returns true when the string contains the substring
    ASSERT_TRUE(StringUtils::contains("hello world", "world"));
    ASSERT_TRUE(StringUtils::contains("hello world", "o w"));

    // Verify that the function returns false when the string does not contain the substring
    ASSERT_FALSE(StringUtils::contains("hello world", "abc"));

    // Verify that the function works correctly with empty strings and substrings
    ASSERT_TRUE(StringUtils::contains("", ""));
    ASSERT_FALSE(StringUtils::contains("hello", ""));
    ASSERT_FALSE(StringUtils::contains("", "hello"));
}