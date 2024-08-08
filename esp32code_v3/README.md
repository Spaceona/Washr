

# About
This is the second version of the SpaceOna code for an ESP32, more specifically the Seeed Studio ESP32C3.
It updated the code architecture to use c++ files to break the functions into separate files to make the code more readable.
It also updated the code to use the PlatformIO IDE instead of the Arduino IDE.

# Contributing Guidelines

### Development Environent Setup
1. Install a C/C++ IDE  which supports PlatformIO (e.g. [Visual Studio Code](https://code.visualstudio.com/), [CLion](https://www.jetbrains.com/clion/)).
2. Install [PlatformIO](https://platformio.org/install/) as an extension in your IDE.
3. Open the `esp32code_v3` folder in your IDE.
4. Open the PlatformIO Home from the PlatformIO extension in your IDE.
5. Build the project using the PlatformIO build button.
6. Upload the project to the ESP32C3 board using the PlatformIO upload button.
7. Open the serial monitor to view the output of the ESP32C3 board. (If using Clion, don't use the upload and monitor button, instead use the Clion Serial Monitor Plugin)
8. Make changes to the code and test them on the ESP32C3 board.
9. If you are going to add new libraries to the project, add them to the `platformio.ini` file.

### Code Style
TODO

### Pull Request Guidelines
TODO

### Firmware Update
Please make sure to change the firmware version in the globals.cpp file before commiting to the repository.
Once the code has been merged please make sure to build the firmware, rename it to the correct version, and commit it to the spaceona-db repository in the correct folder.

### WiFi Configuration
In order to connect the board to WiFi, please follow the steps below:

1. Create a new file named `wifi_secrets.h` in the `include` directory.
2. Copy and paste the following code into the `wifi_secrets.h` file:

```c
#ifndef WIFI_SECRETS_H
#define WIFI_SECRETS_H

#define WIFI_SSID "YOUR_SSID"
#define WIFI_PASSWORD "YOUR_PASSWORD"

#endif //WIFI_SECRETS_H
```

3. Replace `your_ssid` with your actual Wi-Fi network name (SSID).
4. Replace `your_password` with your actual Wi-Fi password (with the quotation marks still).

Please ensure that you do not share your `wifi_secrets.h` file publicly or commit it to version control, as it contains sensitive information.
