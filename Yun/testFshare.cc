#include <iostream>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <libgen.h>
#include <sys/socket.h>
#include "fshare.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

int gtest_argc;
char **gtest_argv;

using namespace testing;

class CLibraryInterface {
public:
  virtual ~CLibraryInterface() = default;
  virtual int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) = 0;
  virtual ssize_t send(int sockfd, const void* buf,  size_t len, int flags) = 0;
  virtual ssize_t recv(int sockfd, void* buf,  size_t len, int flags) = 0;
  virtual int mkdir(const char *pathname, mode_t mode) = 0;
};

class MockSocket : public CLibraryInterface{
  using CLibraryInterface::connect;
  using CLibraryInterface::send;
  using CLibraryInterface::recv;
  using CLibraryInterface::mkdir;
public:
  MOCK_METHOD(int, connect,(int sockfd, const struct sockaddr *addr, socklen_t addrlen),(override));
  MOCK_METHOD(ssize_t, send ,(int sockfd, const void* buf,  size_t len, int flags),(override));
  MOCK_METHOD(ssize_t, recv,(int sockfd, void* buf,  size_t len, int flags),(override));
  MOCK_METHOD(int, mkdir,(const char *pathname, mode_t mode),(override));
};

MockSocket *mocking;

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

class TestRequest : public testing::Test {
protected:
  char *input_buf;
  void SetUp() override {
    std::cout << "-------------SetUp---------------\n";
    if(gtest_argc < 2){
      std::cerr << "Please give test input file\n";
      exit(EXIT_FAILURE);
    }
    int fd = open(gtest_argv[1],O_RDONLY);
    struct stat st;
    if(stat(gtest_argv[1], &st) != 0){
      std::cerr << "Cannot find input file\n";
      exit(EXIT_FAILURE);
    }
    long long filesize = st.st_size;
    input_buf = (char *)malloc(filesize+1);
    int read_cnt;
    char *p = input_buf;
    int acc = 0;
    while(acc < filesize){
      read_cnt = read(fd,input_buf,filesize-acc);
      if(read_cnt == -1){
        std::cerr << "Read file error\n";
        exit(EXIT_FAILURE);
      }
      else if(read_cnt == 0){
        break;
      }
      p += read_cnt;
      acc += read_cnt;
    }
    input_buf[filesize] = '\0';
    std::cout << "Input File content : " << input_buf << "\n";

    mocking = new MockSocket();
    EXPECT_CALL(*mocking, send(_,_,_,_)).WillRepeatedly(Invoke([](int sockfd, const void *buf, size_t len, int flags) {
      std::cout << "This is mocking send\n";
      int write_len = write(sockfd,buf,len);
      return write_len;
    }));

    close(fd);
    std::cout << "---------------------------------\n";
  }
  void TearDown() override {
    std::cout << "-------------TearDown------------\n";
    free(input_buf);
    delete mocking;
    std::cout << "---------------------------------\n";
  }
};

TEST_F(TestRequest,TestSendBytes){
  int fd = open("testOutput.txt",O_CREAT|O_WRONLY|O_TRUNC,0644);
  int result = send_bytes(fd,input_buf,strlen(input_buf));
  EXPECT_EQ(result,0);
  close(fd);
  test_int++;
}

TEST_F(TestRequest, TestGetCmdCode){
  std::cout << "This is TestGetCmdCode : " << input_buf << " : " << strlen(input_buf) << "\n";
  EXPECT_EQ(get_cmd_code(input_buf),N_cmd);
}

TEST_F(TestRequest, TestGetOptionandRequest){
  std::cout << "This is TestGetOptionandRequest : " << input_buf << "(" << strlen(input_buf) << ")\n";
  int new_argc = 0;
  std::vector<std::string> tokens;
  char* token = std::strtok(input_buf, " ");
  while (token != nullptr) {
    tokens.push_back(token);
    new_argc++;
    token = std::strtok(nullptr, " ");
  }
  char *new_argv[new_argc];
  for (int i = 0; i < new_argc; ++i) {
    new_argv[i] = new char[tokens[i].size() + 1];
    std::strcpy(new_argv[i], tokens[i].c_str());
    std::cout << new_argv[i] << std::endl;
  }
  get_option(new_argc,new_argv);
  int fd = open("testOutput.txt",O_WRONLY|O_CREAT|O_TRUNC,0644);
  request(fd);
  close(fd);

  for (int i = 0; i < new_argc; ++i) {
    free(new_argv[i]);
  }
}

class TestResponse : public testing::Test {
protected:
  void SetUp() override {
    std::cout << "-------------SetUp---------------\n";
    if(gtest_argc < 2){
      std::cerr << "Please give test input file\n";
      exit(EXIT_FAILURE);
    }

    mocking = new MockSocket();
    
    EXPECT_CALL(*mocking, recv(_,_,_,_)).WillRepeatedly(Invoke([](int sockfd, const void *buf, size_t len, int flags) {
      std::cout << "This is mocking recv\n";
      int read_len = read(sockfd,(void*)buf,len);
      return read_len;
    }));
    std::cout << "make mocking object\n";
    std::cout << "---------------------------------\n";
  }
  void TearDown() override {
    std::cout << "-------------TearDown------------\n";
    delete mocking;
    std::cout << "close input file discriptor\ndelete mock object\n";
    std::cout << "---------------------------------\n";
  }
};

TEST_F(TestResponse,TestReceiveListResponse){
  // int text = open("./test_input/text.txt",O_RDONLY);
  // int input_fd = open("./test_input/testReceiveList2.txt",O_WRONLY|O_CREAT,0644);
  // server_header new_sh;
  // new_sh.is_error = 0;
  // new_sh.payload_size = 541;
  // write(input_fd,&new_sh,sizeof(new_sh));
  // char b;

  // while(read(text,&b,1) != 0){
  //   write(input_fd,&b,1);
  // }
  // close(input_fd);

  int fd = open(gtest_argv[1],O_RDONLY);
  receive_list_response(fd);
  close(fd);
}

TEST(TestGet,TestMakeDirectory){
  if(gtest_argc < 2){
    std::cerr << "Please give test input file\n";
    exit(EXIT_FAILURE);
  }
  char *input_buf;
  int fd = open(gtest_argv[1],O_RDONLY);
  struct stat st;
  if(stat(gtest_argv[1], &st) != 0){
    std::cerr << "Cannot find input file\n";
    exit(EXIT_FAILURE);
  }
  long long filesize = st.st_size;
  input_buf = (char *)malloc(filesize+1);
  int read_cnt;
  char *p = input_buf;
  int acc = 0;
  while(acc < filesize){
    read_cnt = read(fd,input_buf,filesize-acc);
    if(read_cnt == -1){
      std::cerr << "Read file error\n";
      exit(EXIT_FAILURE);
    }
    else if(read_cnt == 0){
      break;
    }
    p += read_cnt;
    acc += read_cnt;
  }
  input_buf[filesize] = '\0';
  std::cout << "Input File content : " << input_buf << "\n";

  mocking = new MockSocket();
  EXPECT_CALL(*mocking, mkdir(_,_)).WillRepeatedly(Invoke([](const char *pathname, mode_t mode) {
      std::cout << "This is mocking mkdir\n";
      return 0;
    }));
  make_directory(input_buf);

  char * parse = (char*)"";
  while(strcmp(parse, ".") != 0 && strcmp(parse, "/") != 0){
      parse = dirname(input_buf);
  }
  
  char *command = (char*)malloc(sizeof(char)* (8 + strlen(input_buf)) );
  memset(command, 0, sizeof(char)* (8 + strlen(input_buf)));
  strcat(command, (char*)"rm -rf ");
  strcat(command, input_buf);
  int result = -1;
  if(input_buf[0] != '/'){
      result = std::system(command);
  }
  if (result == 0) {
      std::cout << "Directory removed successfully.\n";
  } else {
      std::cerr << "Error removing directory.\n";
  }
  delete mocking;
  free(input_buf);
}

TEST_F(TestResponse,TestReceiveGetResponse){
  dest_dir = (char *) malloc(5);
  strcpy(dest_dir,"temp");
  file_path = (char *)malloc(10);
  strcpy(file_path,"server/he");

  int fd = open(gtest_argv[1],O_RDONLY);
  receive_get_response(fd);
  close(fd);

  char * parse = (char*)"";
  while(strcmp(parse, ".") != 0 && strcmp(parse, "/") != 0){
      parse = dirname(dest_dir);
  }
  
  char *command = (char*)malloc(sizeof(char)* (8 + strlen(dest_dir)) );
  memset(command, 0, sizeof(char)* (8 + strlen(dest_dir)));
  strcat(command, (char*)"rm -rf ");
  strcat(command, dest_dir);
  int result = -1;
  if(dest_dir[0] != '/'){
      result = std::system(command);
  }
  if (result == 0) {
      std::cout << "Directory removed successfully.\n";
  } else {
      std::cerr << "Error removing directory.\n";
  }

  free(dest_dir);
  free(file_path);
}

TEST_F(TestResponse,TestReceivePutResponse){
  file_path = (char *)malloc(10);
  strcpy(file_path,"server/he");
  int fd = open(gtest_argv[1],O_RDONLY);
  receive_put_response(fd);
  close(fd);
  free(file_path);
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  gtest_argc = argc;
  gtest_argv = argv;
  return RUN_ALL_TESTS();
}