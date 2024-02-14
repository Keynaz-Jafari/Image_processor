# C_project

In this project, you can read an image from file, from the internet using a URL or you can email and image to it.

after reading the image, there are some filters you can apply on it and save the edited photo.

the image formats supported are: jpg,png,bmp

libraries used in this project: stb_library, libcurl, openssl, b64
command to build and run : gcc modifying_w_header.c bmp.c filters.c url_function.c email2_test.c b64/libb64-master/include/b64/cdecode.c -o modifying_w_header -lssl -lcrypto -lm -lcurl

