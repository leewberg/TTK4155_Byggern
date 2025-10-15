#include "CAN.h"
#include "input.h"
#include "oled.h"

void doom_init();
void doom_loop();
void fetch_frame();
void fetch_and_send_inputs();
void send_ready_frame();
