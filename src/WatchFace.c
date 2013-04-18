#include "pebble_os.h"
#include "pebble_app.h"
#include "QR_Encode.h"
#include "WatchFace.h"

void handle_init(AppContextRef ctx) {
	(void)ctx;

	window_init(&window, "QR Clock");
	window_stack_push(&window, true);

	window_set_background_color(&window, GColorBlack);

	// Init the layer for the display
	layer_init(&display_layer, window.layer.frame);
	display_layer.update_proc = &display_layer_update_callback;
	layer_add_child(&window.layer, &display_layer);
}

void pbl_main(void *params) {
	PebbleAppHandlers handlers = {
			.init_handler = &handle_init,

			.tick_info = {
					.tick_handler = &handle_minute_tick,
					.tick_units = MINUTE_UNIT
			}
	};
	app_event_loop(params, &handlers);
}

unsigned short get_display_hour(unsigned short hour) {

	if (clock_is_24h_style()) {
		return hour;
	}
	unsigned short display_hour = hour % 12;
	// Converts "0" to "12"
	return display_hour ? display_hour : 12;

}

void display_layer_update_callback(Layer *me, GContext* ctx) {
	(void)me;
	PblTm t;
	get_time(&t);
	char* hour = int_to_str(get_display_hour(t.tm_hour), hour);
	char* minute = int_to_str(t.tm_min, minute);
	int month = t.tm_mon;
	char* day = int_to_str(t.tm_mday, day);
	//generate the string for the qr code
	char codeIn[30];
	strcpy(codeIn, hour);
	strcat(codeIn, ":");
	strcat(codeIn, minute);
	strcat(codeIn, "\n");
	if(month == 1)
		strcat(codeIn, "Jan ");
	else if(month == 2)
		strcat(codeIn, "Feb ");
	else if(month == 3)
		strcat(codeIn, "Mar ");
	else if(month == 4)
		strcat(codeIn, "Apr ");
	else if(month == 5)
		strcat(codeIn, "May ");
	else if(month == 6)
		strcat(codeIn, "Jun ");
	else if(month == 7)
		strcat(codeIn, "Jul ");
	else if(month == 8)
		strcat(codeIn, "Aug ");
	else if(month == 9)
		strcat(codeIn, "Sep ");
	else if(month == 10)
		strcat(codeIn, "Oct ");
	else if(month == 11)
		strcat(codeIn, "Nov ");
	else
		strcat(codeIn, "Dec ");
	strcat(codeIn, day);
	strcat(codeIn, "\0");
	//Generate the QR code
	unsigned char qr_return_data[];
	int width = EncodeData(QR_LEVEL_L /** Level Low **/,
			QR_VERSION_S /** Small Version (version 0-9) **/,
			codeIn /** Input text **/,
			0 /** Let it determine the length by null termination **/,
			qr_return_data /** Return data character array **/);
	for(int i=0; i<width; i++){
		for(int j=0; j<width; j++){
			int bit = (width*i)+j;
			int value = qr_return_data[bit];

			//should draw the pixels to the screen
			GPoint* point = {j, i};
			if(value == 0)
				graphics_context_set_fill_color(ctx, GColorWhite);
			else
				graphics_context_set_fill_color(ctx, GColorBlack);
			graphics_draw_pixel(GContext *ctx, GPoint point);
		}
	}
}

void handle_minute_tick(AppContextRef ctx, PebbleTickEvent *t) {
	(void)t;
	(void)ctx;

	layer_mark_dirty(&display_layer);
}

/**
 * A method created by robhh to turn ints to strings.  He gave me a copy of it via pastebin.
 * This will convert a number between 0 and 255 into a string.
 */
char* int_to_str(int num, char *outbuf) {
	int digit, i, j=0;
	char buf[255];
	for (i=0; i<255; i++) {
		digit = num % 10;
		if ((num==0) && (i>0))
			break;
		else
			buf[i] = '0' + digit;

		num/=10;
	}
	buf[i--] = '\0';
	while (i>=0) {
		outbuf[j++] = buf[i--];
	}
	outbuf[j] = '\0';
	return outbuf;
}
