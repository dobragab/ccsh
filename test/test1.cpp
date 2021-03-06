#include <ccsh/ccsh.hpp>
#include <cstdio>
#include <fstream>
#include <string>

#include <gtest/gtest.h>

using namespace ccsh;

std::string ReadAllText(fs::path const& filename)
{
    std::ifstream t(filename.c_str());
    return std::string(std::istreambuf_iterator<char>(t),
                       std::istreambuf_iterator<char>());
}

TEST(NativeTest, CopyRemoveFile)
{
    fs::path example_path1{__FILE__};
    fs::path example_path2 = example_path1.parent_path() / "example1.file";

    std::string example_content1 = ReadAllText(example_path1);
    EXPECT_NE(example_content1, "");

    shell("rm", {example_path2.string()});
    EXPECT_EQ(fs::exists(example_path2), false);

    shell("cp", {example_path1.string(), example_path2.string()});
    EXPECT_EQ(fs::exists(example_path2), true);

    fs::path example_content2 = ReadAllText(example_path2);
    EXPECT_EQ(example_content1, example_content2);

    shell("rm", {example_path2.string()});
    EXPECT_EQ(fs::exists(example_path2), false);
}

