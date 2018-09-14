# sauce
bunch of c++ classes to wrap around saucenao.com's API. remember to link libcurl
```sh gcc foo.cpp -l curl -o foo.out```

# depends
- [nlohmann/json](https://github.com/nlohmann/json)
- [curl/libcurl](https://github.com/curl/curl)

# todo
- ~~replace system() with libcurl and curlopt~~ **done**
- ~~refactor prototype.cpp into main class~~ **done**
- rewrite sauceMachine into a more structured class
