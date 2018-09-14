/* 
sauce.hpp
version 0.0.1
https://github.com/rfetree/
Copyright (c) 2018 rfetree <https://github.com/rfetree>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
-----------------------------------------
json.hpp copyright notice
-----------------------------------------

 __|  |   __|     |   | |  JSON for Modern C++
|  |  |__   |  |  | | | |  version 3.2.0
|_____|_____|_____|_|___|  https://github.com/nlohmann/json
Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2013-2018 Niels Lohmann <http://nlohmann.me>.
Permission is hereby  granted, free of charge, to any  person obtaining a copy
of this software and associated  documentation files (the "Software"), to deal
in the Software  without restriction, including without  limitation the rights
to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
--------------------------------------------------------
cURL copyright notice
--------------------------------------------------------

COPYRIGHT AND PERMISSION NOTICE

Copyright (c) 1996 - 2018, Daniel Stenberg, daniel@haxx.se, and many contributors, see the THANKS file.

All rights reserved.

Permission to use, copy, modify, and distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF THIRD PARTY RIGHTS. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of a copyright holder shall not be used in advertising or otherwise to promote the sale, use or other dealings in this Software without prior written authorization of the copyright holder.

*/

#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <curl/curl.h>
#include "json.hpp"
namespace sauce
{
    class sauceResult
    {
        public:
            std::vector<std::string> sourcelinks;
            double similarity;
            void init(std::vector<std::string> ext_urls,std::string d)
            {
                similarity = std::stod(d);
                sourcelinks = ext_urls;
            }
    };
    class sauceMachine
    {
    private:
        std::string api_key;
        std::string file_path;
        std::string file_name;
        std::string output_file_name = "saucenao.json";
        std::string my_target_url_format = "https://saucenao.com/search.php?output_type=2&api_key=";
        std::string my_target_url = my_target_url_format;

        std::string extract_file_name(std::string x)
        {
          std::string tmp = "";
          for(int i=x.length()-1;i>=0;i--)
          {
            #ifdef _WIN32
            if(x[i]=='\\')
            {
              return tmp;
            }

            #else
            if(x[i]=='/')
            {
              return tmp;
            }            
            tmp = x[i] + tmp;
            #endif
          }
        }
    public:
        sauceMachine(std::string param_api_key)
        {
            api_key = param_api_key;
            my_target_url = my_target_url_format+api_key;
        }
        void set_file_path(std::string param_file_path)
        {
          file_path = param_file_path;
          file_name = extract_file_name(file_path);
          output_file_name = "saucenao-"+file_name+".json";
        }
        std::string get_output_file_name(void)
        {
          return output_file_name;
        }
        void fetch_json(void)
        {
          std::string str_input_file_name = file_path;
          std::string str_target_url = "http://saucenao.com/search.php?output_type=2&api_key="+api_key;
          std::string str_output_file_name = output_file_name;
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
          curl_easy_setopt(curl,CURLOPT_VERBOSE,1L);
          curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
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
            {
              std::fprintf(stderr, "curl_easy_perform() failed: %s\n",
                      curl_easy_strerror(res));
            }
         
            curl_easy_cleanup(curl);
            curl_mime_free(form);
            curl_slist_free_all(headerlist);
          }
          curl_global_cleanup();
          fclose(file);
        }
        nlohmann::json get_json(void)
        {
          std::ifstream raw_file(output_file_name.c_str());
          nlohmann::json json_obj;
          raw_file >> json_obj;
          return json_obj;
        }     

        std::vector<sauceResult> get_sauce_res(void)
        {
            std::ifstream raw_file(output_file_name.c_str());
            nlohmann::json json_obj;
            raw_file >> json_obj;
            auto x = json_obj["results"];
            std::vector<sauceResult> rvec;
            for(auto iter = x.begin();iter!=x.end();++iter)
            {
                sauceResult sr;
                auto curr = *iter;
                std::string similarity = curr["header"]["similarity"];
                auto ext_urls = curr["data"]["ext_urls"];
                std::vector<std::string> v;
                for(auto iter_lv2 = ext_urls.begin();iter_lv2!=ext_urls.end();++iter_lv2)
                {
                    std::string xstr = *iter_lv2;
                    v.push_back(xstr);
                }
                sr.init(v,similarity);
                rvec.push_back(sr);
            }
            return rvec;
        }
    };
}