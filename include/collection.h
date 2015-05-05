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
mrb_value mrb_mongo_collection_drop(mrb_state *mrb, mrb_value self);
mrb_value mrb_mongo_collection_update(mrb_state *mrb, mrb_value self);
mrb_value mrb_mongo_collection_query(mrb_state *mrb, mrb_value self);

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

mrb_value mrb_mongo_collection_insert(mrb_state *mrb, mrb_value self) {
  mrb_mongo_collection_data *data = DATA_PTR(self);
  mrb_value record_hash, inserted_hash;
  bson_t *doc;
  bson_oid_t oid;
  bson_error_t error;

  mrb_get_args(mrb, "H", &record_hash);

  doc = bson_new();
  mrb_hash_to_bson(mrb, record_hash, doc);

  //add id if not supplied
  if (!bson_has_field(doc, "_id")) {
    bson_oid_init(&oid, NULL);
    bson_append_oid(doc, "_id", -1, &oid);
  }

  if (!mongoc_collection_insert(data->collection, MONGOC_INSERT_NONE, doc, NULL, &error)) {
    bson_destroy(doc);
    mrb_raise(mrb, E_RUNTIME_ERROR, error.message);
  }

  inserted_hash = mrb_hash_new(mrb);
  bson_to_mrb_hash(mrb, doc, inserted_hash);
  bson_destroy(doc);

  return inserted_hash;
}

mrb_value mrb_mongo_collection_drop(mrb_state *mrb, mrb_value self) {
  mrb_mongo_collection_data *data = DATA_PTR(self);
  mrb_value drop_hash;
  bson_t *doc;
  bson_error_t error;

  mrb_get_args(mrb, "|H", &drop_hash);
  if (!mrb_hash_p(drop_hash)) {
    drop_hash = mrb_hash_new(mrb);
  }

  doc = bson_new();
  mrb_hash_to_bson(mrb, drop_hash, doc);

  if (!mongoc_collection_remove(data->collection, MONGOC_REMOVE_NONE, doc, NULL, &error)) {
    bson_destroy(doc);
    mrb_raise(mrb, E_RUNTIME_ERROR, error.message);
  }

  return mrb_nil_value();
}

mrb_value mrb_mongo_collection_update(mrb_state *mrb, mrb_value self) {
  mrb_mongo_collection_data *data = DATA_PTR(self);
  mrb_value selector_hash, update_hash;
  mrb_bool upsert;
  bson_t *selector, *update;
  bson_error_t error;
  mongoc_update_flags_t flags;

  flags = MONGOC_UPDATE_MULTI_UPDATE;

  mrb_get_args(mrb, "HH|b", &selector_hash, &update_hash, &upsert);
  if (!mrb_hash_p(selector_hash)) {
    selector_hash = mrb_hash_new(mrb);
  }
  if (upsert) flags = flags | MONGOC_UPDATE_UPSERT;

  selector = bson_new();
  update = bson_new();
  mrb_hash_to_bson(mrb, selector_hash, selector);
  mrb_hash_to_bson(mrb, update_hash, update);

  if (!mongoc_collection_update(data->collection, flags, selector, update, NULL, &error)) {
    bson_destroy(selector);
    bson_destroy(update);
    mrb_raise(mrb, E_RUNTIME_ERROR, error.message);
  }

  bson_destroy(selector);
  bson_destroy(update);

  return mrb_nil_value();
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

#endif
