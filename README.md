# Yu-Gi-Oh.cpp
### Introduction
Yu-Gi-Oh.cpp is a cross-platform multiplayer CLI game made in C++
### Features
- Play with everyone, doesn't matter if your opponent uses Linux or Windows connecting with them will just work
- Create your own custom cards, just edit the `AllCards` file inside the `build` directory
- Create your own custom decks, create or edit files ending with `.deck`
- Organize your decks in subfolders, inside the `Decks` folder you can create as many subfolders as you want and put your decks there

### Project Highlights
- Implemented documentation with **Doxygen** and hosted it on **GitHub Pages**
- Used **CMake** as the build system and implemented custom modules
- Used **vcpkg** as the package manager and integrated it in CMake
- Wrote Unit Tests with **Catch2** and included them in **CTest**
- Set up **GitHub Actions** to run CI/CD tests
- Implemented async networking using the **ASIO** library with thread-safe communication using mutexes/queues
- Used Sanitizers, LTO/IPO and set warning flags  
  
## Building
#### Requirements
- CMake
- vcpkg (with `VCPKG_ROOT` environment variable set)
#### Steps
1. Clone the repository
```
git clone https://github.com/Giuseppe-Guerrera/Yu-Gi-Oh.cpp.git
```
2. Change into the build directory
```
cd ./Yu-Gi-Oh.cpp/build
```
3. Configure the project
```
cmake ..
```
4. Build the project
```
cmake --build .
```
5. Run the game
```
Linux
./Yu-Gi-Oh.cpp
Windows
Yu-Gi-Oh.cpp.exe
```  
  
##### Note
The project must be built inside the `build` folder as it requires files contained there(AllCards and the Decks dir), alternatively you can move these files to the location of your executable