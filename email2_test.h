#ifndef EMAIL_DOWNLOADER_H
#define EMAIL_DOWNLOADER_H

#include <stdio.h>

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream);

void download_email_attachment();

#endif
