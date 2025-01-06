# Contributing Guidelines

In order to connect the board to WiFi, please follow the steps below:

1. Create a new file named `wifi_secrets.h` in the `esp32code_v1` directory.
2. Copy and paste the following code into the `wifi_secrets.h` file:

```c
#define WIFI_SSID "your_ssid"
#define WIFI_PASSWORD "your_password"
```

3. Replace `your_ssid` with your actual Wi-Fi network name (SSID).
4. Replace `your_password` with your actual Wi-Fi password (with the quotation marks still).

Please ensure that you do not share your `wifi_secrets.h` file publicly or commit it to version control, as it contains sensitive information.
