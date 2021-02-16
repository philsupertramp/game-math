## ds (Data science)-submodule

Used for 

## Dependencies
- ImageMagick/Magick++`>=7.0.10-45`  
Ubuntu: `sudo apt-get install libgraphicsmagick1-dev`  
Arch: `sudo pacman -S imagemagick`


For MacOS/Windows build add
```c++
InitializeMagick(*argv);
```
to your main to initialize `Magick++`  
**Note:** unnecessary for UNIX systems