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

#ifndef _COLLECTION_H
#define _COLLECTION_H

mrb_value mrb_mongo_collection_init(mrb_state *mrb, mrb_value self);
mrb_value mrb_mongo_collection_get_database(mrb_state *mrb, mrb_value self);
mrb_value mrb_mongo_collection_count(mrb_state *mrb, mrb_value self);
mrb_value mrb_mongo_collection_insert(mrb_state *mrb, mrb_value self);
mrb_value mrb_mongo_collection_drop(mrb_state *mrb, mrb_value self);
mrb_value mrb_mongo_collection_update(mrb_state *mrb, mrb_value self);
mrb_value mrb_mongo_collection_query(mrb_state *mrb, mrb_value self);

#endif
