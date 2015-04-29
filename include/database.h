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

#ifndef _DATABASE_H
#define _DATABASE_H

mrb_value mrb_mongo_database_get_client(mrb_state *mrb, mrb_value self);
mrb_value mrb_mongo_database_collections(mrb_state *mrb, mrb_value self);
mrb_value mrb_mongo_database_find_collection(mrb_state *mrb, mrb_value self);

mrb_value mrb_mongo_database_get_client(mrb_state *mrb, mrb_value self) {
  mrb_value client = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "client"));
  return client;
}

mrb_value mrb_mongo_database_collections(mrb_state *mrb, mrb_value self) {
  mrb_value ary;
  char **strv;
  bson_error_t error;
  mrb_mongo_database_data *data = DATA_PTR(self);
  int i;

  //initiate reply array
  ary = mrb_ary_new(mrb);

  //get collection names
  strv = mongoc_database_get_collection_names(data->db, &error);
  if (!strv) { mrb_raise(mrb, E_RUNTIME_ERROR, error.message); }

  for (i = 0; strv[i]; i++) {
    int len;
    len = strlen(strv[i]);
    mrb_ary_push(mrb, ary, mrb_str_new(mrb, strv[i], len));
  }

  return ary;
}

mrb_value mrb_mongo_database_find_collection(mrb_state *mrb, mrb_value self) {
  mrb_value name, c;
  mrb_mongo_database_data *db_data = DATA_PTR(self);
  mrb_mongo_collection_data *collection_data;
  mongoc_collection_t *collection;
  struct RClass *_class_mongo, *_class_mongo_collection;

  mrb_get_args(mrb, "o", &name);
  switch(mrb_type(name)) {
    case MRB_TT_STRING:
      break;
    case MRB_TT_SYMBOL:
      name = mrb_obj_as_string(mrb, name);
      break;
    default:
      mrb_raise(mrb, E_RUNTIME_ERROR, "Collection name should be symbol or string");
  }

  collection = mongoc_database_get_collection(db_data->db, mrb_str_to_cstr(mrb, name));

  _class_mongo = mrb_module_get(mrb, "Mongo");
  _class_mongo_collection = mrb_class_ptr(mrb_const_get(mrb, mrb_obj_value(_class_mongo), mrb_intern_lit(mrb, "Collection")));
  c = mrb_class_new_instance(mrb, 0, NULL, _class_mongo_collection);

  DATA_TYPE(c) = &mrb_mongo_collection_data_type;
  DATA_PTR(c) = NULL;
  collection_data = (mrb_mongo_collection_data *)mrb_malloc(mrb, sizeof(mrb_mongo_collection_data));
  collection_data->collection = collection;
  DATA_PTR(c) = collection_data;

  mrb_iv_set(mrb, c, mrb_intern_lit(mrb, "database"), self);
  return c;
}
#endif
