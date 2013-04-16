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
PblTm now;

Layer hourLayer;
TextLayer dateLayer;
RotBmpPairContainer smallImage;
BmpContainer largeImage;

GPath hour_segment_path;


const GPathInfo HOUR_SEGMENT_PATH_POINTS = {
  3,
  (GPoint []) {
    {0,0},
    {-7,-50},
    {7,-50},
  }
};


void handle_minute_tick(AppContextRef ctx, PebbleTickEvent *t) {
  (void)t;
  (void)ctx;

  get_time(&now);

  //update the date layer
  static char dateText[25];
  string_format_time(dateText, 25, "%a %B %e %T", &now);
  text_layer_set_text(&dateLayer, dateText);


  //draw the hour hand
  layer_mark_dirty(&hourLayer);


  //rotate the small circle to the minute location
  unsigned int angle = (now.tm_min + 36)%60 * 6;
  rotbmp_pair_layer_set_angle(&smallImage.layer, TRIG_MAX_ANGLE * angle / 360);
  layer_mark_dirty(&smallImage.layer.layer);
}



void handle_hour_layer_update(Layer *me, GContext *ctx) {
  (void)me;

  graphics_context_set_fill_color(ctx, GColorWhite);

  unsigned int angle = (now.tm_hour % 12) * 30;
  gpath_rotate_to(&hour_segment_path, TRIG_MAX_ANGLE * angle / 360);

  gpath_draw_filled(ctx, &hour_segment_path);

}

void handle_init(AppContextRef ctx) {
  (void)ctx;

  window_init(&window, "Concentric Sky Face");
  window_stack_push(&window, true);
  window_set_background_color(&window, GColorBlack);

  resource_init_current_app(&APP_RESOURCES);


  /* set up date layer
   */
  text_layer_init(&dateLayer, GRect(1, 168-16, 144, 16));
  text_layer_set_text_color(&dateLayer, GColorWhite);
  text_layer_set_background_color(&dateLayer, GColorClear);
  text_layer_set_text_alignment(&dateLayer, GTextAlignmentCenter);
  text_layer_set_font(&dateLayer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  layer_add_child(&window.layer, &dateLayer.layer);


  /* set up large circle layer
   */
  bmp_init_container(RESOURCE_ID_IMAGE_LARGE_CIRCLE, &largeImage);
  largeImage.layer.layer.frame.origin.x = (144-largeImage.layer.layer.frame.size.w)/2;
  largeImage.layer.layer.frame.origin.y = (168-largeImage.layer.layer.frame.size.h)/2;
  layer_add_child(&window.layer, &largeImage.layer.layer);


  /* set up the hour hand path
   */
  layer_init(&hourLayer, window.layer.frame);
  hourLayer.update_proc = handle_hour_layer_update;
  gpath_init(&hour_segment_path, &HOUR_SEGMENT_PATH_POINTS);
  gpath_move_to(&hour_segment_path, grect_center_point(&largeImage.layer.layer.frame));
  layer_add_child(&window.layer, &hourLayer);


  /* set up small circle layer
   */
  rotbmp_pair_init_container(RESOURCE_ID_IMAGE_SMALL_CIRCLE_WHITE, RESOURCE_ID_IMAGE_SMALL_CIRCLE_BLACK, &smallImage);
  rotbmp_pair_layer_set_src_ic(&smallImage.layer, GPoint(1,1));
  // using set_src_ic tweaks the origin?
  // smallImage.layer.layer.frame.origin.x = (144/2) - (smallImage.layer.layer.frame.size.w/2);
  // smallImage.layer.layer.frame.origin.y = (168/2) - (smallImage.layer.layer.frame.size.h/2);
  layer_add_child(&window.layer, &smallImage.layer.layer);




  // do an initial update
  handle_minute_tick(ctx, NULL); 
}


void handle_deinit(AppContextRef ctx) {
  (void)ctx;

}


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
    .deinit_handler = &handle_deinit,

    .tick_info = {
      .tick_handler = &handle_minute_tick,
      .tick_units = SECOND_UNIT
    }

  };
  app_event_loop(params, &handlers);
}
