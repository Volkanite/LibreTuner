LibreTuner
=========
Open source engine tuning software for Mazda platforms.
- Continuing work of LibreTuner [(original project no longer in development)](https://github.com/LibreTuner/LibreTuner)
- Project continues from the [master](https://github.com/LibreTuner/LibreTuner/tree/master) branch which uses the [0.1.3](https://github.com/LibreTuner/LibreTuner/releases/tag/v0.1.3) interface instead of the newer redesigned and [simplified](https://github.com/LibreTuner/LibreTuner/tree/simplify) interface of [0.1.4](https://github.com/LibreTuner/LibreTuner/releases/tag/0.1.4) (Overboost).


Screenshots
-----------
![Main Window](https://user-images.githubusercontent.com/3116133/62159853-ab6d5b80-b2e0-11e9-931d-1b15481e83fb.png)

Supported vehicles
------------------
* 2005-2007 Mazdaspeed6 / Mazda 6 MPS / Mazdaspeed Atenza
* MX-5 NC (Only editing)


Supported Interfaces
--------------------
* J2534 interfaces with CAN support (OBDX Pro VX)
* ELM327 devices (OBDLink SX)
* SocketCAN

Planned support
---------------
* Mazdaspeed3 (Gen 1 & 2)
* First Generation RX-8


Layout
------
All interesting code lives at `lib/LibLibreTuner`.

Building
--------
### Requirements (Debian)
* Qt5 - `sudo apt install qtcreator qtbase5-dev qt5-qmake libqt5charts5-dev libqt5datavisualization5-dev`
* Python3-pip - `sudo apt install python3-pip`
* Git - `sudo apt install git`  
* CMake 3.10 or higher - `sudo apt install cmake`  
* A compiler that supports C++17
* conan - `sudo pip3 install conan==1.66`

### Linux
1. `git clone https://github.com/Volkanite/LibreTuner.git`
2. `cd LibreTuner`
3. `git submodule update --init --recursive`
4. `conan install conanfile.txt`
5. `cmake .`
6. `make`
7. `cd bin`
8. `./LibreTuner`

### Windows
#### Requirements
- MSVC 14.2x (Visual Studio 2019)  
- Qt Creator 5.14.2 - https://download.qt.io/archive/qt/5.14/5.14.2
- 9GB free space (for Qt Creator install)
- Install options for Qt Creator (MSVC 2017 32-bit, Qt Charts, Qt Data Visualization)
- Conan 1.66 - https://github.com/conan-io/conan/releases/tag/1.66.0
- Cmake 3.31.11 - https://cmake.org/download/#legacy
- Ninja - https://github.com/ninja-build/ninja/releases
- Copy ninja.exe to `C:\Program Files\CMake\bin`

#### Command Line (Part 1)
1. `git clone https://github.com/Volkanite/LibreTuner.git`
2. `cd LibreTuner`
3. `git submodule update --init --recursive`
4. `conan install conanfile.txt`
#### Qt Creator (Part 2)
5. In Qt Creator, Open Project->LibreTuner\CMakeLists.txt
6. Manage Kits...
7. Select 'Desktop Qt 5.14.2 MSVC2017 32bit'
8. Set Compilers to Microsoft Visual C++ Compiler 16.x (x86)
9. Go to Projects, and set the build directory to the LibreTuner root directory
10. Go to Edit, Right click the LibreTuner project and choose Build
11. Add `C:\Qt\Qt5.14.2\5.14.2\msvc2017\bin` to the system's 'Path' variable.
12. Add the 'QT_QPA_PLATFORM_PLUGIN_PATH' environment variable to your system, with the path `C:\Qt\Qt5.14.2\5.14.2\msvc2017\plugins\platforms\`


