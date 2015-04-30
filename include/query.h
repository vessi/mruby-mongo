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
#include "conversions.h"

#ifndef _QUERY_H
#define _QUERY_H

mrb_value mrb_mongo_query_init(mrb_state *mrb, mrb_value self);
mrb_value mrb_mongo_query_sort(mrb_state *mrb, mrb_value self);
mrb_value mrb_mongo_query_limit(mrb_state *mrb, mrb_value self);
mrb_value mrb_mongo_query_skip(mrb_state *mrb, mrb_value self);
mrb_value mrb_mongo_query_fields(mrb_state *mrb, mrb_value self);
mrb_value mrb_mongo_query_count(mrb_state *mrb, mrb_value self);
mrb_value mrb_mongo_query_to_array(mrb_state *mrb, mrb_value self);

mrb_value mrb_mongo_query_init(mrb_state *mrb, mrb_value self) {
  mrb_value collection;
  mrb_value query_hash;

  //parse args
  mrb_get_args(mrb, "oH", &collection, &query_hash);

  //set internal variables
  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "collection"), collection);
  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "query_hash"), query_hash);
  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "sort_hash"), mrb_nil_value());
  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "fields_hash"), mrb_nil_value());
  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "skip"), mrb_fixnum_value(0));
  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "limit"), mrb_fixnum_value(0));
  
  return self;
}

mrb_value mrb_mongo_query_sort(mrb_state *mrb, mrb_value self) {
  mrb_value sort_hash, query;

  mrb_get_args(mrb, "H", &sort_hash);

  query = mrb_obj_dup(mrb, self);

  mrb_iv_set(mrb, query, mrb_intern_lit(mrb, "sort_hash"), sort_hash);

  return query;
}

mrb_value mrb_mongo_query_limit(mrb_state *mrb, mrb_value self) {
  mrb_value limit, query;

  mrb_get_args(mrb, "i", &limit);

  query = mrb_obj_dup(mrb, self);

  mrb_iv_set(mrb, query, mrb_intern_lit(mrb, "limit"), mrb_fixnum_value(mrb_fixnum(limit)));

  return query;
}

mrb_value mrb_mongo_query_skip(mrb_state *mrb, mrb_value self) {
  mrb_value skip, query;

  mrb_get_args(mrb, "i", &skip);

  query = mrb_obj_dup(mrb, self);

  mrb_iv_set(mrb, query, mrb_intern_lit(mrb, "skip"), mrb_fixnum_value(mrb_fixnum(skip)));

  return query;
}

mrb_value mrb_mongo_query_fields(mrb_state *mrb, mrb_value self) {
  mrb_value fields_ary, fields_hash, tmp_field, query;
  int len, i;

  mrb_get_args(mrb, "o", &fields_ary);

  switch (mrb_type(fields_ary)) {
    case MRB_TT_ARRAY:
      len = RARRAY_LEN(fields_ary);
      fields_hash = mrb_hash_new(mrb);
      for (i=0; i < len; i++) {
        tmp_field = mrb_ary_ref(mrb, fields_ary, i);
        mrb_hash_set(mrb, fields_hash, tmp_field, mrb_fixnum_value(1));
      }
      break;
    case MRB_TT_HASH:
      fields_hash = mrb_obj_dup(mrb, fields_ary);
      break;
    default:
      mrb_raise(mrb, E_TYPE_ERROR, "Argument should be Array or Hash");
      break;
  }

  query = mrb_obj_dup(mrb, self);
  mrb_iv_set(mrb, query, mrb_intern_lit(mrb, "fields_hash"), fields_hash);

  return query;
}

mrb_value mrb_mongo_query_count(mrb_state *mrb, mrb_value self) {
  mrb_mongo_collection_data *collection_data;
  mrb_value collection, query_hash;
  bson_t *query;
  bson_error_t error;
  int64_t count;

  collection = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "collection"));
  collection_data = (mrb_mongo_collection_data*)DATA_PTR(collection);

  query_hash = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "query_hash"));
  query = bson_new();
  mrb_hash_to_bson(mrb, query_hash, query);

  count = mongoc_collection_count(collection_data->collection, MONGOC_QUERY_NONE, query, 0, 0, NULL, &error);

  bson_destroy(query);

  return mrb_fixnum_value(count);
}

mrb_value mrb_mongo_query_to_array(mrb_state *mrb, mrb_value self) {
  mrb_mongo_collection_data *collection_data;
  mongoc_cursor_t *cursor;
  bson_t *query, *fields;
  const bson_t *doc;
  mrb_value collection, query_hash, fields_hash, sort_hash, result_hash, mrb_result, mrb_doc;

  collection  = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "collection"));
  query_hash  = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "query_hash"));
  fields_hash = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "fields_hash"));
  sort_hash   = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "sort_hash"));

  //create result_hash
  if (mrb_nil_p(sort_hash)) {
    result_hash = query_hash;
  } else {
    result_hash = mrb_hash_new(mrb);
    mrb_hash_set(mrb, result_hash, mrb_str_new_cstr(mrb, "$query"), query_hash);
    mrb_hash_set(mrb, result_hash, mrb_str_new_cstr(mrb, "$orderby"), sort_hash);
  }

  query = bson_new();
  mrb_hash_to_bson(mrb, result_hash, query);
  fields = NULL;
  if (!mrb_nil_p(fields_hash)) {
    fields = bson_new();
    mrb_hash_to_bson(mrb, fields_hash, fields);
  }
  collection_data = (mrb_mongo_collection_data*)DATA_PTR(collection);

  cursor = mongoc_collection_find(
    collection_data->collection,
    MONGOC_QUERY_NONE,
    mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "skip"))),
    mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "limit"))),
    0,
    query,
    fields,
    NULL
  );

  mrb_result = mrb_ary_new(mrb);

  while (mongoc_cursor_more(cursor) && mongoc_cursor_next(cursor, &doc)) {
    mrb_doc = mrb_hash_new(mrb);
    bson_to_mrb_hash(mrb, doc, mrb_doc);
    mrb_ary_push(mrb, mrb_result, mrb_doc);
  }

  mongoc_cursor_destroy(cursor);
  if (fields) {
    bson_destroy(fields);
  }
  bson_destroy(query);

  return mrb_result;
}

#endif
