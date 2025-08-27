
# ChikoBot Firmware

Firmware and libraries for ChikoBot, an ESP32-based robot platform. This project includes main robot logic, hardware drivers, BLE gamepad support, face/eye animation, and a suite of test and example applications.

## Features
- Modular code for robot control, sensors, and peripherals
- BLE gamepad client for Xbox and other controllers
- OLED face/eye animation with U8g2
- Accelerometer (BMA250) support
- Multiple test and example programs
- PlatformIO-based build system

## Project Structure

```
├── src/main_code/        # Main application code
├── src/examples/         # Example sketches and test programs
├── lib/                  # Libraries (BLE, face, sensors, etc.)
├── platformio.ini        # PlatformIO configuration
└── README.md             # This file
```


## Requirements
- ESP32 Dev Board (e.g., ESP32-DevKitC)
- Windows, macOS, or Linux PC

## Step-by-Step: Install VS Code and PlatformIO

### 1. Install Visual Studio Code
- Download VS Code from [https://code.visualstudio.com/](https://code.visualstudio.com/)
- Run the installer and follow the prompts to complete installation.

### 2. Install PlatformIO Extension for VS Code
- Open VS Code.
- Go to the Extensions view (Ctrl+Shift+X).
- Search for `PlatformIO IDE`.
- Click **Install** on the PlatformIO IDE extension (by PlatformIO Labs).
- Wait for installation to complete (it may take a few minutes).

### 3. Clone the Project Repository
Open a terminal (in VS Code: Ctrl+` or use your OS terminal) and run:
```
git clone https://gitlab.com/chikobot/chiko_firmware.git
cd chiko_firmware
```

### 4. Open the Project in VS Code
- In VS Code, go to `File > Open Folder...`
- Select the `chiko_firmware` directory you just cloned.

### 5. Configure PlatformIO for Your Board (Optional)
- Open `platformio.ini` in the project root.
- Make sure the `board` field in your environment matches your ESP32 board (e.g., `esp32dev`).
- Save the file if you make changes.

### 6. Build, Upload, and Monitor
- Open the PlatformIO sidebar (alien icon on the left).
- Under "PROJECT TASKS", expand your desired environment (e.g., `chiko_main`, `Example_XboxController`).
- Click **Build** to compile, **Upload** to flash, and **Monitor** to view serial output.
- Default serial baud rate is 115200.

---

## Quick Start (Summary)
1. Install VS Code
2. Install PlatformIO IDE extension
3. Clone this repo and open it in VS Code
4. Build, upload, and monitor using PlatformIO sidebar

## Running Examples
- Each example or test has its own environment in `platformio.ini`.
- To run an example, build/upload the corresponding environment (e.g., `Example_XboxController`).
- You can add new examples in the `examples/` folder and create a new environment with a `build_src_filter` in `platformio.ini`.

## Test Your Code
1. Edit ´src/main_code/main.cpp´.
2. Select `chiko_main` environment under "PROJECT TASKS"
3. Build and upload the `main.cpp`.
4. Monitor the out using `Serial Terminal`.

## VS Code/PlatformIO Tips
- Use the PlatformIO sidebar for all build, upload, and monitor tasks.
- Use "Clean" if you encounter strange build errors.
- You can have multiple environments for different tests/examples.
- Use `build_src_filter` to control which files are compiled for each environment.

## Detailed Tutorials, Library Discription and Workflow
1. [How to setup the development envoirnment](https://github.com/helscream/chiko_firmware/wiki/Setup-Development-Environment).
2. [Tutorails](https://github.com/helscream/chiko_firmware/wiki/Firmware-Tutorial)
3. All more in the [Wiki](https://github.com/helscream/chiko_firmware/wiki) of the Repo.

## Contributing
Contributions are welcome! Please fork the repo and submit a merge request.

## License
See `LICENSE` file for details.

## Authors
ChikoBot Team and contributors.
