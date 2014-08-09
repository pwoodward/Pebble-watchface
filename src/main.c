#include <pebble.h>
#include "watch_face.h"

void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
  // Buffers
  static char time_buffer[] = "00:00";
  static char date_buffer[] = "Ddddddddd Mmmmmmmmm 00";
  static char battery_buffer[] = "100%";
  
  if (!tick_time) 
  {
    time_t now = time(NULL);
    tick_time = localtime(&now);
  }
  
  // Update Minute Layers
  if ((units_changed & MINUTE_UNIT) != 0)
  {
    // Format the time string
    strftime(time_buffer, sizeof(time_buffer), "%H:%M", tick_time);
  
    // Update the text layer
    text_layer_set_text(time_layer, time_buffer);
  }
  
  // Update Date Layer
  if ((units_changed & DAY_UNIT) != 0)
  {
    // Format the time string
    strftime(date_buffer, sizeof(date_buffer), "%A\n%B %d", tick_time);
  
    // Update the text layer
    text_layer_set_text(date_layer, date_buffer);
  }
  
  // Update weather layer
  
  // Update Battery state
  BatteryChargeState charge_state = battery_state_service_peek();
  snprintf(battery_buffer, sizeof(battery_buffer), "%d%%", charge_state.charge_percent);
  text_layer_set_text(power_layer, battery_buffer);
}

void init()
{
  // Push the window to the front
  show_watch_face();
  tick_timer_service_subscribe(MINUTE_UNIT, (TickHandler) tick_handler);
  // Get a time structure so that the face doesn't start blank
  struct tm *t;
  time_t temp;
  temp = time(NULL);
  t = localtime(&temp);
 
  // Manually call the tick handler when the window is loading
  tick_handler(t, MINUTE_UNIT | DAY_UNIT);
  
}

void deinit()
{
  // Cleanup
  tick_timer_service_unsubscribe();
  hide_watch_face();
}

int main(void)
{
  init();
  app_event_loop();
  deinit();
}
