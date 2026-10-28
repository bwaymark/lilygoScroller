# T-Display S3 Marquee

A simple Arduino sketch that scrolls a rotating set of messages across the screen of a LilyGO T-Display S3, right to left, like a marquee or ticker. Text is drawn into an off-screen sprite so it scrolls smoothly without flicker.

## Hardware

- **Board:** LilyGO T-Display S3 (ESP32-S3R8, 16MB flash, 8MB PSRAM)
- **Display:** 1.9" ST7789 IPS LCD, 170×320, 8-bit parallel interface (built in)
- **Cable:** USB-C cable that carries data, not just power

Both the standard and touch versions of the T-Display S3 work (touch isn't used). The T-Display S3 AMOLED and the original ESP32 T-Display are different boards with different display drivers and are **not** supported by this sketch as written.

Pins used (all on-board, no wiring needed):

| GPIO | Purpose |
|---|---|
| 15 | Display power enable (needed when running on battery) |
| 38 | Backlight |
| 0, 14 | Buttons (not used yet) |

## Software requirements

| Component | Version | Notes |
|---|---|---|
| Arduino IDE | 2.x | https://www.arduino.cc/en/software |
| esp32 board package (Espressif) | **2.0.14** | 3.x has known problems with TFT_eSPI on this board |
| TFT_eSPI library (Bodmer) | 2.5.x | Needs a one-line configuration change, see below |

## Setup

### 1. Install the ESP32 board package

1. In Arduino IDE open **Settings** (Preferences on older versions).
2. In **Additional boards manager URLs** add:
   ```
   https://espressif.github.io/arduino-esp32/package_esp32_index.json
   ```
3. Open **Boards Manager**, search for `esp32` by Espressif Systems, choose version **2.0.14** from the dropdown and install.

### 2. Install and configure TFT_eSPI

1. Open **Library Manager**, search for `TFT_eSPI` by Bodmer and install it.
2. Find the library folder:
   - macOS: `~/Documents/Arduino/libraries/TFT_eSPI`
   - Windows: `Documents\Arduino\libraries\TFT_eSPI`
   - Linux: `~/Arduino/libraries/TFT_eSPI`
3. Edit `User_Setup_Select.h` in that folder:
   - comment out `#include <User_Setup.h>`
   - uncomment `#include <User_Setups/Setup206_LilyGo_T_Display_S3.h>`
4. Save the file.

Updating TFT_eSPI through Library Manager overwrites this file, so redo this step after any library update.

Alternatively, LilyGO publish a pre-configured copy of TFT_eSPI in their T-Display-S3 repository (https://github.com/Xinyuan-LilyGO/T-Display-S3) under `lib/`. Copy that into your libraries folder instead.

### 3. Board settings

In the **Tools** menu set:

| Setting | Value |
|---|---|
| Board | ESP32S3 Dev Module |
| USB CDC On Boot | Enabled |
| Flash Size | 16MB (128Mb) |
| PSRAM | OPI PSRAM |
| Upload Mode | UART0 / Hardware CDC |
| Port | The board's USB serial port (see below) |

### 4. Connect the board

Plug the board in with a USB-C data cable. The port appears as:

- macOS: `/dev/cu.usbmodem…`
- Windows: a new `COM` port in Device Manager
- Linux: `/dev/ttyACM0` (add yourself to the `dialout` group if you get a permission error: `sudo usermod -aG dialout $USER`, then log out and in)

If no port appears, or uploading fails, put the board into bootloader mode: hold the **BOOT** button, press and release **RST**, then release **BOOT**. Upload, then press **RST** once to run the sketch.

### 5. Upload

1. Clone or download this repository.
2. Open the sketch in Arduino IDE. The `.ino` file must sit in a folder of the same name (Arduino enforces this).
3. Click **Upload**.

The messages should start scrolling across the middle of the screen as soon as the upload finishes.

## Customising

All settings are at the top of the sketch.

- **Messages:** edit the `messages` list. Add or remove as many lines as you like; the count works itself out. Each message scrolls fully off the screen before the next one starts.
- **Speed:** `STEP_PX` is how many pixels the text moves each frame and `FRAME_MS` is the delay between frames. A bigger step is faster but less smooth; a shorter delay is faster and smoother.
- **Font:** change `FreeSansBold24pt7b` in `setup()` to another TFT_eSPI free font, such as `FreeSansBold18pt7b` (smaller) or `FreeSerifBold24pt7b`.
- **Colour:** change `TFT_YELLOW` to any TFT_eSPI colour constant (`TFT_WHITE`, `TFT_CYAN`, `TFT_GREEN` etc.) or an RGB565 value.

### Keeping personal messages out of the repo

Move the `messages` array and `MSG_COUNT` into a file called `messages.h` next to the sketch, add `messages.h` to `.gitignore`, and replace the array in the sketch with:

```cpp
#include "messages.h"
```

Commit a `messages.example.h` containing the public set. Anyone cloning the repo copies it to `messages.h` before compiling.

## Troubleshooting

| Problem | Likely cause and fix |
|---|---|
| `Bad CPU type in executable` (macOS) | Apple Silicon Mac running the Intel-only esptool shipped with core 2.0.x. Install Rosetta: `softwareupdate --install-rosetta --agree-to-license`, then restart the IDE. |
| Compiles and uploads but the screen stays black | TFT_eSPI is not using the T-Display S3 setup (recheck step 2), or you are on esp32 core 3.x (go back to 2.0.14). |
| Screen works on USB but not on battery | GPIO 15 must be set high to power the display; the sketch does this, so check you haven't removed it. |
| Garbled or wrong colours | Wrong TFT_eSPI setup file selected. |
| No serial port | Charge-only USB cable, or the board needs bootloader mode (step 4). |
| `exit status 2` / upload timeout | Use bootloader mode, and close anything else using the port (such as the Serial Monitor). |
| Text flickers | You have changed the sketch to draw directly to `tft` instead of the sprite `spr`. |

## Licence

MIT. See `LICENSE`.
