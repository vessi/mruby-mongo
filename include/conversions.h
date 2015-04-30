#include <errno.h>
#include <memory.h>
#include <mongoc.h>
#include <mruby.h>
#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/hash.h>
#include <mruby/string.h>
#include <mruby/variable.h>
#include <stdio.h>
#include <stdlib.h>
#include "types.h"

#ifndef _CONVERSIONS_H
#define _CONVERSIONS_H

void mrb_to_bson(mrb_state *mrb, const char *key, mrb_value value, bson_t *doc);
void mrb_ary_to_bson(mrb_state *mrb, mrb_value ary, bson_t *doc);
void mrb_hash_to_bson(mrb_state *mrb, mrb_value hash, bson_t *doc);

void mrb_to_bson(mrb_state *mrb, const char *key, mrb_value value, bson_t *doc) {
  char *str_value;
  int64_t fix_value;
  bool bool_value;
  double double_value;
  mrb_value tmp_value;
  bson_t *child;

  tmp_value = value;

  switch(mrb_type(value)) {
    case MRB_TT_SYMBOL:
      tmp_value = mrb_obj_as_string(mrb, value);
    case MRB_TT_STRING:
      str_value = mrb_str_to_cstr(mrb, tmp_value);
      bson_append_utf8(doc, key, -1, str_value, -1);
      break;
    case MRB_TT_FIXNUM:
      fix_value = mrb_fixnum(value);
      bson_append_int64(doc, key, -1, fix_value);
      break;
    case MRB_TT_FALSE:
    case MRB_TT_TRUE:
      bool_value = mrb_bool(value);
      bson_append_bool(doc, key, -1, bool_value);
      break;
    case MRB_TT_FLOAT:
      double_value = mrb_float(value);
      bson_append_double(doc, key, -1, double_value);
      break;
    case MRB_TT_ARRAY:
      child = bson_new();
      mrb_ary_to_bson(mrb, value, child);
      bson_append_array(doc, key, -1, child);
      bson_destroy(child);
      break;
    case MRB_TT_HASH:
      child = bson_new();
      mrb_hash_to_bson(mrb, value, child);
      bson_append_document(doc, key, -1, child);
      bson_destroy(child);
      break;
    default:
      break;    
  }
}

void mrb_ary_to_bson(mrb_state *mrb, mrb_value ary, bson_t *doc) {
  int len = RARRAY_LEN(ary);
  const char* key;
  char buffer[16];
  mrb_value tmp_value;
  for (int i = 0; i < len; i++) {
    tmp_value = mrb_ary_ref(mrb, ary, i);
    //used bson_uint32_to_string from libbson due to https://api.mongodb.org/libbson/1.0.0/performance.html
    bson_uint32_to_string(i, &key, buffer, sizeof buffer);
    mrb_to_bson(mrb, key, tmp_value, doc);
  }
}

void mrb_hash_to_bson(mrb_state *mrb, mrb_value hash, bson_t *doc) {
  mrb_value ary = mrb_hash_keys(mrb, hash);
  int len = RARRAY_LEN(ary);
  char * str_key;

  for (int i = 0; i < len; ++i) {
    mrb_value key = mrb_ary_ref(mrb, ary, i);
    switch (mrb_type(key)) {
      case MRB_TT_STRING:
        str_key = mrb_str_to_cstr(mrb, key);
        break;
      case MRB_TT_SYMBOL:
        str_key = mrb_str_to_cstr(mrb, mrb_obj_as_string(mrb, key));
        break;
      default:
        mrb_raise(mrb, E_RUNTIME_ERROR, "keys should be only symbol or string");
        break;
    }
    mrb_value value = mrb_hash_get(mrb, hash, key);
    mrb_to_bson(mrb, str_key, value, doc);
  }
}
#endif
