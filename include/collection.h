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
#include "types.h"

#ifndef _COLLECTION_H
#define _COLLECTION_H

mrb_value mrb_mongo_collection_get_database(mrb_state *mrb, mrb_value self);
mrb_value mrb_mongo_collection_count(mrb_state *mrb, mrb_value self);
mrb_value mrb_mongo_collection_insert(mrb_state *mrb, mrb_value self);

mrb_value mrb_mongo_collection_get_database(mrb_state *mrb, mrb_value self) {
  mrb_value database = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "database"));
  return database;
}

mrb_value mrb_mongo_collection_count(mrb_state *mrb, mrb_value self) {
  mrb_mongo_collection_data *data = DATA_PTR(self);
  int64_t count;
  bson_t query;
  bson_error_t error;

  bson_init(&query);
  count = mongoc_collection_count(data->collection, MONGOC_QUERY_NONE, &query, 0, 0, NULL, &error);
  bson_destroy(&query);
  if (count < 0) {
    mrb_raise(mrb, E_RUNTIME_ERROR, error.message);
  }
  return mrb_fixnum_value(count);
}

mrb_value mrb_mongo_collection_find(mrb_state *mrb, mrb_value self) {
  mrb_mongo_collection_data *data = DATA_PTR(self);
  bson_t query;
  mrb_value hash_query;

  bson_init(&query);
  
  if (!(mrb_get_args(mrb, "|H", &hash_query) == 0)){

  }

}

mrb_value mrb_mongo_collection_insert(mrb_state *mrb, mrb_value self) {
  mrb_mongo_collection_data *data = DATA_PTR(self);
  struct RClass *_class_mongo = mrb_module_get(mrb, "Mongo");
  struct RClass *_class_mongo_record = mrb_class_ptr(mrb_const_get(mrb, mrb_obj_value(_class_mongo), mrb_intern_lit(mrb, "Record")));
  mrb_value record;

  record = mrb_class_new_instance(mrb, 0, NULL, _class_mongo_record);
  return record;
}
#endif
