# 📑 Simultanous Mouse and Gamepad Plugin
Native dll plugin for starfield. This branch is for ASI loader. 

## ⚙ Requirements

- [CMake 3.26+](https://cmake.org/)
  - Add this to your `PATH` during installtion/updating
- [PowerShell](https://github.com/PowerShell/PowerShell/releases/latest)
- [Vcpkg](https://github.com/microsoft/vcpkg)
  - Set the `VCPKG_ROOT` environment variable to the path of the vcpkg folder
  - Make sure your local vcpkg port is up-to-date by pulling the latest and do `vcpkg integrate install`
- [Visual Studio Community 2022](https://visualstudio.microsoft.com/)
  - Desktop development with C++
- [Starfield Steam Distribution](#-deployment)
  - Set the `SFPath` environment variable to the path of the game installation
  
## ⬇️ Get started

### 💻 Register Visual Studio as a Generator

- Open `x64 Native Tools Command Prompt`
- Run `cmake`
- Close the cmd window

### 📦 Dependencies

- [DKUtil](https://github.com/gottyduke/DKUtil)
- LibStarfield

These dependencies can be configured through the git submodule by running `update-submodule.bat`. Alternatively, the dependencies can also use a local git repository, by setting the `DKUtilPath` environment variable to the path of the local git repository.

In order to enable local git repository lookup, existing folders within `extern` should be **removed**.

> To prevent duplicating submodules in multiple local projects, it's recommended to clone the DKUtil repositories locally. Then, set the environment path accordingly, this ensures all projects share the same package.  

### 🔨 Building

```
.\make-sln-msvc.bat
cmake --build build --config Release
```
A Visual Studio solution will be generated inside `build` folder.

### ➕ Addon

This project bundles [DKUtil](https://github.com/gottyduke/DKUtil).  
This plugin template has auto deployment rules for easier build-and-test, build-and-package features, using simple json rules. [Read more here!](https://github.com/gottyduke/SF_PluginTemplate/wiki/Custom-deployment-rules)  
> Both enabled by default.

## 📖 License

This branch is licensed under [MIT](LICENSE).

## ❓ Credits

- [Parapets for the original plugin and LibStarfield](https://github.com/Exit-9B/Starfield-SimultaneousInput).
- [Ryan for his commonLibSSE code](https://github.com/Ryan-rsm-McKenzie/CommonLibSSE) which was referenced in DKUtil and LibStarfield.
- [Original plugin template](https://github.com/gottyduke/PluginTemplate).
