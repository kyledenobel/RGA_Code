# Compiler Installation Instructions
1) go to the following [link and install the latest version for your operating system](https://sourceforge.net/projects/sdcc/files/)
2) run the installer
3) confirm proper installation by opening a new teminal and running `sdcc -v`
    - if this does not work, make sure you have added the compiler to your path


# Flashing Tool Installation Instructions
1) go to the following [link and install the software](https://www.st.com/en/development-tools/stvd-stm8.html#get-software)
2) run the installer
3) add directory containing `STVP_CmdLine` to your path
    - on windows this should be "C:\Program Files (x86)\STMicroelectronics\st_toolset\stvp"
    - for mac, append to the file ".zshrc"
            - "export PATH="$PATH:<filepath>"


# Confirming setup
To confirm the setup, run
`make verify`

# Makefile Commands
`make help`
- shows a list of available commands
`make verify`
- attempts to run version check on sdcc and list commands of STVP_CmdLine. This should run without failing
`make build`
- builds the hex file
`make flash`
- flashes the hex file to the target
