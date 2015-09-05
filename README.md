# kodama
multi-threaded orthodox file manager

----
[![Build Status](https://travis-ci.org/srouquette/kodama.png)](https://travis-ci.org/srouquette/kodama)
[![Coverage Status](https://coveralls.io/repos/srouquette/kodama/badge.svg?branch=develop&service=github)](https://coveralls.io/github/srouquette/kodama)
<!---
[![Coverity Status](https://scan.coverity.com/projects/6257/badge.svg)](https://scan.coverity.com/projects/srouquette-kodama)
--->

## Linux
```shell
    # debian, ubuntu...
    sudo apt-get install libboost-all-dev qt5-default qttools5-dev-tools google-mock
    # arch
    sudo pacman -S boost qt5-base qt5-quickcontrols gtest gmock
```
### Build kodama
```shell
    ./waf configure
    ./waf install_release
    bin/release/kodama
```
## Windows

Download and build [Boost](http://www.boost.org/).

```shell
b2.exe -a -d0 -j4 variant=debug,release link=shared runtime-link=shared threading=multi --build-type=complete stage [address-model=64]
```

Note: if you have a link error when configuring the project, make sure you used the right address model when building boost.

If you don't want to build [Boost](http://www.boost.org/), download [BlueGo](http://vertexwahn.de/bluego.html) and build Boost with it. Or check the blog for prebuilt libs.

Download and install [Qt](http://qt-project.org/downloads).

Google Test is optional, but if you want to build the unit tests, make sure you built gtest with multi-threading (MD) and for x64, based on your platform.

### Build kodama
Edit env.bat and specify where Boost and Qt are located.
```shell
    configure.bat

    release.bat
    run.bat release

    release.bat install
    cd bin\release
    kodama.exe
```
## License
[MPL 2.0](https://www.mozilla.org/MPL/2.0/)
