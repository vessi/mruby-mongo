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

static const struct mrb_data_type mrb_mongo_bson_data_type = {
  "mrb_mongo_bson_data", mrb_mongo_bson_data_free,
};

static const struct mrb_data_type mrb_mongo_client_data_type = {
  "mrb_mongo_client_data", mrb_mongo_client_data_free,
};

static const struct mrb_data_type mrb_mongo_database_data_type = {
  "mrb_mongo_database_data", mrb_mongo_database_data_free,
};

static const struct mrb_data_type mrb_mongo_collection_data_type = {
  "mrb_mongo_collection_data", mrb_mongo_collection_data_free,
};

#endif
