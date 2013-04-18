#include "pebble_os.h"
#include "pebble_app.h"
//#include "QR_Encode.h"
#include "test.h"

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
	//SS ss = get_display_hour(t.tm_hour);
	char codeIn[20] ;
	//sprintf(codeIn, "%u:%u\n%u/%u/%u", hour, t.tm_min, t.tm_mon, t.tm_mday, t.tm_year);
	//strcpy(codeIn, hour);
}

void handle_minute_tick(AppContextRef ctx, PebbleTickEvent *t) {
	(void)t;
	(void)ctx; // TODO: Pass tick event/PblTime rather than make layer use `get_time()`?

	layer_mark_dirty(&display_layer);
}

