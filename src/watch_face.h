#ifndef WATCH_FACE_H
#define WATCH_FACE_H

#include <pebble.h>
  
void show_watch_face(void);
void hide_watch_face(void);

extern TextLayer *time_layer;
extern TextLayer *date_layer;
extern TextLayer *weather_layer;
extern TextLayer *power_layer;

// AppMessage Keys
#define KEY_TEMPERATURE 0
#define KEY_CONDITIONS 1

#endif /* WATCH_FACE_H */