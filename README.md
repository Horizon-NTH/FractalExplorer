# ShapeDrawer

## Introduction

**FractalExplorer** is an application that let you explore the beautiful Mandelbrot set.

## Installation Instructions

> You can also simply install a pre-built version [here](https://github.com/Horizon-NTH/FractalExplorer/releases).

### Get Source Code

You first need to clone the [repository](https://github.com/Horizon-NTH/FractalExplorer). Make sure to use [git](https://git-scm.com) and don't forget to include `--recurse-submodules` when cloning.

```bash
git clone https://github.com/Horizon-NTH/FractalExplorer --recurse-submodules
```

You will now able to choose between [script](#script-installation) or [manual](#manual-installation) installation

- ### Script Installation

  Use the [`install.sh`](https://github.com/Horizon-NTH/FractalExplorer/blob/master/install.sh) script to compile the executable. You can choose to erase build files by adding `-e` / `--erase`.

  ```bash
  ./install.sh --erase
  ```

- ### Manual Installation

  #### Create Build Environment

  Ensure you have [CMake](https://cmake.org/) installed. Generate the build environment using CMake. 

  ```bash
  cd FractalExplorer
  mkdir build && cd build
  cmake ..
  ```

  #### Build

  - ##### CMake

    Build directly using CMake:

    ```bash
    cmake --build .
    ```

    This command will create all the executable in the main folder.

  - ##### Microsoft Visual Studio

    Alternatively, you can use [Microsoft Visual Studio](https://visualstudio.microsoft.com/) to launch the project and build it. Open the `fractalexplorer.sln` file with Microsoft Visual Studio.

## Usage

To learn how to use this application, please refer to the [wiki](https://github.com/Horizon-NTH/FractalExplorer/wiki).

## Dependencies

* **HorizonGUI**

    The code relies on [HorizonGUI](https://github.com/Horizon-NTH/HorizonGUI) for all the graphics-related functionality of the application.