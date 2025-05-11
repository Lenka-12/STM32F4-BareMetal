
# STM32F4xx BareMetal

# Getting Started with STM32F4xx series

Clone the repository to your local computer. Go to linker script `build/stm32_ls.ld` and update the RAM and Flash size according to your processor specs.

## Tools Needed

Before you start, ensure you have the following tools installed on your system:

1. **Arm GNU Toolchain**
   - **Windows**:
     - Download the Arm GNU Toolchain for Windows (here)[https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads].
     - Install the toolchain by running the installer.
     - During installation, specify the installation directory (e.g., `C:\Program Files (x86)\GNU Arm Embedded Toolchain`).
     - Add the toolchain's `bin` directory to the system's `PATH` environment variable.
   - **Linux (Ubuntu)**:
     - Install the Arm GNU Toolchain using the package manager:
       ```bash
       sudo apt update
       sudo apt install gcc-arm-none-eabi
       ```

2. **OpenOCD**
   - **Windows**:
     - Download the Windows distribution of OpenOCD from https://gnutoolchains.com/arm-eabi/openocd/.
     - Extract the downloaded archive to a folder of your choice (e.g., `C:\openocd`).
     - Add the OpenOCD `bin` directory to the system's `PATH` environment variable.
   - **Linux (Ubuntu)**:
     - Install OpenOCD using the package manager:
       ```bash
       sudo apt update
       sudo apt install openocd
       ```

3. **Make**
   - **Windows**:
     - Download the Windows binary distribution of [Make](https://gnuwin32.sourceforge.net/packages/make.htm) or use choco.
     - Install Make by running the installer.
     - Add the Make `bin` directory to the system's `PATH` environment variable.
   - **Linux (Ubuntu)**:
     - Install Make using the package manager:
       ```bash
       sudo apt update
       sudo apt install make
       ```

## Downloading Code to our Board

1. Go to the project folder containing the Makefile.
2. Open that folder in the terminal.
3. Type `make flash`.
   
## References
### Openocd
(pdf)(https://openocd.org/doc/pdf/openocd.pdf
https://openocd.org/pages/documentation.html

### ToolChain
https://gcc.gnu.org/onlinedocs/gcc/ARM-Options.html

### Make
https://www.gnu.org/software/make/manual/make.html

### LinkerScript
https://ftp.gnu.org/old-gnu/Manuals/ld-2.9.1/html_chapter/ld_3.html

### To-do: add more depth info on installation, library usage, and more..
