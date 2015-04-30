#include "mrb_mongo.h"

void mrb_mruby_mongo_gem_init(mrb_state* mrb) {
  struct RClass *class_mongo = mrb_define_module(mrb, "Mongo");

  struct RClass *class_mongo_bson = mrb_define_class_under(mrb, class_mongo, "Bson", mrb->object_class);
  mrb_define_method(mrb, class_mongo_bson, "initialize", mrb_mongo_bson_init, MRB_ARGS_OPT(1));
  mrb_define_method(mrb, class_mongo_bson, "to_json", mrb_mongo_to_json, MRB_ARGS_NONE());

  struct RClass *class_mongo_client = mrb_define_class_under(mrb, class_mongo, "Client", mrb->object_class);
  mrb_define_method(mrb, class_mongo_client, "initialize", mrb_mongo_client_init, MRB_ARGS_OPT(1));
  mrb_define_method(mrb, class_mongo_client, "databases", mrb_mongo_client_databases, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_mongo_client, "[]", mrb_mongo_client_find_database, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_mongo_client, "address", mrb_mongo_client_get_address, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_mongo_client, "database_exists?", mrb_mongo_client_database_exists, MRB_ARGS_REQ(1));

  struct RClass *class_mongo_database = mrb_define_class_under(mrb, class_mongo, "Database", mrb->object_class);
  mrb_define_method(mrb, class_mongo_database, "initialize", mrb_mongo_database_init, MRB_ARGS_REQ(2));
  mrb_define_method(mrb, class_mongo_database, "client", mrb_mongo_database_get_client, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_mongo_database, "collections", mrb_mongo_database_collections, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_mongo_database, "[]", mrb_mongo_database_find_collection, MRB_ARGS_REQ(1));

  struct RClass *class_mongo_collection = mrb_define_class_under(mrb, class_mongo, "Collection", mrb->object_class);
  mrb_define_method(mrb, class_mongo_collection, "initialize", mrb_mongo_collection_init, MRB_ARGS_REQ(2));
  mrb_define_method(mrb, class_mongo_collection, "database", mrb_mongo_collection_get_database, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_mongo_collection, "count", mrb_mongo_collection_count, MRB_ARGS_NONE());
  //mrb_define_method(mrb, class_mongo_collection, "insert", mrb_mongo_collection_insert, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_mongo_collection, "where", mrb_mongo_collection_query, MRB_ARGS_OPT(1));

  struct RClass *class_mongo_query  = mrb_define_class_under(mrb, class_mongo, "Query", mrb->object_class);
  mrb_define_method(mrb, class_mongo_query, "initialize", mrb_mongo_query_init, MRB_ARGS_REQ(2));
  mrb_define_method(mrb, class_mongo_query, "sort", mrb_mongo_query_sort, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_mongo_query, "limit", mrb_mongo_query_limit, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_mongo_query, "skip", mrb_mongo_query_skip, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_mongo_query, "count", mrb_mongo_query_count, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_mongo_query, "to_a", mrb_mongo_query_to_array, MRB_ARGS_NONE());

  //struct RClass *class_mongo_record = mrb_define_class_under(mrb, class_mongo, "Record", mrb->object_class);

  //struct RClass *class_mongo_cursor = mrb_define_class_under(mrb, class_mongo, "Cursor", mrb->object_class);

  mongoc_init();

  DONE;
}

void mrb_mruby_mongo_gem_final(mrb_state* mrb) {
  mongoc_cleanup();
}
