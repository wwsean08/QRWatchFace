#pragma once

#define MY_UUID { 0x5C, 0xFF, 0x97, 0xF4, 0x50, 0x56, 0x40, 0x2A, 0x8E, 0x52, 0xFD, 0x2B, 0x73, 0x23, 0x42, 0xC9 }
PBL_APP_INFO(MY_UUID,
             "QR Clock", "wwsean08",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_WATCH_FACE);
	     
Window window;

Layer display_layer;

unsigned short get_display_hour(unsigned short hour);
void display_layer_update_callback(Layer *me, GContext* ctx);
void handle_minute_tick(AppContextRef ctx, PebbleTickEvent *t) ;
void handle_init(AppContextRef ctx) ;
void pbl_main(void *params);
