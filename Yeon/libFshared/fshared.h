#ifndef GOOGLE_TEST_SUM_H
#define GOOGLE_TEST_SUM_H

#ifdef __cplusplus
extern "C"{
#endif
typedef enum {
    list,
    get,
    put,
    N_cmd
} cmd ;

extern char * cmd_str[N_cmd];

typedef struct {
    cmd command ;
    int src_path_len ;
    int des_path_len ;
    int payload_size ;
} client_header ;

typedef struct {
    int is_error ; // on success 0, on error 1
    int payload_size ;
} server_header ;

extern char * recv_payload;
extern char * send_payload;

extern client_header ch ;
extern server_header sh ;

extern int port_num ;
extern char * server_dir;

extern const int buf_size;

int send_bytes(int fd, char * buf, size_t len);

int directory_check(char * filepath);

void print_usage();

void get_option(int argc, char * argv[]);

void list_response(char * filepath, const int conn);

char * parse_directory(char * toparse);

void get_response(int conn);

void make_directory(char * towrite);

void put_response(int conn);

void * go_thread(void * arg);


#ifdef __cplusplus
}
#endif

#endif