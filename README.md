
# Dependencies
To compile and develope this projects, following dependencies are needed:
- [OpenGL](http://www.prinmath.com/csci5229/misc/install.html)
- GLEW, GLFW and Assimp
```
sudo apt install glew glfw assimp
```
Those above script varies as your distro are different. For example of Arch user: `sudo pacman -S glew glfw assimp`
- [SOIL2](https://github.com/SpartanJ/SOIL2) (Simple OpenGL Image Library 2)
- [Cmake](https://snapcraft.io/cmake) (For building SOIL2):

# Building and compilation
## Building SOIL2:
1. Clone the repo
2. Download [premake5](https://premake.github.io/download.html#v5)
3. Copy premake5 into SOIL2 directory, where premake5.lua is located
4. Run `./premake5 gmake2`
5. Run `cd make/linux`
6. Run `sudo make`

# Compiling the project
After building SOIL2, the shared and static library located in `/lib/linux`. Per need to export the path or copy those libs into `/usr/lib` so that the compiler could find those.

To compile `planet.cpp`:
```
g++ planet.cpp -lGL -lGLEW -lglfw -lassimp -llibsoil2-debug
```

# References

Please feel free to checkout these awesome tutorials for better understanding of the project.

- [Learn OpenGL](https://learnopengl.com/)
- [Sonar System Youtube Videos](https://www.youtube.com/watch?v=Tz0dq2krCW8&list=PLRtjMdoYXLf6zUMDJVRZYV-6g6n62vet8)
