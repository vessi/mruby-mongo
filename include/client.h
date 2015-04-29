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

#ifndef _CLIENT_H
#define _CLIENT_H

mrb_value mrb_mongo_client_init(mrb_state *mrb, mrb_value self);
mrb_value mrb_mongo_client_databases(mrb_state *mrb, mrb_value self);
mrb_value mrb_mongo_client_find_database(mrb_state *mrb, mrb_value self);
mrb_value mrb_mongo_client_get_address(mrb_state *mrb, mrb_value self);
mrb_value mrb_mongo_client_database_exists(mrb_state *mrb, mrb_value self);

mrb_value mrb_mongo_client_init(mrb_state *mrb, mrb_value self) {
  mrb_mongo_client_data *data;
  mongoc_client_t *client;
  mrb_value arg_db;
  bool server_status;
  bson_t mongoc_reply;
  bson_error_t mongoc_error;
  char* db_name = NULL;

  //parse args and set db name by default
  if (mrb_get_args(mrb, "|S", &arg_db) == 0) {
    db_name = "mongodb://localhost:27017";
  } else {
    db_name = mrb_str_to_cstr(mrb, arg_db);
  }

  //initiate mongoc client and set internal data
  data = (mrb_mongo_client_data *)DATA_PTR(self);
  if (data) {
    mrb_free(mrb, data);
  }
  DATA_TYPE(self) = &mrb_mongo_client_data_type;
  DATA_PTR(self) = NULL;
  client = mongoc_client_new(db_name);
  data = (mrb_mongo_client_data *)mrb_malloc(mrb, sizeof(mrb_mongo_client_data));
  data->client = client;
  DATA_PTR(self) = data;

  //check client connectivity
  server_status = mongoc_client_get_server_status(data->client, NULL, &mongoc_reply, &mongoc_error);
  if (!server_status) {
    bson_destroy(&mongoc_reply);
    mrb_raise(mrb, E_RUNTIME_ERROR, mongoc_error.message);
  }

  //free reply
  bson_destroy(&mongoc_reply);

  //set address to get connection address
  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "address"), mrb_str_new(mrb, db_name, strlen(db_name)));

  //return new client instance
  return self;
}

mrb_value mrb_mongo_client_databases(mrb_state *mrb, mrb_value self) {
  mrb_mongo_client_data *data = DATA_PTR(self);
  bson_error_t error;
  char **strv;
  mrb_value ary;
  int i;

  //initiate reply array
  ary = mrb_ary_new(mrb);

  //get database names
  strv = mongoc_client_get_database_names(data->client, &error);
  //raise error if getting database names failed
  if (!strv) { mrb_raise(mrb, E_RUNTIME_ERROR, error.message); }
  //populate array
  for (i = 0; strv[i]; i++) {
    int len;
    len = strlen(strv[i]);
    mrb_ary_push(mrb, ary, mrb_str_new(mrb, strv[i], len));
  }

  return ary;
}

mrb_value mrb_mongo_client_find_database(mrb_state *mrb, mrb_value self) {
  mrb_value name, c;
  mrb_mongo_client_data *data = DATA_PTR(self);
  mrb_mongo_database_data *db_data;
  mongoc_database_t * database;
  struct RClass *_class_mongo, *_class_mongo_database;

  mrb_get_args(mrb, "o", &name);
  switch(mrb_type(name)) {
    case MRB_TT_STRING:
      break;
    case MRB_TT_SYMBOL:
      name = mrb_obj_as_string(mrb, name);
      break;
    default:
      mrb_raise(mrb, E_RUNTIME_ERROR, "Database name should be symbol or string");
  }

  database = mongoc_client_get_database(data->client, mrb_str_to_cstr(mrb, name));

  _class_mongo = mrb_module_get(mrb, "Mongo");
  _class_mongo_database = mrb_class_ptr(mrb_const_get(mrb, mrb_obj_value(_class_mongo), mrb_intern_lit(mrb, "Database")));
  c = mrb_class_new_instance(mrb, 0, NULL, _class_mongo_database);

  DATA_TYPE(c) = &mrb_mongo_database_data_type;
  DATA_PTR(c) = NULL;
  db_data = (mrb_mongo_database_data *)mrb_malloc(mrb, sizeof(mrb_mongo_database_data));
  db_data->db = database;
  DATA_PTR(c) = db_data;

  mrb_iv_set(mrb, c, mrb_intern_lit(mrb, "client"), self);
  return c;
}

mrb_value mrb_mongo_client_get_address(mrb_state *mrb, mrb_value self) {
  return mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "address"));
}

mrb_value mrb_mongo_client_database_exists(mrb_state *mrb, mrb_value self) {
  mrb_value name;
  mrb_mongo_client_data *data = DATA_PTR(self);
  char **strv;
  bson_error_t error;
  char *db_name;

  mrb_get_args(mrb, "o", &name);
  switch(mrb_type(name)) {
    case MRB_TT_STRING:
      break;
    case MRB_TT_SYMBOL:
      name = mrb_obj_as_string(mrb, name);
      break;
    default:
      mrb_raise(mrb, E_RUNTIME_ERROR, "Database name should be symbol or string");
  }

  db_name = mrb_str_to_cstr(mrb, name);

  strv = mongoc_client_get_database_names(data->client, &error);
  //raise error if getting database names failed
  if (!strv) { mrb_raise(mrb, E_RUNTIME_ERROR, error.message); }
  for (int i = 0; strv[i]; i++) {
    if (strcmp(strv[i], db_name) == 0) return mrb_true_value();
  }
  return mrb_false_value();
}

#endif
