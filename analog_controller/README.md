# STM32 Firmware Template

## Cloning

Clone using

```Bash
git clone https://github.gatech.edu/GTSR/STM32G4-Firmware-Template.git --recurse-submodules
```

**OR**

If you already cloned the repository, then run to clone submodules (ex. stmlibs, CMSIS, and HAL library)

```Bash
git submodule update --init --recursive --remote
```

## Using template on existing repo

Create a new branch on the repository with (replace \<branch-name\> with the branch name desired)

```Bash
git switch -c <branch-name>
```

Delete uneeded (most likely all files besides possibly C code files) files and commit changes

Add STM32G4-Firmware-Template as a remote repository

```Bash
git remote add -f template https://github.gatech.edu/GTSR/STM32G4-Firmware-Template.git
```

Fetch template data

```bash
git fetch template
```

Merge template changes. **NOTE:** This assumes you want to merge on the main branch of the template

**In general, do not use `--allow-unrelated-histories`**

```bash
git merge template/main --allow-unrelated-histories -s ort -X patience -X ignore-all-space
```

Clone submodules

```Bash
git submodule update --init --recursive --remote
```

Move files saved from before to their correct locations (most probably Core/Src for .c files and Core/Inc for .h files)

## Updating Makefile

For each program, update `TARGET` variable in the Makefile to be specific to your program. **Use only alphanumeric characters and underscore. No spaces. Target name cannot start with a number**

If you add new source files, then you will need to update the Makefile

Update `C_APP_SOURCES` or `CXX_APP_SOURCES` with all used C and C++ source files in the `Core/Src` directory

If you add new libraries, then update `C_LIB_SOURCES` and `CXX_LIB_SOURCES` with all used C and C++ source files used by library code. Follow the format shown by `printf` library

If you need custom compiler flags, then update `EXTRA_CFLAGS`, `EXTRA_CXXFLAGS` and `EXTRA_LDFLAGS` to modify the compilation of C files, compilation of C++ files, and the linker respectively

## Compiling

Compile code by running `make` at top-level of the repository
By default, no prints and `O2` optimization level

Additional rules:

- `make error` prints error logs and has `O2` optimization
- `make warn` prints error and warnings logs and has `O2` optmization
- `make info` prints errors, warnings, and info logs and has `O1` optimization and has debug symbols
- `make debug` prints debug logs, debug optimization and debug symbols
- `make verbose` prints verbose logs, debug optimization and debug symbols

Use `make clean` or `make distclean` clean up compiled code and to allow recompilation from scratch

**NOTE:** By default, the HAL library and other 3rd party libraries are not compiled with warnings. If you want to check if there are warnings in those files, append `FULL_WARNING=1` to your `make` command. You must clean up all build artifacts with `make distclean` for this to correctly take effect.

## Flashing

Currently, it is expected that the flashing tool (OpenOCD or ST-Programmer) is contained in the path enviornment variable

Add `flash` to the end of any of the compilation commands to flash the board with corresponding logging level and optimization level

Use `flash-stprog` if you need to flash with the ST progammer. `flash` is the standard command. `flash-stprog` is for backwards compatibility.

### For Windows Users

Use `usbipd` to connect the flashing tool (like ST-Link) to WSL
The following must be done in **admin** command prompt

#### List the avaliable USB devices

```bash
usbipd list
```

#### Binding USB Device

Bind device to usbipd where `#-#` corresponds to the flashing tool (STLINK, XDS-100) that you have plugged in. This must be done once per device

```bash
usbipd bind --busid #-#
```

#### Attaching USB device

Attach device to WSL. This must be done every reboot or when device is unplugged/replugged

```bash
usbipd attach --wsl --busid #-#
```

Optionally, use `lsusb` on WSL terminal to verify USB device is visible to WSL
Follow flashing instruction to flash

## Erasing

Currently, it is expected that the flashing tool (OpenOCD or ST-Programmer) is contained in the path

Use `make erase` to clear any firmware on the MCU
Use `make erase-stprog` to clear any firmware on MCU with ST-programmer

## Static analyzer

Use `make analyze` to run the GCC static analyzer while compiling

## Updating submodules (including **STMLIBS**)

To update all submodules, run

```bash
git submodule update --init --recursive --remote
```
Then, commit the changes to the base repository


To update a specific submodule, run

```bash
# Paths can be found in the .gitmodules file
git submodule update --init --recursive --remote path/to/submodule
```

To fix a submodule to a specific branch, use

```bash
# Paths can be found in the .gitmodules file
git submodule set-branch --branch <branch_name> path/to/submodule 
```

## Updating from template

If template has been updated, and you want to have the changes from the template, do the following

Add STM32G4-Firmware-Template as a remote repository
```bash
git remote add -f template https://github.gatech.edu/GTSR/STM32G4-Firmware-Template.git
```

Fetch template data

```bash
git fetch template
```

Merge template changes. **NOTE:** This assumes you want to merge on the main branch of the template

**In general, do not use `--allow-unrelated-histories`**

```bash
git merge template/main --allow-unrelated-histories -s ort -X patience -X ignore-all-space
```

## Setting up IDE

Follow instructions in the [docs](docs/ide-configs/README.md)

## Formatting

`.clang-format` is the file that indicates the code formatting for the program

Feel free to change the top-level `.clang-format` for your specific project. The top-level project will only modify formatting project-specific files in core folder

- `make format` will format all application code
- `make format-check` will check if application code is formatted correctly
    - Mostly intended for CI
- `make format-all` will format all code in repository including library code and submodules
- `make format-check-all` will check if all code follows the format specificed in the repository including library code and submodules

## Debugging

Setting up GDB support is a WIP

## File Hirearchy

```
.
|-- Core ~ Contains main application files
|   |-- Config ~ contains configuration files for application
|   |-- Inc ~ contains headers files for application
|   `-- Src ~ contains source files for application
|-- docs ~ contains any documentation
|   |-- images ~ Contains images used by documentation
|   `-- vscode ~ Contains recommended vscode configuration
|-- init ~ Contains startup and linker scripts
|-- lib ~ Contains 1st and 3rd party library
|   |-- ARM ~ Contains libraries from ARM
|   |   `-- CMSIS ~ Contains a standardized interface that defines hardware specifics
|   |-- GTSR ~ Contains libraries developed by GTSR
|   |   `-- stmlibs ~  Contains simplified library to interface with STM hardware periperals
|   |       |-- g4 ~ contains API and source to interface with STM32G4
|   |       |-- test
|   |       |   `-- support
|   |-- ST ~ Contains library from ST
|   |   `-- STM32G4xx_HAL_Driver ~ Contains hardware abstraction layer for STM32G4 (Only used when stmlibs does not fill the needs of application)
|   `-- third-party ~ Contains code from 3rd parties
|       `-- printf ~ Stack-based printf implementation
|-- tools ~ Contains scripts and tooling for project
|-- LICENSE
|-- Makefile ~ Script for how to compile the code
|-- README.md ~ This file
|-- .clang-format ~ Indicates how to format code
|-- .clang-tidy ~ Indicates what static analyzer warnings to show
|-- .gitignore ~ Indicates what files for git to not track
`-- .gitmodules ~ Indicates what submodules are in the project
```