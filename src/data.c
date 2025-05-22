#include "include.h"


// global JSON data

cJSON *data = NULL;     // --- root
cJSON *id = NULL;       //  |--- device id (string)
cJSON *raw = NULL;      //  |--- scale sensor raw data (number)


// JSON related functions

void parse_json(const char *json_string) {
  // Parse the JSON string
  cJSON *json = cJSON_Parse(json_string);
  if (json == NULL) {
    printf("Error parsing JSON\n");
    return;
  }

  // Extract values from JSON
  cJSON *name = cJSON_GetObjectItem(json, "name");
  cJSON *age = cJSON_GetObjectItem(json, "age");

  if (cJSON_IsString(name) && name->valuestring != NULL) {
    printf("Name: %s\n", name->valuestring);
  }
  if (cJSON_IsNumber(age)) {
    printf("Age: %f\n", age->valuedouble);
  }

  printf(cJSON_PrintUnformatted(json));
  cJSON_Delete(json);
}

void init_json_data(const char *device_id) {
  data = cJSON_CreateObject();
  id = cJSON_CreateString(device_id);
  cJSON_AddItemToObject(data, "id", id);
  raw = cJSON_CreateNumber(0);
  cJSON_AddItemToObject(data, "raw", raw);
}


// data related fucntions

void put_raw_scale(uint32_t raw_scale_value) {
  cJSON_SetNumberValue(raw, raw_scale_value);
}

void change_device_id(char *device_id){
  if ((id != NULL) && (id->type == cJSON_String) && (id->valuestring != NULL)) {
    free(id->valuestring);
    id->valuestring = strdup(device_id);
  }
}


// JSON debug

int printf_json(cJSON* json) {
  char *str = cJSON_PrintUnformatted(json);
  printf("%s", str);
  int size = strlen(str);
  free(str);
  return size;
}
