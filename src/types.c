#include "types.h"

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
