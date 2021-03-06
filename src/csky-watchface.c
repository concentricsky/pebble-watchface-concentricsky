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
             1, 3, /* App version */
             RESOURCE_ID_IMAGE_MENU_ICON,
             APP_INFO_WATCH_FACE);

Window window;
PblTm now;

TextLayer dateLayer;

BmpContainer largeImage;
Layer hourLayer;
GPath hour_segment_path;

RotBmpPairContainer smallImage;
Layer minuteLayer;
GPath minute_segment_path;



const GPoint HOUR_CENTER = {59,82};
const GPathInfo HOUR_SEGMENT_PATH_POINTS = {
  3,
  (GPoint []) {
    {0,0},
    {-12,-51},
    {11,-51},
  }
};


const GPoint MINUTE_CENTER = {109,106};
const GPathInfo MINUTE_SEGMENT_PATH_POINTS = {
  3,
  (GPoint []) {
    {0,0},
    {0,-27},
    {9,-27},
  }
};



void handle_minute_tick(AppContextRef ctx, PebbleTickEvent *t) {
  (void)t;
  (void)ctx;

  get_time(&now);

  //update the date layer
  static char dateText[25];
  string_format_time(dateText, 25, "%A %B %e", &now);
  text_layer_set_text(&dateLayer, dateText);


  //draw the hour hand
  layer_mark_dirty(&hourLayer);

}



void handle_hour_layer_update(Layer *me, GContext *ctx) {
  (void)me;

  graphics_context_set_fill_color(ctx, GColorBlack);

  for (int hour = 1; hour <= now.tm_hour % 12; hour++) {
    unsigned int angle = (hour % 12) * 30;
    gpath_rotate_to(&hour_segment_path, TRIG_MAX_ANGLE * angle / 360);
    gpath_draw_filled(ctx, &hour_segment_path);
  }

  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_circle(ctx, HOUR_CENTER, 12);


  graphics_fill_circle(ctx, MINUTE_CENTER, 27);
}

void handle_minute_layer_update(Layer *me, GContext *ctx) {
  (void)me;

  graphics_context_set_fill_color(ctx, GColorBlack);

  // for (int minute = 1; minute <= now.tm_min; minute++) {
  for (int minute = 1; minute <= now.tm_sec; minute++) {
    unsigned int angle = minute * 6;
    gpath_rotate_to(&minute_segment_path, TRIG_MAX_ANGLE * angle / 360);
    gpath_draw_filled(ctx, &minute_segment_path);
  }

  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_circle(ctx, MINUTE_CENTER, 6);
  graphics_draw_circle(ctx, MINUTE_CENTER, 27);
  graphics_draw_circle(ctx, MINUTE_CENTER, 28);
  graphics_draw_circle(ctx, MINUTE_CENTER, 29);
}



void handle_init(AppContextRef ctx) {
  (void)ctx;

  window_init(&window, "Concentric Sky Face");
  window_stack_push(&window, true);
  window_set_background_color(&window, GColorWhite);

  resource_init_current_app(&APP_RESOURCES);



  /* set up large circle layer
   */
  bmp_init_container(RESOURCE_ID_IMAGE_WATCHFACE_LG_CIRCLE, &largeImage);
  layer_add_child(&window.layer, &largeImage.layer.layer);

  /* set up the hour hand path
   */
  layer_init(&hourLayer, window.layer.frame);
  hourLayer.update_proc = handle_hour_layer_update;
  gpath_init(&hour_segment_path, &HOUR_SEGMENT_PATH_POINTS);
  gpath_move_to(&hour_segment_path, HOUR_CENTER);
  layer_add_child(&window.layer, &hourLayer);



  layer_init(&minuteLayer, window.layer.frame);
  minuteLayer.update_proc = handle_minute_layer_update;
  gpath_init(&minute_segment_path, &MINUTE_SEGMENT_PATH_POINTS);
  gpath_move_to(&minute_segment_path, MINUTE_CENTER);
  layer_add_child(&window.layer, &minuteLayer);

  /* set up small circle layer
   */
  rotbmp_pair_init_container(RESOURCE_ID_IMAGE_WATCHFACE_SM_CIRCLE_WHITE, RESOURCE_ID_IMAGE_WATCHFACE_SM_CIRCLE_BLACK, &smallImage);
  smallImage.layer.layer.frame.origin.x = 65;
  smallImage.layer.layer.frame.origin.y = 65;
  layer_add_child(&window.layer, &smallImage.layer.layer);




  /* set up date layer
   */
  text_layer_init(&dateLayer, GRect(1, 168-16, 144, 16));
  text_layer_set_text_color(&dateLayer, GColorBlack);
  text_layer_set_background_color(&dateLayer, GColorClear);
  text_layer_set_text_alignment(&dateLayer, GTextAlignmentCenter);
  text_layer_set_font(&dateLayer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  layer_add_child(&window.layer, &dateLayer.layer);


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
      .tick_units = MINUTE_UNIT
    }

  };
  app_event_loop(params, &handlers);
}
