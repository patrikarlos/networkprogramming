/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2018, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/ 
/* <DESC>
 * Use getinfo to get content-type after completed transfer.
 * </DESC>
 */ 
#include <stdio.h>
#include <curl/curl.h>
#include <getopt.h>

size_t do_datastuff(void *buffer, size_t size, size_t nmemb, void *userp);
size_t do_headerstuff(void *buffer, size_t size, size_t nitems, void *userdata);

int visualize_data,visualize_header;

static size_t num_points = 0;
static const char* shortopt = "hd?";
static struct option longopt[]={
    {"header", no_argument , 0,'h'},
    {"data", no_argument , 0,'d'},
    {"help", no_argument , 0,'?'},
    {0,0,0,0}, /* sentinel */  
};

static const char* program_name;

static void show_usage(){
	printf("%s\n"
	       "usage: %s <-d> <-h> URL\n"
	       "\n"
	       "  -d, --data    Show payload.\n"
	       "  -h, --header  Show header.\n"
	       "  ?, --help     Show this.\n"
	       "\n", program_name, program_name);
}


int main(int argc, char *argv[])
{
  CURL *curl;
  CURLcode res;

  visualize_data=0;
  visualize_header=0;

  int last=argc;
  printf("There are %d args.\n",last);

  const char* separator = strrchr(argv[0], '/');
  if ( separator ){
    program_name = separator + 1;
  } else {
    program_name = argv[0];
  }
  

  int op, option_index;
  while ( (op=getopt_long(argc, argv, shortopt, longopt, &option_index)) != -1 ){
    switch (op){
    case 'd': /* --data */
      visualize_data=1;
      break;
      
    case 'h': /* --header */
      visualize_header=1;
      break;
      
    case '?': /* --help */
      show_usage();
      exit(0);
    }
  }
  
  if ( argc < 2 ){
    fprintf(stderr, "%s: need at least one input.\n", program_name);
    exit(1);
  }
  

  printf("cURLing: %s \n",argv[last-1]);
  
  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, argv[last-1]);
    //    curl_easy_setopt(curl, CURLOPT_HEADER, 0);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, do_datastuff);
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, do_headerstuff);    
    res = curl_easy_perform(curl);
 
    if(CURLE_OK == res) {
      char *ct;
      /* ask for the content-type */
      res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct);
      
      printf("Doit.\n");
      if((CURLE_OK == res) && ct){
        printf("We received Content-Type: %s\n", ct);
      } else {
	printf("Issues with curl.\n");
      }
    } else {
      printf("cURL_OK != res, its %s.\n",curl_easy_strerror(res));
    }
      
 
    /* always cleanup */ 
    curl_easy_cleanup(curl);
  }
  printf("Exiting..\n");
  return 0;
}

size_t do_datastuff(void *buffer, size_t size, size_t nmemb, void *userp){
  if(visualize_data){
    printf("Data => %s" , buffer);
  }
  return size*nmemb;
}


size_t do_headerstuff(void *buffer, size_t size, size_t nitems, void *userp){
  if(visualize_header){
    printf("Header => %s" , buffer);
  }
  return size*nitems;
}

