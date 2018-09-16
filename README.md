# sauce
C++ classes to wrap around saucenao.com and \*booru APIs.

# example
- program to identify source and print corresponding tags from gelbooru
```c++
#include "sauce.hpp"
#include <iostream>

int main()
{
    //create an instance of sauceMech and pass your saucenao API key as the parameter   
    sauce::sauceMech sm("0000000000000000000000000000000000000000");
    //set path    
    sm.set_image_path("saber.png");
    //downloads info into a json file, network transfer does not occur if file already exists
    //saucenao results can also be obtained in a json object (nlohmann::json) with the sauceMech::get_json() function
    sm.fetch_json();
    //process json and get a std::vector<sauce::sauceResult> with *booru tag information, links and similarity percentage
    auto x = sm.get_sauce_res();
    for(auto it = x.begin();it!=x.end();++it)
    {
        auto y = *it;
        int n = y.gelbooru_tag_set.size();
        auto zz = y.gelbooru_tag_set;
        if(n)
        {
            std::cout<<"gelbooru id: "<<y.gelbooru_id<<std::endl;
            std::cout<<"gelbooru tags: "<<std::endl;
            for(auto tag_iter = zz.begin(); tag_iter!=zz.end(); ++tag_iter)
            {
                std::cout<<*tag_iter<<std::endl;
            }
        }
        int n2 = y.danbooru_tag_set.size();
        auto vv = y.danbooru_tag_set;
        if(n2)
        {
            std::cout<<"danbooru id: "<<y.gelbooru_id<<std::endl;
            std::cout<<"danbooru tags: "<<std::endl;
            for(auto tag_iter = vv.begin(); tag_iter!=vv.end(); ++tag_iter)
            {
                std::cout<<*tag_iter<<std::endl;
            }
        }

    }
    return 0;
}
```

- program to identify the source of all files in a directory and rename them to their gelbooru IDs, if available

```c++

#include "sauce.hpp"
#include <cstdio>
#include <algorithm>
#include <map>
#include <string>
#include <iostream>
#include <dirent.h>

int main()
{
    std::map<std::string,std::string> mymap;
    auto dirp = opendir(".");
    auto dp = readdir(dirp);
    while(dp!=NULL)
    {
        std::cout<<dp->d_name<<std::endl;
        std::string fn = dp->d_name;
        if(fn.find(".png")==std::string::npos && fn.find(".jpg")==std::string::npos)
        {
            dp = readdir(dirp);
            continue;
        }
        std::cout<<fn<<std::endl;
        dp = readdir(dirp);
        sauce::sauceMech sm("0000000000000000000000000000000000000000");
        sm.set_image_path(fn);
        sm.fetch_json();
        auto x = sm.get_sauce_res();
        for(auto it = x.begin();it!=x.end();++it)
        {
            auto y = *it;
            int n = y.gelbooru_tag_set.size();
            auto zz = y.gelbooru_tag_set;
            if(n)
            {
                std::string nf = y.gelbooru_id +"-gelbooru"+ fn.substr(fn.rfind("."));
                mymap[fn] = nf;
                break;
            }
        }
        dp = readdir(dirp);
    }   
    closedir(dirp);
    int res;
    for(auto it = mymap.begin();it != mymap.end(); ++it)
    {
        auto fn = it->first;
        auto nf = it->second;
        res = std::rename(fn.c_str(),nf.c_str());
        res?(std::cout<<fn<<" successfully renamed to "<<nf<<std::endl):(std::cout<<fn<<" could not be renamed"<<std::endl);
    }
    return 0;
}
```

remember to link libcurl

    g++ foo.cpp -l curl -o foo.out

# CURRENTLY SUPPORTED BOORUS
- [Gelbooru](https://gelbooru.com/)
- [Danbooru](https://danbooru.donmai.us/)

# depends
- [nlohmann/json](https://github.com/nlohmann/json)
- [curl/libcurl](https://github.com/curl/curl)
- [zeux/pugixml](https://github.com/zeux/pugixml)

# todo
- ~~replace system() with libcurl and curlopt~~ **done**
- ~~refactor prototype.cpp into main class~~ **done**
- ~~add \*booru API~~ **done**
- ~~add Danbooru API support~~ **done**
- implement general purpose tag map
