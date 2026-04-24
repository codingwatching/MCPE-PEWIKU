# Pewiku
**Whats it?**  
Implementation of a block-based game inspired by Minecraft.  
Right now this project corresponds to Minecraft Pocket Edition v0.6.2  

Discord: [https://discord.gg/e5pHK5u2UD](https://discord.gg/e5pHK5u2UD)  

**Goals:**  
- Full compliance with MCPE 0.7 & 0.8  
- Saving MCPE experience from gameplay
> Release updates according to the MCPE versions. Version 0.8 is not a goal, but an intermediate stage. The target is version 1.0-1.2

**Tasks is right now:**  
- Options Screen  
- Fix bugs  
- Polish Android building  

**Recent updates:**
- Play on linux / windows  
- PC controls  

**Features:**  
- Basic blocks  
- World generation like MC beta  
- Crafting system  
- Creative/Survival game modes  
- Multiplayer & Server core includes  


# Assets
You should take the assets from the original MCPE or use your custom assets.   


# Building
You *can* to go to the `handheld`/`project` folder and select a platform, but it's better to build from the project root.  

### Build all
useful for dev  
build both client and server: `make build_all`  
### Standalone server
build: `make build-server`  
build & run: `make run-server`  

### Windows
Building using Visual Studio 2022 is supported.  
Open the .sln, build, and run!  
### Linux (Debian)
Install all dependencies, see them: **`make deps`**  
build: `make build-client`  
build & run: `make run-client` (optional: add `DEBUG=1` for debug mode)  
### Android
android - new solution  
android_java - old for android 2.2  
*guide will appear later*  
