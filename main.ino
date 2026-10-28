#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite spr = TFT_eSprite(&tft);

const char* messages[] = {
  "Today's forecast: mostly words, moving left, with a slight chance of more words later.",
  "Please do not tap the screen. It is not a touchscreen and it is already doing its best.",
  "Powered by a chip smaller than a postage stamp and far more confident than it has any right to be.",
  "This message will self-destruct in ten seconds, or more accurately it will scroll off the left and come back later, which is less dramatic but kinder to the hardware.",
  "If you can read this, you have been staring at a desk gadget for too long and should probably put the kettle on.",
  "These words have travelled several centimetres at enormous expense to reach you, and would appreciate a small nod of acknowledgment."
};
const int MSG_COUNT = sizeof(messages) / sizeof(messages[0]);

const int STEP_PX  = 2;   // pixels moved per frame
const int FRAME_MS = 10;  // delay per frame; lower is faster

int msgIndex = 0;
int textW;
int x;

void setup() {
  pinMode(15, OUTPUT);
  digitalWrite(15, HIGH);       // display power, needed when on battery

  tft.init();
  tft.setRotation(1);           // landscape, 320x170
  tft.fillScreen(TFT_BLACK);

  pinMode(38, OUTPUT);
  digitalWrite(38, HIGH);       // backlight

  spr.createSprite(tft.width(), 60);
  spr.setFreeFont(&FreeSansBold24pt7b);
  spr.setTextColor(TFT_YELLOW);
  spr.setTextDatum(ML_DATUM);   // vertically centred on y

  textW = spr.textWidth(messages[msgIndex]);
  x = tft.width();              // start just off the right edge
}

void loop() {
  spr.fillSprite(TFT_BLACK);
  spr.drawString(messages[msgIndex], x, spr.height() / 2);
  spr.pushSprite(0, (tft.height() - spr.height()) / 2);

  x -= STEP_PX;

  if (x < -textW) {             // message fully off the left edge
    msgIndex = (msgIndex + 1) % MSG_COUNT;
    textW = spr.textWidth(messages[msgIndex]);
    x = tft.width();
  }

  delay(FRAME_MS);
}
