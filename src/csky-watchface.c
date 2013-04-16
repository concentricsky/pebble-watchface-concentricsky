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
             "Concentric Sky", "Concentric Sky",
             1, 0, /* App version */
             RESOURCE_ID_IMAGE_MENU_ICON,
             APP_INFO_WATCH_FACE);

Window window;

void handle_second_tick(AppContextRef ctx, PebbleTickEvent *t) {
  (void)t;

  PblTm tim;
  get_time(&tim);

}


void handle_init(AppContextRef ctx) {
  (void)ctx;

  window_init(&window, "Concentric Sky");
  window_stack_push(&window, true);

  resource_init_current_app(&APP_RESOURCES);

  // setup background layer
  // bmp_init_container(RESOURCE_ID_IMAGE_BACKGROUND, &date_container);
  // layer_add_child(&window.layer, &date_container.layer.layer);


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
