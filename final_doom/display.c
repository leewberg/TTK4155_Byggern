#include "display.h"

double delta = 1;
uint32_t lastFrameTime = 0;

uint8_t *display_buf;
uint8_t zbuffer[ZBUFFER_SIZE];

void setupDisplay() {
  // Setup display

  display_buf = malloc(SCREEN_WIDTH * SCREEN_HEIGHT / 8);
  memset(display_buf, 0, SCREEN_WIDTH * SCREEN_HEIGHT / 8);

  // initialize z buffer
  memset(zbuffer, 0xFF, ZBUFFER_SIZE);
}

// Adds a delay to limit play to specified fps
// Calculates also delta to keep movement consistent in lower framerates
void fps() {
  while (systick_ms - lastFrameTime < FRAME_TIME);
  delta = (double)(systick_ms - lastFrameTime) / FRAME_TIME;
  lastFrameTime = systick_ms;
}

double getActualFps() {
  return 1000 / (FRAME_TIME * delta);
}

// Faster way to render vertical bits
void drawByte(uint8_t x, uint8_t y, uint8_t b) {
  display_buf[(y / 8)*SCREEN_WIDTH + x] = b;
}

bool getGradientPixel(uint8_t x, uint8_t y, uint8_t i) {
  if (i == 0) return 0;
  if (i >= GRADIENT_COUNT - 1) return 1;

  uint8_t index = max(0, min(GRADIENT_COUNT - 1, i)) * GRADIENT_WIDTH * GRADIENT_HEIGHT // gradient index
                  + y * GRADIENT_WIDTH % (GRADIENT_WIDTH * GRADIENT_HEIGHT)             // y byte offset
                  + x / GRADIENT_HEIGHT % GRADIENT_WIDTH;                               // x byte offset

  // return the bit based on x
  return read_bit(gradient[index], x % 8);
}

void fadeScreen(uint8_t intensity, bool color) {
  for (uint8_t x = 0; x < SCREEN_WIDTH; x++) {
    for (uint8_t y = 0; y < SCREEN_HEIGHT; y++) {
      if (getGradientPixel(x, y, intensity)) 
        drawPixel(x, y, color, false);
    }
  }
}

// Faster drawPixel than display.drawPixel.
// Avoids some checks to make it faster.
void drawPixel(int8_t x, int8_t y, bool color, bool raycasterViewport) {
  // prevent write out of screen buffer
  if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= (raycasterViewport ? RENDER_HEIGHT : SCREEN_HEIGHT)) {
    return;
  }

  if (color) {
    // white
    display_buf[x + (y / 8)*SCREEN_WIDTH] |= (1 << (y & 7));
  } else {
    // black
    display_buf[x + (y / 8)*SCREEN_WIDTH] &= ~(1 << (y & 7));
  }
}

// For raycaster only
// Custom draw Vertical lines that fills with a pattern to simulate
// different brightness. Affected by RES_DIVIDER
void drawVLine(uint8_t x, int8_t start_y, int8_t end_y, uint8_t intensity) {
  int8_t y;
  int8_t lower_y = max(min(start_y, end_y), 0);
  int8_t higher_y = min(max(start_y, end_y), RENDER_HEIGHT - 1);
  uint8_t c;

  uint8_t bp;
  uint8_t b;
  for (c = 0; c < RES_DIVIDER; c++) {
    y = lower_y;
    b = 0;
    while (y <= higher_y) {
      bp = y % 8;
      b = b | getGradientPixel(x + c, y, intensity) << bp;

      if (bp == 7) {
        // write the whole byte
        drawByte(x + c, y, b);
        b = 0;
      }

      y++;
    }

    // draw last byte
    if (bp != 7) {
      drawByte(x + c, y - 1, b);
    }
  }
}

// Custom drawBitmap method with scale support, mask, zindex and pattern filling
void drawSprite(
  int8_t x, int8_t y,
  const uint8_t bitmap[], const uint8_t mask[],
  int16_t w, int16_t h,
  uint8_t sprite,
  double distance,
  bool only_add
) {
  uint8_t tw = (double) w / distance;
  uint8_t th = (double) h / distance;
  uint8_t byte_width = w / 8;
  uint8_t pixel_size = max(1, 1.0 / distance);
  uint16_t sprite_offset = byte_width * h * sprite;

  bool pixel;
  bool maskPixel;

  // Don't draw the whole sprite if the anchor is hidden by z buffer
  // Not checked per pixel for performance reasons
  if (zbuffer[min(max(x, 0), ZBUFFER_SIZE - 1) / Z_RES_DIVIDER] < distance * DISTANCE_MULTIPLIER) {
    return;
  }

  for (uint8_t ty = 0; ty < th; ty += pixel_size) {
    // Don't draw out of screen
    if (y + ty < 0 || y + ty >= RENDER_HEIGHT) {
      continue;
    }

    uint8_t sy = ty * distance; // The y from the sprite

    for (uint8_t tx = 0; tx < tw; tx += pixel_size) {
      uint8_t sx = tx * distance; // The x from the sprite
      uint16_t byte_offset = sprite_offset + sy * byte_width + sx / 8;

      // Don't draw out of screen
      if (x + tx < 0 || x + tx >= SCREEN_WIDTH) {
        continue;
      }

      maskPixel = mask ? read_bit(mask[byte_offset], sx % 8) : 1;
	  if (maskPixel == NULL) continue;

      if (maskPixel) {
        pixel = read_bit(bitmap[byte_offset], sx % 8);
		if (only_add && !pixel) continue;
        for (uint8_t ox = 0; ox < pixel_size; ox++) {
          for (uint8_t oy = 0; oy < pixel_size; oy++) {
            drawPixel(x + tx + ox, y + ty + oy, pixel, true);
          }
        }
      }
    }
  }
}

// Draw a single character.
// Made for a custom font with some useful sprites. Char size 4 x 6
// Uses less memory than display.print()
void drawChar(int8_t x, int8_t y, char ch) {
  uint8_t c = 0;
  uint8_t n;
  uint8_t bOffset;
  uint8_t b;
  uint8_t line = 0;

  // Find the character
  while (CHAR_MAP[c] != ch && CHAR_MAP[c] != '\0') c++;

  bOffset = c / 2;
  for (; line < CHAR_HEIGHT; line++) {
    b = bmp_font [ line * bmp_font_width + bOffset];
    for (n = 0; n < CHAR_WIDTH; n++)
      if (read_bit(b, (c % 2 == 0 ? 0 : 4) + n))
        drawPixel(x + n, y + line, 1, false);
  }
}

// Draw a string
void drawText(int8_t x, int8_t y, char *txt, uint8_t space) {
  uint8_t pos = x;
  uint8_t i = 0;
  char ch;
  while ((ch = txt[i]) != '\0') {
    drawChar(pos, y, ch);
    i++;
    pos += CHAR_WIDTH + space;

    // shortcut on end of screen
    if (pos > SCREEN_WIDTH) return;
  }
}

// Draw a F() string
// void drawText(int8_t x, int8_t y, const __FlashStringHelper *txt_p, uint8_t space = 1) {
//   uint8_t pos = x;
//   uint8_t i = 0;
//   char ch;
//   while (pos < SCREEN_WIDTH && (ch = F_char(txt_p, i)) != '\0') {
//     drawChar(pos, y, ch);
//     i++;
//     pos += CHAR_WIDTH + space;
//   }
// }

// Draw an integer (3 digit max!)
void drawInt(uint8_t x, uint8_t y, uint8_t num) {
  char buf[4]; // 3 char + \0
  // itoa(num, buf, 10);
  sprintf(buf, "%d", num);
  drawText(x, y, buf, 1);
}

void invertDisplay(bool invert) {
  if (invert){
	for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT / 8; i++){
		display_buf[i] = ~display_buf[i];
	}
  }
}

void clear_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
    for (uint8_t i = x; i < x + w; i++) {
        for (uint8_t j = y; j < y + h; j++) {
            // Calculate buffer index and bit position
            uint16_t byte_index = (i) + (j / 8) * SCREEN_WIDTH;
            uint8_t bit = j % 8;
            // Clear the pixel
            display_buf[byte_index] &= ~(1 << bit);
        }
    }
}
