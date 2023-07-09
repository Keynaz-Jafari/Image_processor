#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>

#define h_addr h_addr_list[0]
void download_image_ssl(char *server, char *path, char *filename);