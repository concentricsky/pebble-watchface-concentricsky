/*

 Concentric Sky Watch Face


 Copyright 2013 Concentric Sky
 Author: Wiggins 
 */


#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { 0x31, 0x09, 0x0D, 0xE1, 0x82, 0x7E, 0x43, 0x43, 0xA5, 0x78, 0x34, 0x95, 0xA4, 0x4C, 0x22, 0x4B }
PBL_APP_INFO(MY_UUID,
             "ConcentricSky", "Concentric Sky",
             1, 1, /* App version */
             RESOURCE_ID_IMAGE_MENU_ICON,
             APP_INFO_WATCH_FACE);

Window window;

TextLayer dateLayer;

void handle_second_tick(AppContextRef ctx, PebbleTickEvent *t) {
  (void)t;
  (void)ctx;

  PblTm now;
  get_time(&now);
  static char dateText[12];

  string_format_time(dateText, 12, "%B %e", &now);
  text_layer_set_text(&dateLayer, dateText);


}


void handle_init(AppContextRef ctx) {
  (void)ctx;

  window_init(&window, "ConcentricSky");
  window_stack_push(&window, true);
  window_set_background_color(&window, GColorBlack);

  resource_init_current_app(&APP_RESOURCES);


  //set up date layer
  text_layer_init(&dateLayer, GRect(1, 168-16, 144, 16));
  text_layer_set_text_color(&dateLayer, GColorWhite);
  text_layer_set_background_color(&dateLayer, GColorClear);
  text_layer_set_text_alignment(&dateLayer, GTextAlignmentRight);
  text_layer_set_font(&dateLayer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  layer_add_child(&window.layer, &dateLayer.layer);



  // do an initial update
  handle_second_tick(ctx, NULL); 
}


void handle_deinit(AppContextRef ctx) {
  (void)ctx;

}


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
    .deinit_handler = &handle_deinit,

    .tick_info = {
      .tick_handler = &handle_second_tick,
      .tick_units = SECOND_UNIT
    }

  };
  app_event_loop(params, &handlers);
}
