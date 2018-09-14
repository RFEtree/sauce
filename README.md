# sauce
bunch of c++ classes to wrap around saucenao.com and \*booru APIs.

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
    //process json and get a std::vector<sauce::sauceResult> with tag information, links and similarity percentage
    auto x = sm.get_sauce_res();
    for(auto it = x.begin();it!=x.end();++it)
    {
        auto y = *it;
        int n = y.gelbooru_tag_set.size();
        auto zz = y.gelbooru_tag_set;
        if(n)
        {
            for(auto tag_iter = zz.begin(); tag_iter!=zz.end(); ++tag_iter)
            {
                std::cout<<*tag_iter<<std::endl;
            }
        }
    }
    return 0;
}
```

remember to link libcurl

    g++ foo.cpp -l curl -o foo.out

# CURRENTLY SUPPORTED BOORUS
- [Gelbooru](https://gelbooru.com/)
- Danbooru (coming soon)

# depends
- [nlohmann/json](https://github.com/nlohmann/json)
- [curl/libcurl](https://github.com/curl/curl)
- [zeus/pugixml](https://github.com/zeux/pugixml)

# todo
- ~~replace system() with libcurl and curlopt~~ **done**
- ~~refactor prototype.cpp into main class~~ **done**
- ~~add \*booru API~~ **done**
- add Danbooru API support
