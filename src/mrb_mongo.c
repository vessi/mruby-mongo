#include "mrb_mongo.h"

void mrb_mruby_mongo_gem_init(mrb_state* mrb) {
  struct RClass *class_mongo = mrb_define_module(mrb, "Mongo");
  mrb_define_const(mrb, class_mongo, "LOG_NONE", mrb_fixnum_value(-1));
  mrb_define_const(mrb, class_mongo, "LOG_ERROR", mrb_fixnum_value(0));
  mrb_define_const(mrb, class_mongo, "LOG_CRITICAL", mrb_fixnum_value(1));
  mrb_define_const(mrb, class_mongo, "LOG_WARNING", mrb_fixnum_value(2));
  mrb_define_const(mrb, class_mongo, "LOG_MESSAGE", mrb_fixnum_value(3));
  mrb_define_const(mrb, class_mongo, "LOG_INFO", mrb_fixnum_value(4));
  mrb_define_const(mrb, class_mongo, "LOG_DEBUG", mrb_fixnum_value(5));
  mrb_define_class_method(mrb, class_mongo, "log_level", mrb_mongo_get_log_level, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, class_mongo, "log_level=", mrb_mongo_set_log_level, MRB_ARGS_REQ(1));

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
  mrb_define_method(mrb, class_mongo_collection, "insert", mrb_mongo_collection_insert, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_mongo_collection, "drop", mrb_mongo_collection_drop, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_mongo_collection, "update", mrb_mongo_collection_update, MRB_ARGS_REQ(2));
  mrb_define_method(mrb, class_mongo_collection, "where", mrb_mongo_collection_query, MRB_ARGS_OPT(1));

  struct RClass *class_mongo_query  = mrb_define_class_under(mrb, class_mongo, "Query", mrb->object_class);
  mrb_define_method(mrb, class_mongo_query, "initialize", mrb_mongo_query_init, MRB_ARGS_REQ(2));
  mrb_define_method(mrb, class_mongo_query, "sort", mrb_mongo_query_sort, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_mongo_query, "limit", mrb_mongo_query_limit, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_mongo_query, "skip", mrb_mongo_query_skip, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_mongo_query, "fields", mrb_mongo_query_fields, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_mongo_query, "count", mrb_mongo_query_count, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_mongo_query, "to_a", mrb_mongo_query_to_array, MRB_ARGS_NONE());

  mongoc_init();
  mrb_mongo_set_log_handler_level(0);

  DONE;
}

void mrb_mruby_mongo_gem_final(mrb_state* mrb) {
  mongoc_cleanup();
}
