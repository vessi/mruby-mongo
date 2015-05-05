#include <mongoc.h>
#include <mruby.h>
#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/hash.h>
#include <mruby/string.h>
#include <mruby/variable.h>

#ifndef _TYPES_H
#define _TYPES_H

typedef struct {
  bson_t *bson;
} mrb_mongo_bson_data;

typedef struct {
  mongoc_client_t *client;
} mrb_mongo_client_data;

typedef struct {
  mongoc_database_t *db;
} mrb_mongo_database_data;

typedef struct {
  mongoc_collection_t *collection;
} mrb_mongo_collection_data;

void mrb_mongo_bson_data_free(mrb_state *mrb, void *p);
void mrb_mongo_client_data_free(mrb_state *mrb, void *p);
void mrb_mongo_database_data_free(mrb_state *mrb, void *p);
void mrb_mongo_collection_data_free(mrb_state *mrb, void *p);

const struct mrb_data_type mrb_mongo_bson_data_type = {
  "mrb_mongo_bson_data", mrb_mongo_bson_data_free,
};

const struct mrb_data_type mrb_mongo_client_data_type = {
  "mrb_mongo_client_data", mrb_mongo_client_data_free,
};

const struct mrb_data_type mrb_mongo_database_data_type = {
  "mrb_mongo_database_data", mrb_mongo_database_data_free,
};

const struct mrb_data_type mrb_mongo_collection_data_type = {
  "mrb_mongo_collection_data", mrb_mongo_collection_data_free,
};

void mrb_mongo_bson_data_free(mrb_state *mrb, void *p) {
  mrb_mongo_bson_data *data = (mrb_mongo_bson_data*)p;
  bson_destroy(data->bson);
};

void mrb_mongo_client_data_free(mrb_state *mrb, void *p){
  mrb_mongo_client_data *data = (mrb_mongo_client_data*)p;
  mongoc_client_destroy(data->client);
};

void mrb_mongo_database_data_free(mrb_state *mrb, void *p){
  mrb_mongo_database_data *data = (mrb_mongo_database_data*)p;
  mongoc_database_destroy(data->db);
};

void mrb_mongo_collection_data_free(mrb_state *mrb, void *p){
  mrb_mongo_collection_data *data = (mrb_mongo_collection_data*)p;
  mongoc_collection_destroy(data->collection);
};

#endif
