#include <cstdio>
#include <string>
#include <curl/curl.h>
std::string api_key = "YOUR API KEY HERE";
void fetch_sauce_and_write(std::string str_input_file_name)
{

  std::string str_target_url = "http://saucenao.com/search.php?output_type=2&api_key="+api_key;
  std::string str_output_file_name = "lmao.json";
  const char * input_file_name = str_input_file_name.c_str();
  const char * target_url = str_target_url.c_str();
  const char * output_file_name = str_output_file_name.c_str();
  CURL *curl;
  CURLcode res;
 
  curl_mime *form = NULL;
  curl_mimepart *field = NULL;
  struct curl_slist *headerlist = NULL;
  static const char buf[] = "Expect:";
 
  curl_global_init(CURL_GLOBAL_ALL);
 
  curl = curl_easy_init();
  FILE * file = fopen(output_file_name, "w");
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, file) ;
  if(curl) {
    form = curl_mime_init(curl);
    field = curl_mime_addpart(form);
    curl_mime_name(field, "file");
    curl_mime_filedata(field, input_file_name);  
    field = curl_mime_addpart(form);
    curl_mime_name(field, "submit");
    curl_mime_data(field, "send", CURL_ZERO_TERMINATED);
    curl_easy_setopt(curl, CURLOPT_URL, target_url);
    headerlist = curl_slist_append(headerlist, buf);
  	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
    curl_easy_setopt(curl, CURLOPT_MIMEPOST, form);
    res = curl_easy_perform(curl);
    if(res != CURLE_OK)
      std::fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
 
    curl_easy_cleanup(curl);
    curl_mime_free(form);
    curl_slist_free_all(headerlist);
  }
}

int main(void)
{
  std::string myfile = "/path/to/file.png";
  fetch_sauce_and_write(myfile);
  return 0;
}