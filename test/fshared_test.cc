// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <pthread.h>
// #include <fcntl.h>
// #include <dirent.h>
// #include <sys/stat.h>
// #include <libgen.h>
// #include <errno.h>
#include <iostream>

#include "../libFshared/fshared.h"
#include "gtest/gtest.h"


TEST(DIR_CHECK_TEST, DIR_CHECK_TEST)
{
    const int MAX_PATH = 4096;
    char input[MAX_PATH];
    std::cin.getline(input, MAX_PATH);

    EXPECT_EQ(0, directory_check(input));
}

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}