#include <iostream>

#include "../libFshared/fshared.h"
#include "gtest/gtest.h"
#include <limits.h>
#include <cstring>
#include <sys/socket.h> 
#include "gmock/gmock.h"
#include <fcntl.h>
#include <unistd.h>

char ** pass_argv;
int pass_argc = 0;
//sang
using namespace testing;
using ::testing::Return;
using ::testing::SetArgReferee;
using ::testing::_;


class MockClient {
    public:
        virtual ~MockClient() {}
        virtual int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) = 0;
        virtual ssize_t send(int sockfd, const void* buf,  size_t len, int flags) = 0;
        virtual ssize_t recv(int sockfd, void* buf,  size_t len, int flags) = 0;
        // virtual void put_response(int conn) = 0;
        // virtual void get_response(int conn) = 0;
        // virtual void list_response(char * filepath, const int conn)= 0;
};

class Mocking: public MockClient{
    using MockClient::connect;
    using MockClient::send;
    using MockClient::recv;
    // using MockClient::put_response;
    // using MockClient::get_response;
    // using MockClient::list_response;

  public:
    MOCK_METHOD(int, connect,(int sockfd, const struct sockaddr *addr, socklen_t addrlen),(override));
    MOCK_METHOD(ssize_t, send ,(int sockfd, const void* buf,  size_t len, int flags),(override));
    MOCK_METHOD(ssize_t, recv,(int sockfd, void* buf,  size_t len, int flags),(override));
    // MOCK_METHOD(void, put_response,(int conn),(override));
    // MOCK_METHOD(void, get_response,(int conn),(override));
    // MOCK_METHOD(void, list_response,(char * filepath, int conn),(override));

};

Mocking *mocking;

int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    return mocking->connect(sockfd, addr, addrlen);
}
ssize_t send(int sockfd, const void* buf,  size_t len, int flags)
{
    return mocking->send(sockfd,buf,len,flags);
}
ssize_t recv(int sockfd, void* buf,  size_t len, int flags)
{
    return mocking->recv(sockfd,buf,len,flags);
}

// void put_response(int conn)
// {
//     return mocking->put_response(conn);
// }
// void get_response(int conn)
// {
//     return mocking->get_response(conn);
// }
// void list_response(char * filepath, const int conn)
// {
//     return mocking->list_response(filepath, conn);
// }

TEST(DIR_CHECK_TEST, DIR_CHECK_INPUT)
{
    const int MAX_PATH = 4096;
    char input[MAX_PATH];
    std::cin.getline(input, MAX_PATH);

    EXPECT_EQ(0, directory_check(input));
}

TEST(GET_OPTION_TEST, GET_OPTION_INPUT){
    const int MAX_ARG = 4096;
    char input[MAX_ARG];
    std::cin.getline(input, MAX_ARG);

    std::istringstream iss(input);
    int initialCapacity = 100;
    char** words = (char**)malloc(initialCapacity * sizeof(char*));
    int wordCount = 0;

    std::string word;
    while (iss >> word) {
        if (wordCount >= initialCapacity) {
            words = (char**)realloc(words, 2 * initialCapacity * sizeof(char*));
            initialCapacity *= 2;
        }

        words[wordCount] = strdup(word.c_str());
        wordCount++;
    }
   
    EXPECT_NO_THROW(get_option(wordCount, words)); 
    free(words);
}

TEST(LIST_RESPONSE_TEST, LIST_RESPONSE_INPUT)
{
    //file path standard input으로 받기
    const int MAX_PATH = 4096;
    char input[MAX_PATH];
    const int conn = 1;
    std::cin.getline(input, MAX_PATH);
    // int output_fd = open("./test_output/list_response_output", O_WRONLY|O_CREAT, 0777);
    
    mocking = new Mocking();

    EXPECT_CALL(*mocking, send(_,_,_,_)).WillRepeatedly(Invoke([](int sockfd, const void *buf, size_t len, int flags) {
            // std::cout << "10" << std::endl;  // Print 10 to stdout
            return len;  // Call the original send function
        }));

    EXPECT_NO_THROW(list_response(input, conn));
    delete mocking;
}


// TEST(GO_THREAD, GO_THREAD_INPUT){
//     //올바른 command가 온다고 보장해야함.  
//     //올바른 port num이 온다고 보장해야함. 
//     int * conn = (int *) malloc(sizeof(int));
//     *conn = 1;

//     int input[4];
//     bool is_valid_input = true;
//     for(int i=0; i<4; i++){
//         std::string input_str;
//         std::getline(std::cin, input_str);
        
//         std::stringstream ss(input_str);    

//         if (!(ss >> input[i])) {
//             is_valid_input = false;
//             break;
//         } 
//         std::cout << input[i] << std::endl;
//     }

//     if(input[0] < 0 || input[0] > 2){
//         is_valid_input = false;
//     }

//     if(is_valid_input == false){ // 올바른 Input 값이 아니라면 false로 test 종료
//         ASSERT_TRUE(is_valid_input);
//     }

//     // ch.command = input[0];
//     ch.src_path_len = input[1];
//     ch.des_path_len = input[2];
//     ch.des_path_len = input[3];

//     mocking = new Mocking();

//     EXPECT_CALL(*mocking, recv(_,_,_,_)).WillRepeatedly(Invoke([](int sockfd, const void *buf, size_t len, int flags) {
            
//             return len;  // Call the original send function
//         }));

//     EXPECT_CALL(*mocking, list_response(_,_)).WillRepeatedly(Invoke([](char * file_path, int conn) {
//         std::cout << "!!!!!!!list_response!!!!!!" << std::endl;  // Print 10 to stdout
//         return ;  // Call the original send function
//     }));
    
//     EXPECT_CALL(*mocking, put_response(_)).WillRepeatedly(Invoke([](int conn) {
//         std::cout << "put_response" << std::endl;  // Print 10 to stdout
//         return ;  // Call the original send function
//     }));
//     EXPECT_CALL(*mocking, get_response(_)).WillRepeatedly(Invoke([](int conn) {
//         std::cout << "get_response" << std::endl;  // Print 10 to stdout
//         // return ;  // Call the original send function
//     }));

//     EXPECT_NO_THROW(go_thread(conn));

//     delete mocking;
// }

TEST(GET_RESPONSE, GET_RESPONSE_INPUT){
    //input을 argument로 주깅
    // if(pass_argc < 2) return;
    std::string input_str;
    std::getline(std::cin, input_str);

    // ch.payload_size = strlen(pass_argv[1]);

    mocking = new Mocking ();

    EXPECT_CALL(*mocking, recv(_,_,_,_)).WillRepeatedly(Invoke([&input_str](int sockfd, const void *buf, size_t len, int flags) {
            // std::cout << "Im mocking recv\n";
            recv_payload = strdup(input_str.c_str());
            // recv_payload = pass_argv[1];
            return len;  // Call the original send function
        }));
    
    EXPECT_CALL(*mocking, send(_,_,_,_)).WillRepeatedly(Invoke([](int sockfd, const void *buf, size_t len, int flags) {
            // std::cout << "Im mocking send\n";
            return len;  // Call the original send function
        }));
    int conn = 1;
    
    EXPECT_NO_THROW(get_response(conn));

    delete mocking;
}

int main(int argc, char **argv){
    pass_argc = argc;
    pass_argv = argv;
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}