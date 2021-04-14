# TechminoBackend

![Build release](https://github.com/26F-Studio/TechminoBackend/workflows/Build%20release/badge.svg?branch=main)

Backend for the tetromino-based game [Techmino](https://github.com/26F-Studio/Techmino0.12)

## Usage

### Windows

Clone and init the project:

```bash
git clone https://github.com/26F-Studio/TechminoBackend.git
cd TechminoBackend
git submodule update --init
```

Install Vcpkg:

```
./vcpkg/bootstrap-vcpkg.bat
```

Install necessary packages:

```
./vcpkg/vcpkg install drogon:x64-windows cryptopp:x64-windows --vcpkg-root=./vcpkg
```

Configure project: (Replace `{Project_Path}` to the absolute path of this project in your disk, for release version just
change `Debug` to `Release`)

```bash
./vcpkg/installed/x64-windows/tools/drogon/drogon_ctl.exe create model ./lib_models/models
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE="{PROJECT_PATH}/vcpkg/scripts/buildsystems/vcpkg.cmake" -G "CodeBlocks - NMake Makefiles" "{PROJECT_PATH}"
```

Build the project: (Replace `{CPU_CORES}` to number of your CPU's logic cores)

```
cmake --build "{PROJECT_PATH}\build" --target TechminoBackend --parallel {CPU_CORES}
```

### Linux

Install packages:

```bash
sudo apt update
sudo apt install git gcc g++ cmake libjsoncpp-dev uuid-dev openssl libssl-dev zlib1g-dev libc-ares-dev libbrotli-dev postgresql-all libmariadb-dev libsqlite3-dev -y
```

Build drogon:

```bash
git clone https://github.com/an-tao/drogon
cd drogon
git submodule update --init
mkdir build
cd build
cmake -DPostgreSQL_TYPE_INCLUDE_DIR=/usr/include/postgresql ..
make && sudo make install
```

Build CryptoPP

```bash
git clone https://github.com/weidai11/cryptopp.git
cd cryptopp
git submodule update --init
wget -O CMakeLists.txt https://raw.githubusercontent.com/noloader/cryptopp-cmake/master/CMakeLists.txt
wget -O cryptopp-config.cmake https://raw.githubusercontent.com/noloader/cryptopp-cmake/master/cryptopp-config.cmake
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
sudo make && sudo make install
```

Clone and init the project:

```bash
git clone https://github.com/26F-Studio/TechminoBackend.git
cd TechminoBackend
git submodule update --init
```

Configure project: (for release version just change `Debug` to `Release`)

```bash
drogon_ctl create model ./lib_models/models
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
```

Build the project: (Replace `{CPU_CORES}` to number of your CPU's logic cores)

```bash
cmake --build . --target TechminoBackend --parallel {CPU_CORES}
```