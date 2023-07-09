#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h> //For internet address 
#include <netdb.h> //For network database operations
#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>

#define h_addr h_addr_list[0]

void download_image_ssl(char *server, char *path, char *filename) {
    int sockfd;
    struct sockaddr_in serv_addr;
    struct hostent *host;

    SSL_library_init(); // initialize ssl library 
    SSL_CTX *ctx = SSL_CTX_new(TLS_client_method()); //SSL context (SSL_CTX) is created using SSL_CTX_new(TLS_client_method()). This context will hold SSL configuration options.

    if (ctx == NULL) {
        printf("Error creating SSL context\n");
        return;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);  //TCP socket is created 

    if (sockfd < 0) {
        printf("Error opening socket\n");
        return;
    }

    host = gethostbyname(server); //The server hostname is resolved to an IP address 

    if (host == NULL) {
        printf("Error resolving server hostname\n");
        return;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;//The server address structure (serv_addr) is initialized with the resolved IP address and port (443 for HTTPS)
    serv_addr.sin_port = htons(443); 
    memcpy(&serv_addr.sin_addr.s_addr, host->h_addr, host->h_length);

    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Error connecting to server\n");
        return;
    }

    SSL *ssl = SSL_new(ctx); // ssl object create mishe
    BIO *sbio = BIO_new_socket(sockfd, BIO_NOCLOSE); // bio object create mishe ke associates the SSL object with the socket
    SSL_set_bio(ssl, sbio, sbio); //SSL object is configured to use the BIO objects

    if (SSL_connect(ssl) <= 0) {
        printf("Error establishing SSL connection\n");
        return;
    }

    char request[1024];
    sprintf(request, "GET %s HTTP/1.0\r\nHost: %s\r\n\r\n", path, server); // 1.1 bood     GET request
    SSL_write(ssl, request, strlen(request));

    char response[1024];
    int bytes_read = SSL_read(ssl, response, sizeof(response)); //server's response
    int content_length = -1;

    if (bytes_read > 0) { //If any bytes are read in the response, the content length of the image file is extracted from the response header
        char *content_length_start = strstr(response, "Content-Length: ");
        if (content_length_start != NULL) {
            content_length_start += strlen("Content-Length: ");
            char *content_length_end = strstr(content_length_start, "\r\n");
            if (content_length_end != NULL) {
                char length_str[16];
                strncpy(length_str, content_length_start, content_length_end - content_length_start);
                length_str[content_length_end - content_length_start] = '\0';
                content_length = atoi(length_str);
            }
        }

        char *body_start = strstr(response, "\r\n\r\n");
        if (body_start != NULL) {
            body_start += strlen("\r\n\r\n");

            FILE *fp = fopen(filename, "wb"); //If the body is found, the image data is written to a file 
            if (fp != NULL) {
                fwrite(body_start, 1, bytes_read - (body_start - response), fp);
                while ((bytes_read = SSL_read(ssl, response, sizeof(response))) > 0) {
    fwrite(response, 1, bytes_read, fp);
}

                fclose(fp);
            }
        }
    }

    SSL_shutdown(ssl);
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    close(sockfd);
}


