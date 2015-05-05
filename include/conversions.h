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
mrb_value bson_to_mrb(mrb_state *mrb, const bson_value_t *value);
void bson_to_mrb_hash(mrb_state *mrb, const bson_t* doc, mrb_value hash);
void mrb_ary_to_bson(mrb_state *mrb, mrb_value ary, bson_t *doc);
void mrb_hash_to_bson(mrb_state *mrb, mrb_value hash, bson_t *doc);

void mrb_to_bson(mrb_state *mrb, const char *key, mrb_value value, bson_t *doc) {
  char *str_value;
  int64_t fix_value;
  uint64_t ufix_value;
  bool bool_value;
  double double_value;
  mrb_value tmp_value;
  bson_t *child;
  bson_oid_t oid_value;

  tmp_value = value;

  switch(mrb_type(value)) {
    case MRB_TT_SYMBOL:
      tmp_value = mrb_obj_as_string(mrb, value);
    case MRB_TT_STRING:
      str_value = mrb_str_to_cstr(mrb, tmp_value);
      //special case for oid:
      if ((strcmp(key, "_id") == 0) && (bson_oid_is_valid(str_value, strlen(str_value)))) {
        bson_oid_init_from_string(&oid_value, str_value);
        bson_append_oid(doc, key, -1, &oid_value);
      } else {
        bson_append_utf8(doc, key, -1, str_value, -1);
      }
      break;
    case MRB_TT_FIXNUM:
      fix_value = mrb_fixnum(value);
      bson_append_int64(doc, key, -1, fix_value);
      break;
    case MRB_TT_FALSE:
      if (mrb_nil_p(value)) {
        bson_append_null(doc, key, -1);
        break;
      }
    case MRB_TT_TRUE:
      bool_value = mrb_bool(value);
      bson_append_bool(doc, key, -1, bool_value);
      break;
    case MRB_TT_UNDEF:
      bson_append_null(doc, key, -1);
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
    case MRB_TT_DATA:
      if (strcmp(mrb_obj_classname(mrb, value), "Time") == 0) {
        ufix_value = mrb_fixnum(mrb_funcall_argv(mrb, value, mrb_intern_lit(mrb, "to_i"), 0, NULL)) / 0.001 +
                     mrb_fixnum(mrb_funcall_argv(mrb, value, mrb_intern_lit(mrb, "usec"), 0, NULL)) / 1000;
        bson_append_date_time(doc, key, -1, ufix_value);
        break;
      }
      printf("Don't know how to process instance of %s\n", mrb_obj_classname(mrb, value));
      break;
    default:
      printf("unrecognized mruby type %d\n", mrb_type(value));
      break;    
  }
}

mrb_value bson_to_mrb(mrb_state *mrb, const bson_value_t *value) {
  mrb_value tmp, tmp2, tmp_class, tmp_args_2[2];
  bson_t *tmp_bson;
  const bson_value_t *bson_value;
  bson_iter_t iter;
  char oid[25];
  bool test;
  int64_t timestamp;

  switch (value->value_type) {
    case BSON_TYPE_OID:
      bson_oid_to_string(&value->value.v_oid, oid);
      tmp = mrb_str_new_cstr(mrb, oid);
      break;
    case BSON_TYPE_UTF8:
      tmp = mrb_str_new_cstr(mrb, value->value.v_utf8.str);
      break;
    case BSON_TYPE_BOOL:
      tmp = mrb_bool_value(value->value.v_bool);
      break;
    case BSON_TYPE_NULL:
      tmp = mrb_nil_value();
      break;
    case BSON_TYPE_INT32:
      tmp = mrb_fixnum_value(value->value.v_int32);
      break;
    case BSON_TYPE_DOUBLE:
      tmp = mrb_float_value(mrb, value->value.v_double);
      break;
    case BSON_TYPE_DATE_TIME:
      timestamp = value->value.v_datetime;
      tmp_args_2[0] = mrb_float_value(mrb, timestamp / 1000);
      tmp_args_2[1] = mrb_float_value(mrb, timestamp % 1000);
      tmp_class = mrb_const_get(mrb, mrb_obj_value(mrb->object_class), mrb_intern_lit(mrb, "Time"));
      tmp = mrb_funcall_argv(mrb, tmp_class, mrb_intern_lit(mrb, "at"), 2, tmp_args_2);
      tmp = mrb_funcall(mrb, tmp, "utc", 0);
      break;
    case BSON_TYPE_ARRAY:
      tmp_bson = bson_new_from_data(value->value.v_doc.data, value->value.v_doc.data_len);
      tmp      = mrb_ary_new(mrb);
      test = bson_iter_init(&iter, tmp_bson);
      while (bson_iter_next(&iter)) {
        bson_value = bson_iter_value(&iter);
        tmp2 = bson_to_mrb(mrb, bson_value);
        mrb_ary_push(mrb, tmp, tmp2);
      }
      bson_destroy(tmp_bson);
      break;
    case BSON_TYPE_DOCUMENT:
      tmp_bson = bson_new_from_data(value->value.v_doc.data, value->value.v_doc.data_len);
      tmp      = mrb_hash_new(mrb);
      bson_to_mrb_hash(mrb, tmp_bson, tmp);
      break;
    case BSON_TYPE_SYMBOL:
      tmp = mrb_symbol_value(mrb_intern_cstr(mrb, value->value.v_symbol.symbol));
      break;
    default:
      printf("unrecognized type %#04x\n", value->value_type);
      tmp = mrb_nil_value();
      break;
  }
  return tmp;
}

void bson_to_mrb_hash(mrb_state *mrb, const bson_t* doc, mrb_value hash) {
  bson_iter_t iter;
  const bson_value_t *value;
  const char *key;
  mrb_value mrb_key;
  mrb_value mrb_value;

  bson_iter_init(&iter, doc);
  while (bson_iter_next(&iter)) {
    key = bson_iter_key(&iter);
    mrb_key = mrb_str_new_cstr(mrb, key);
    value = bson_iter_value(&iter);
    mrb_value = bson_to_mrb(mrb, value);
    mrb_hash_set(mrb, hash, mrb_key, mrb_value);
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
