#ifndef __DATA_H__
#define __DATA_H__

#include "cJSON.h"


// global JSON data

extern cJSON *data;     // --- root
//extern cJSON *id;       //  |--- device id (string)
//extern cJSON *raw;      //  |--- scale sensor raw data (number)


// JSON related functions

void parse_json(const char *json_string);
void init_json_data(const char *device_id);


// data specific functions

void put_raw_scale(uint32_t raw_scale_value);
void change_device_id(char *device_id);


// JSON debug

int printf_json(cJSON* json);


#endif // __DATA_H__
