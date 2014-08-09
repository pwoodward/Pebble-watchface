#include "watch_face.h"
#include <pebble.h>

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_roboto_bold_subset_49;
static GFont s_res_gothic_18_bold;
static InverterLayer *top_line;
static InverterLayer *bottom_line;

TextLayer *time_layer;
TextLayer *date_layer;
TextLayer *weather_layer;
TextLayer *power_layer;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_background_color(s_window, GColorBlack);
  window_set_fullscreen(s_window, true);
  
  s_res_roboto_bold_subset_49 = fonts_get_system_font(FONT_KEY_ROBOTO_BOLD_SUBSET_49);
  s_res_gothic_18_bold = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  // time_layer
  time_layer = text_layer_create(GRect(7, 50, 130, 60));
  text_layer_set_background_color(time_layer, GColorClear);
  text_layer_set_text_color(time_layer, GColorWhite);
  text_layer_set_text(time_layer, "00:00");
  text_layer_set_font(time_layer, s_res_roboto_bold_subset_49);
  layer_add_child(window_get_root_layer(s_window), (Layer *)time_layer);
  
  // date_layer
  date_layer = text_layer_create(GRect(10, 10, 124, 42));
  text_layer_set_background_color(date_layer, GColorClear);
  text_layer_set_text_color(date_layer, GColorWhite);
  text_layer_set_text(date_layer, "");
  text_layer_set_font(date_layer, s_res_gothic_18_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)date_layer);
  
  // top_line
  top_line = inverter_layer_create(GRect(7, 52, 130, 4));
  layer_add_child(window_get_root_layer(s_window), (Layer *)top_line);
  
  // bottom_line
  bottom_line = inverter_layer_create(GRect(7, 112, 130, 4));
  layer_add_child(window_get_root_layer(s_window), (Layer *)bottom_line);
  
  // weather_layer
  weather_layer = text_layer_create(GRect(10, 116, 80, 42));
  text_layer_set_background_color(weather_layer, GColorClear);
  text_layer_set_text_color(weather_layer, GColorWhite);
  text_layer_set_text(weather_layer, "Loading...");
  text_layer_set_font(weather_layer, s_res_gothic_18_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)weather_layer);
  
  // power_layer
  power_layer = text_layer_create(GRect(90, 138, 44, 20));
  text_layer_set_background_color(power_layer, GColorClear);
  text_layer_set_text_color(power_layer, GColorWhite);
  text_layer_set_text(power_layer, "");
  text_layer_set_text_alignment(power_layer, GTextAlignmentRight);
  layer_add_child(window_get_root_layer(s_window), (Layer *)power_layer);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(time_layer);
  text_layer_destroy(date_layer);
  inverter_layer_destroy(top_line);
  inverter_layer_destroy(bottom_line);
  text_layer_destroy(weather_layer);
  text_layer_destroy(power_layer);
}
// END AUTO-GENERATED UI CODE

static void handle_window_unload(Window* window) {
  destroy_ui();
}

void show_watch_face(void) {
  initialise_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
}

void hide_watch_face(void) {
  window_stack_remove(s_window, true);
}
