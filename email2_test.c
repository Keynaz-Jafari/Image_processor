
#include <stdio.h>

#include <curl/curl.h>

#include <string.h>

#include "stdlib.h"

#include "b64/libb64-master/include/b64/cdecode.h"



// Callback function to write the fetched email data to a file

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)

{

   // printf("!!!!!!!!!!!!!!writing this : %s",(char *)ptr);

    return fwrite(ptr, size, nmemb, stream);

}



void download_email_attachment(char* email_img)


{

    CURL *curl;

    CURLcode res;

    FILE *file;

    const char *url = "imaps://imap.gmail.com/INBOX/";

    const char *userpwd = "melikaemial1@gmail.com:mnxccbmzxchgjtmn";

    const char *filename = "email2.txt";

    const char *fetchedFilename = "fetched.txt";

    const char *encodedFilename = "encoded2.txt";



    curl = curl_easy_init();

    if (curl)

    {

        // Set the URL and authentication credentials

        curl_easy_setopt(curl, CURLOPT_URL, url);

        curl_easy_setopt(curl, CURLOPT_USERPWD, userpwd);



        // Enable VERBOSE mode for detailed output

        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);



        // Perform the SEARCH command to retrieve a list of UIDs

        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "SEARCH ALL");

        curl_easy_setopt(curl, CURLOPT_NOBODY, 1L); // Avoid downloading the email data



        // Set the callback function to write data to a file

        file = fopen("email2.txt", "wb");

        if (file)

        {

            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

            curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);



            // Perform the request

            res = curl_easy_perform(curl);

            if (res != CURLE_OK)

            {

                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

            }



            // Close the file

            fclose(file);



            // Open the file for reading

            FILE *fp = fopen("email2.txt", "rb");

            if (fp)

            {

                // Find the latest UID from the email data

                int latestUID = -1;

                char line[256];

                while (fgets(line, sizeof(line), fp))

                {

                    // Assuming each line contains a single UID

                    if (line[0] == '*' && line[1] == ' ')

                    {

                        char *uidStr = strtok(line + 2, " \t\r\n");

                        while (uidStr)

                        {

                            int uid = atoi(uidStr);

                            if (uid > latestUID)

                            {

                                latestUID = uid;

                            }

                            uidStr = strtok(NULL, " \t\r\n");

                        }

                    }

                }

                fclose(fp);



                if (latestUID != -1)

                {

                    printf("Latest UID found: %d\n", latestUID);



                    // Construct the URL with the latest UID

                    char latestURL[256];

                    snprintf(latestURL, sizeof(latestURL), "%s;UID=%d", url, latestUID);

                    



                    // Set the callback function to write data to a file

                    FILE * myfile = fopen("fetched.txt", "wb");

                    if (myfile)

                    {

                        curl_easy_cleanup(curl);

                        curl = curl_easy_init();

                        if(!curl){

                            //printf("here!");

                        }

                        curl_easy_setopt(curl, CURLOPT_URL, latestURL);

                        curl_easy_setopt(curl, CURLOPT_USERNAME,"melikaemial1@gmail.com");

                        curl_easy_setopt(curl, CURLOPT_PASSWORD,"mnxccbmzxchgjtmn");

                        curl_easy_setopt(curl, CURLOPT_USE_SSL,CURLUSESSL_ALL);

                        curl_easy_setopt(curl, CURLOPT_WRITEDATA, myfile);

                        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

                        



                        // Perform the request

                        res = curl_easy_perform(curl);

                        if (res != CURLE_OK)

                        {

                            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

                        }



                        // Close the file

                        fclose(myfile);



                        // Open the fetched file for reading

                        FILE *fetchedFile = fopen(fetchedFilename, "r");

                        if (fetchedFile)

                        {

                            // Find the start of the image data

                            fseek(fetchedFile, 0L, SEEK_END);

                            int fetchedFileSize = ftell(fetchedFile);

                            fseek(fetchedFile, 0L, SEEK_SET);

                            char curchar;

                            int i;

                            for (i = 0; i < fetchedFileSize; i++)

                            {

                                fread(&curchar, sizeof(char), 1, fetchedFile);

                                if (curchar == 'X')

                                {

                                    fread(&curchar, sizeof(char), 1, fetchedFile);

                                    if (curchar == '-')

                                    {

                                        fread(&curchar, sizeof(char), 1, fetchedFile);

                                        if (curchar == 'A')

                                        {

                                            fread(&curchar, sizeof(char), 1, fetchedFile);

                                            if (curchar == 't')

                                            {

                                    

                                            break;

                                            }

                                        }

                                    }

                                }

                            }



                            int j;

                            int write = 0;

                            for (j = i; j < fetchedFileSize; j++)

                            {

                                fread(&curchar, sizeof(char), 1, fetchedFile);

                                if (curchar == '\r')

                                {

                                    fread(&curchar, sizeof(char), 1, fetchedFile);

                                    if (curchar == '\n')

                                    {

                                        write = 1;

                                        break;

                                    }

                                }

                            }



                            if (write)

                            {

                                FILE *fpEncoded = fopen(encodedFilename, "wb");

                                //fseek(fetchedFile, j + 1, SEEK_SET); // Move the file position indicator to j+1
                                fread(&curchar,sizeof(char),1,fetchedFile);
                                fread(&curchar,sizeof(char),1,fetchedFile);
                                j+=2;
                                j=ftell(fetchedFile);

                                while ((fread(&curchar, sizeof(char), 1, fetchedFile) == 1) && (j < fetchedFileSize-36))

                                {

                                    fwrite(&curchar, sizeof(char), 1, fpEncoded);

                                    j++;

                                }

                                fclose(fpEncoded);



                                // Open the encoded file for reading

                                FILE *fpEncodedRead = fopen(encodedFilename, "r");

                                if (fpEncodedRead)

                                {

                                    // Calculate the size of the encoded image data

                                    fseek(fpEncodedRead, 0L, SEEK_END);

                                    int encodedFileSize = ftell(fpEncodedRead);

                                    fseek(fpEncodedRead, 0L, SEEK_SET);



                                    // Initialize the base64 decoder

                                    base64_decodestate state;

                                    base64_init_decodestate(&state);



                                    // Create a buffer to hold the encoded data

                                    char encoded_data_read[encodedFileSize];

                                    fread(encoded_data_read, sizeof(char), encodedFileSize, fpEncodedRead);



                                    // Open the output file for writing the decoded image

                                    FILE *fp2 = fopen(email_img, "wb");

                                    if (fp2)

                                    {

                                        // Calculate the size of the decoded image data

                                        int decoded_size = (encodedFileSize / 4) * 3;

                                        unsigned char decoded_data[decoded_size];



                                        // Decode the image data

                                        int num_bytes_decoded = base64_decode_block(encoded_data_read, encodedFileSize, decoded_data, &state);



                                        // Write the decoded image data to the output file

                                        fwrite(decoded_data, sizeof(unsigned char), num_bytes_decoded, fp2);



                                        // Close the output file

                                        fclose(fp2);

                                    }

                                    else

                                    {

                                        fprintf(stderr, "Failed to open output file\n");

                                    }



                                    // Close the encoded file

                                    fclose(fpEncodedRead);

                                }

                                else

                                {

                                    fprintf(stderr, "Failed to open file %s\n", encodedFilename);

                                }

                            }

                            else

                            {

                                fprintf(stderr, "Failed to find the start of image data\n");

                            }



                            // Close the fetched file

                            fclose(fetchedFile);

                        }

                        else

                        {

                            fprintf(stderr, "Failed to open file %s\n", fetchedFilename);

                        }

                    }

                    else

                    {

                        fprintf(stderr, "Failed to open file %s\n", fetchedFilename);

                    }

                }

                else

                {

                    fprintf(stderr, "No UIDs found in the email data\n");

                }

            }

            else

            {

                fprintf(stderr, "Failed to open file %s\n", filename);

            }

        }

        else

        {

            fprintf(stderr, "Failed to open file %s\n", filename);

        }



        // Cleanup

        curl_easy_cleanup(curl);

    }

    else

    {

        fprintf(stderr, "Failed to initialize curl\n");

    }

}


