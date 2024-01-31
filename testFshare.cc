#include <stdio.h>
#include <stdlib.h>
#include "fshared.h"
#include "gtest/gtest.h"

int gtest_argc;
char **gtest_argv;

class TestSendBytes : public testing::Test {
protected:
  char *path;
  int fd;
  char *input_buf;
  void SetUp() override {
    if(gtest_argc < 2){
      std::cerr << "Please give test input file\n";
      exit(EXIT_FAILURE);
    }
    int fd = open(gtest_argv[1]);
    struct stat st;
    if(stat(gtest_argv[1], &st) != 0){
      std::cerr << "Cannot find input file\n";
      exit(EXIT_FAILURE);
    }
    char *input_buf = (char *)
  }
  void TearDown() override {
    free(path);
    close(fd);
    free(input_buf);
  }
};

TEST_F(TestWithFile, testSendbytes)
{
  EXPECT_EXIT(send_bytes(), testing::ExitedWithCode(0), "Success")
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  gtest_argc = argc;
  gtest_argv = argv;
  return RUN_ALL_TESTS();
}