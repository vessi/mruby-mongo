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

mrb_value mrb_mongo_collection_init(mrb_state *mrb, mrb_value self);
mrb_value mrb_mongo_collection_get_database(mrb_state *mrb, mrb_value self);
mrb_value mrb_mongo_collection_count(mrb_state *mrb, mrb_value self);
mrb_value mrb_mongo_collection_insert(mrb_state *mrb, mrb_value self);

mrb_value mrb_mongo_collection_init(mrb_state *mrb, mrb_value self) {
  mrb_value database, collection_name;
  mrb_mongo_database_data *db_data;
  mrb_mongo_collection_data *data;
  mongoc_collection_t *collection;

  //parse args
  mrb_get_args(mrb, "oS", &database, &collection_name);

  //prepare data structure
  data = (mrb_mongo_collection_data*)DATA_PTR(self);
  if (data) {
    mrb_free(mrb, data);
  }
  DATA_TYPE(self) = &mrb_mongo_collection_data_type;
  DATA_PTR(self) = NULL;
  data = (mrb_mongo_collection_data*)mrb_malloc(mrb, sizeof(mrb_mongo_collection_data));

  //get db data
  db_data = (mrb_mongo_database_data *)DATA_PTR(database);

  collection = mongoc_database_get_collection(db_data->db, mrb_str_to_cstr(mrb, collection_name));

  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "database"), database);
  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "name"), collection_name);

  //set data structure
  data->collection = collection;
  DATA_PTR(self) = data;

  return self;
}

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

mrb_value mrb_mongo_collection_query(mrb_state *mrb, mrb_value self) {
  mrb_value hash_query, query;
  mrb_value query_params[2];
  
  mrb_get_args(mrb, "|H", &hash_query);
  if (!mrb_hash_p(hash_query)) {
    hash_query = mrb_hash_new(mrb);
  }

  struct RClass *_class_mongo = mrb_module_get(mrb, "Mongo");
  struct RClass *_class_mongo_query = mrb_class_ptr(mrb_const_get(mrb, mrb_obj_value(_class_mongo), mrb_intern_lit(mrb, "Query")));

  query_params[0] = self;
  query_params[1] = hash_query;

  query = mrb_obj_new(mrb, _class_mongo_query, 2, query_params);

  return query;
}

/*mrb_value mrb_mongo_collection_insert(mrb_state *mrb, mrb_value self) {
  mrb_mongo_collection_data *data = DATA_PTR(self);
  struct RClass *_class_mongo = mrb_module_get(mrb, "Mongo");
  struct RClass *_class_mongo_record = mrb_class_ptr(mrb_const_get(mrb, mrb_obj_value(_class_mongo), mrb_intern_lit(mrb, "Record")));
  mrb_value record;

  record = mrb_obj_new(mrb, _class_mongo_record, 0, NULL);
  return record;
}*/
#endif
