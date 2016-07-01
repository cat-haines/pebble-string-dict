#pragma once
#include "@smallstoneapps/linked-list/linked-list.h"
#include "../src/h/pebble-string-dict-private.h"

typedef _StringDict StringDict;

StringDict* string_dict_create();
void string_dict_destroy(StringDict *this);

void string_dict_write_data(StringDict* this, char* key, uint16_t length, void* data);
void string_dict_write_null(StringDict* this, char* key);
void string_dict_write_bool(StringDict* this, char* key, bool data);
void string_dict_write_int(StringDict* this, char* key, int32_t data);
void string_dict_write_string(StringDict* this, char* key, char* data);

bool string_dict_has_key(StringDict* this, char* key);

void* string_dict_get_data(StringDict *this, char* key);
bool string_dict_get_bool(StringDict *this, char* key);
uint32_t string_dict_get_int(StringDict *this, char* key);
char* string_dict_get_string(StringDict *this, char* key);