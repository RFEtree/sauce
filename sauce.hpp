/* 
-----------------------------------------
copyright notice
-----------------------------------------
sauce.hpp
version 0.0.1
https://github.com/rfetree/sauce
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

--------------------------------------------------------
pugixml copyright notice
--------------------------------------------------------
MIT License

Copyright (c) 2006-2018 Arseny Kapoulkine

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "pugixml.hpp"
#include "pugixml.cpp"
#include <iostream>
#include <set>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <string>
#include <curl/curl.h>
#include "json.hpp"
namespace sauce
{
  //-------------------------------------
  //-------------------------------------
  //-------------------------------------
  class danbooruMech
  {
  private:
      std::string fn;
      std::string target_url = "https://danbooru.donmai.us/posts/";
  public:
      std::string id_num;
      bool file_exists = false;
      danbooruMech(std::string param_id_num)
      {
          id_num = param_id_num;
          target_url+=id_num+".json";
          fn = id_num + "-danbooru.json";
          std::ifstream ftest(fn.c_str());
          file_exists = ftest.good();
      }

      void fetch(void)
      {
        if(file_exists==false)
        {          
          const char * my_fn = fn.c_str();
          const char * my_url = target_url.c_str();
          CURL * easyhandle;
          CURLcode res;
          easyhandle = curl_easy_init();
          curl_easy_setopt(easyhandle,CURLOPT_VERBOSE,1L);
          curl_easy_setopt(easyhandle,CURLOPT_URL, my_url);
          FILE * fp = fopen(my_fn,"w");
          curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, fp);
          res = curl_easy_perform(easyhandle);
          curl_easy_cleanup(easyhandle);
          fclose(fp);
          file_exists = true;
        }
        else
        {
          std::cout<<"file "+fn+" exists"<<std::endl;
        }
      }
      std::string get_tags(void)
      {
        std::ifstream raw_file(fn.c_str());
        nlohmann::json json_obj;
        raw_file>>json_obj;
        std::string ret = json_obj["tag_string"];
        return ret;
      }
  };  
  //-------------------------------------
  //-------------------------------------
  //-------------------------------------
  class gelbooruMech
  {
  private:
      std::string fn;
      std::string target_url = "https://gelbooru.com/index.php?page=dapi&s=post&q=index&id=";
  public:
      std::string id_num;
      bool file_exists = false;
      gelbooruMech(std::string param_id_num)
      {
          id_num = param_id_num;
          target_url+=id_num;
          fn = id_num + "-gelbooru.xml";
          std::ifstream ftest(fn.c_str());
          file_exists = ftest.good();
      }

      void fetch(void)
      {
        if(file_exists==false)
        {          
          const char * my_fn = fn.c_str();
          const char * my_url = target_url.c_str();
          CURL * easyhandle;
          CURLcode res;
          easyhandle = curl_easy_init();
          curl_easy_setopt(easyhandle,CURLOPT_VERBOSE,1L);
          curl_easy_setopt(easyhandle,CURLOPT_URL, my_url);
          FILE * fp = fopen(my_fn,"w");
          curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, fp);
          res = curl_easy_perform(easyhandle);
          curl_easy_cleanup(easyhandle);
          fclose(fp);
          file_exists = true;
        }
        else
        {
          std::cout<<"file "+fn+" exists"<<std::endl;
        }
      }
      std::string get_tags(void)
      {
          pugi::xml_document doc;
          doc.load_file(fn.c_str());
          auto my_attrib = doc.child("posts").child("post").attribute("tags");
          std::string ret = my_attrib.value();
          return ret;
      }
  };  
  //-------------------------------------
  //-------------------------------------
  //-------------------------------------
    class sauceResult
    {
        public:
          std::set<std::string> gelbooru_tag_set;
          std::set<std::string> danbooru_tag_set;
          std::string gelbooru_id = "";
          std::string danbooru_id = "";
          void gelbooru_tags(std::string x)
          {
            std::string xtag = "";
            for(int i=0;i<x.length();i++)
            {
              if(x[i]==' ')
              {
                if(xtag!="")
                {
                  gelbooru_tag_set.insert(xtag);
                  xtag = "";
                }
              }
              xtag+=x[i];
            }
          }
          void danbooru_tags(std::string x)
          {
            std::string xtag = "";
            for(int i=0;i<x.length();i++)
            {
              if(x[i]==' ')
              {
                if(xtag!="")
                {
                  danbooru_tag_set.insert(xtag);
                  xtag = "";
                }
              }
              xtag+=x[i];
            }
          }          
            std::vector<std::string> sourcelinks;
            double similarity;
            void init(std::vector<std::string> ext_urls,std::string d)
            {
                similarity = std::stod(d);
                sourcelinks = ext_urls;
            }
    };
    //-----------------------------------------
    //-----------------------------------------
    //-----------------------------------------
    class sauceMech
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
        sauceMech(std::string param_api_key)
        {
            api_key = param_api_key;
            my_target_url = my_target_url_format+api_key;
        }
        void set_image_path(std::string param_file_path)
        {
          file_path = param_file_path;
          file_name = extract_file_name(file_path);
          output_file_name = "saucenao-"+file_name+".json";
        }
        std::string get_output_file_name(void)
        {
          return output_file_name;
        }
        int set_testmode(int x)
        {
          if(x==1 && my_target_url!=my_target_url_format)
          {
            my_target_url+="&testmode=1";
            return 1;
          }
          return 0;
        }
        int get_testmode(void)
        {
          size_t x = my_target_url.find("&testmode=1");
          if(x!=std::string::npos)
          {
            return 1;
          }
          return 0;
        }
        int set_dbmask(int x)
        {
          if(my_target_url!=my_target_url_format)
          {
            my_target_url += "&dbmask="+x;
            return 1;
          } 
          return 0;
        }
        int get_dbmask(void)
        {
          std::string search_term = "&dbmask=";
          size_t x = my_target_url.find(search_term);
          if(my_target_url!=my_target_url_format && x!=std::string::npos)
          {
            int y = x+search_term.length();
            int ret;
            std::stringstream ss(my_target_url.substr(y));
            ss>>ret;
            return ret;
          } 
          return -1;
        }
        int set_dbmaski(int x)
        {
          if(my_target_url!=my_target_url_format)
          {
            my_target_url += "&dbmaski="+x;
            return 1;
          } 
          return 0;
        }
        int get_dbmaski(void)
        {
          std::string search_term = "&dbmaski=";
          size_t x = my_target_url.find(search_term);
          if(my_target_url!=my_target_url_format && x!=std::string::npos)
          {
            int y = x+search_term.length();
            int ret;
            std::stringstream ss(my_target_url.substr(y));
            ss>>ret;
            return ret;
          } 
          return -1;
        }

        int set_db(int x)
        {
          if(my_target_url!=my_target_url_format)
          {
            my_target_url+="&db="+x;
            return 1;
          }
          return 0;
        }

        int get_db(void)
        {
          std::string search_term = "&db=";
          size_t x = my_target_url.find(search_term);
          if(my_target_url!=my_target_url_format && x!=std::string::npos)
          {
            int y = x+search_term.length();
            int ret;
            std::stringstream ss(my_target_url.substr(y));
            ss>>ret;
            return ret;
          } 
          return -1;
        }
        int set_numres(int x)
        {
          if(my_target_url!=my_target_url_format)
          {
            my_target_url+="&numres="+x;
            return 1;
          }
          return 0;
        }

        int get_numres(void)
        {
          std::string search_term = "&numres=";
          size_t x = my_target_url.find(search_term);
          if(my_target_url!=my_target_url_format && x!=std::string::npos)
          {
            int y = x+search_term.length();
            int ret;
            std::stringstream ss(my_target_url.substr(y));
            ss>>ret;
            return ret;
          } 
          return -1;
        }
        int fetch_json(void)
        {
          std::string str_input_file_name = file_path;
          std::string str_target_url = "http://saucenao.com/search.php?output_type=2&api_key="+api_key;
          std::string str_output_file_name = output_file_name;
          const char * input_file_name = str_input_file_name.c_str();
          const char * target_url = str_target_url.c_str();
          const char * output_file_name = str_output_file_name.c_str();

          //file existence test
          std::ifstream ftest(str_output_file_name.c_str());
          if(ftest.good())
          {
            std::cout<<"file "<<str_output_file_name<<" exists"<<std::endl;
            return 0;
          }
          //-----------
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
          return 1;
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
                    //---------------------------------------
                    std::string gb_check = "gelbooru.com";
                    if(xstr.find(gb_check)!=std::string::npos)
                    {
                      int idpos = xstr.find("id=");
                      idpos+=3;
                      std::string gb_id = "";
                      while(idpos<xstr.length())
                      {
                        if(xstr[idpos]=='&')
                        {
                          break;
                        }
                        gb_id+=xstr[idpos++];
                      }
                      gelbooruMech gm(gb_id);
                      gm.fetch();
                      sr.gelbooru_tags(gm.get_tags());
                      sr.gelbooru_id = gb_id;
                    }
                    
                    //---------------------------------------
                    std::string db_check = "danbooru.donmai.us";
                    if(xstr.find(db_check)!=std::string::npos)
                    {
                      int idpos = xstr.find("/show/");
                      idpos+=6;
                      std::string db_id = "";
                      while(idpos<xstr.length())
                      {
                        db_id+=xstr[idpos++];
                      }
                      danbooruMech dm(db_id);
                      dm.fetch();
                      sr.danbooru_tags(dm.get_tags());
                      sr.danbooru_id = db_id;
                    }
                    
                    //---------------------------------------
                    v.push_back(xstr);
                }
                sr.init(v,similarity);
                rvec.push_back(sr);
            }
            return rvec;
        }
    };
}