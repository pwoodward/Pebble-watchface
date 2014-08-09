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
  // Get weather update every 30 minutes
  if(tick_time->tm_min % 30 == 0) 
  {
    // Begin dictionary
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);

    // Add a key-value pair
    dict_write_uint8(iter, 0, 0);

    // Send the message!
    app_message_outbox_send();
  }
  
  // Update Battery state
  BatteryChargeState charge_state = battery_state_service_peek();
  snprintf(battery_buffer, sizeof(battery_buffer), "%d%%", charge_state.charge_percent);
  text_layer_set_text(power_layer, battery_buffer);
}

// AppMessage Callbacks
static void inbox_received_callback(DictionaryIterator *iterator, void *context) 
{
  // Store incoming information
  static char temperature_buffer[8];
  static char conditions_buffer[32];
  static char weather_buffer[40];
  
  // Read first item
  Tuple *t = dict_read_first(iterator);

  // For all items
  while(t != NULL) 
  {
    // Which key was received?
    switch(t->key) 
    {
      case KEY_TEMPERATURE:
        snprintf(temperature_buffer, sizeof(temperature_buffer), "%dC", (int)t->value->int32);
        break;
      case KEY_CONDITIONS:
        snprintf(conditions_buffer, sizeof(conditions_buffer), "%s", t->value->cstring);
        break;
      default:
        APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
        break;
    }

    // Look for next item
    t = dict_read_next(iterator);
  }
  // Assemble full string and display
  snprintf(weather_buffer, sizeof(weather_buffer), "%s\n%s", temperature_buffer, conditions_buffer);
  text_layer_set_text(weather_layer, weather_buffer);
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) 
{
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) 
{
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) 
{
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

void init()
{
  // Push the window to the front
  show_watch_face();
  tick_timer_service_subscribe(MINUTE_UNIT, (TickHandler) tick_handler);
  
  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  
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
