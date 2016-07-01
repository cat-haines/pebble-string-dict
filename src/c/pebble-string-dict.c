#include "pebble-string-dict.h"

typedef struct StringDictTuple {
  char *key;
  void *data;
} StringDictTuple;

static bool prv_string_dict_find(void* key, void* data) {
  StringDictTuple* tuple = data;
  return (strcmp(tuple->key, (char*) key) == 0);
}

static bool prv_string_dict_free_node(void* obj, void* context) {
  if (!obj) return true;

  StringDictTuple* tuple = obj;
  if (tuple->key) free(tuple->key);
  if (tuple->data) free(tuple->data);

  tuple->key = NULL;
  tuple->data = NULL;

  free(tuple);
  tuple = NULL;

  return true;
}

StringDict* string_dict_create() {
  StringDict* this = (StringDict*) linked_list_create_root();
  return this;
}

void string_dict_destroy(StringDict *this) {
  if (!this) return;
  linked_list_foreach(this, prv_string_dict_free_node, NULL);
  linked_list_clear(this);
  free(this);
  this = NULL;
}

void string_dict_write_data(StringDict* this, char* key, uint16_t length, void* data) {
  int16_t list_key = linked_list_find_compare(this, key, prv_string_dict_find);
  StringDictTuple* tuple;

  if (list_key == -1) {
    tuple = malloc(sizeof(StringDictTuple));

    tuple->key = malloc(strlen(key)+1);
    strcpy(tuple->key, key);
    linked_list_append(this, tuple);
  } else {
    tuple = linked_list_get(this, list_key);
    if (tuple->data) free(tuple->data);
  }

  if (length > 0) {
    tuple->data = malloc(length);
    memcpy(tuple->data, data, length);
  }
}

void string_dict_write_null(StringDict* this, char* key) {
  string_dict_write_data(this, key, 0, NULL);
}

void string_dict_write_bool(StringDict* this, char* key, bool data) {
  uint8_t val = data ? 1 : 0;
  string_dict_write_data(this, key, sizeof(uint8_t), &val);
}

void string_dict_write_int(StringDict* this, char* key, int32_t data) {
  int32_t* val = &data;
  string_dict_write_data(this, key, sizeof(int32_t), val);
}

void string_dict_write_string(StringDict* this, char* key, char* data) {
  string_dict_write_data(this, key, strlen(data) + 1, data);
}

bool string_dict_has_key(StringDict* this, char* key) {
  return linked_list_find_compare(this, key, prv_string_dict_find) != -1;
}

void* string_dict_get_data(StringDict *this, char* key, uint16_t length) {
  int16_t list_key = linked_list_find_compare(this, key, prv_string_dict_find);
  if (list_key == -1) {
    // Not Found
    APP_LOG(APP_LOG_LEVEL_WARNING, "Could not find %s in StringDict", key);
    return NULL;
  }

  StringDictTuple* tuple = linked_list_get(this, list_key);

  char* data = tuple->data;
  return tuple->data;
}

bool string_dict_get_bool(StringDict *this, char* key) {
  void* data = string_dict_get_data(this, key, 1);
  if (data == NULL) return false;

  return (*((uint8_t *) data)) == 1;
}

uint32_t string_dict_get_int(StringDict *this, char* key) {
  void* data = string_dict_get_data(this, key, sizeof(int32_t));
  if (data == NULL) return 0;

  return (*((int32_t *) data));
}

char* string_dict_get_string(StringDict *this, char* key) {
  char* data = (char*) string_dict_get_data(this, key, 14);
  return data;
}