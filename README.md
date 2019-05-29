# OpenGL

## 如何编译

0. 根据[官方教程](https://github.com/microsoft/vcpkg)安装vcpkg

1. 安装依赖：
```
cd <path/to/vcpkg>
./vcpkg install assimp glew glfw3 glm sqlite3
```

2. 编译
```
cd <path/to/our/project>
mkdir build; cd build;
cmake -DCMAKE_TOOLCHAIN_FILE=<path/to/vcpkg>/scripts/buildsystems/vcpkg.cmake ..
make
```

## CLion用户如何使用vcpkg？

Go to File -> Settings -> Build, Execution, Deployment -> CMake
Choose one profile, and then find "CMake Options".
Insert value to the option: -DCMAKE_TOOLCHAIN_FILE=<path_to_vcpkg>/scripts/buildsystems/vcpkg.cmake
