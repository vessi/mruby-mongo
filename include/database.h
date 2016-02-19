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

#ifndef _DATABASE_H
#define _DATABASE_H

mrb_value mrb_mongo_database_init(mrb_state *mrb, mrb_value self);
mrb_value mrb_mongo_database_get_client(mrb_state *mrb, mrb_value self);
mrb_value mrb_mongo_database_collections(mrb_state *mrb, mrb_value self);
mrb_value mrb_mongo_database_find_collection(mrb_state *mrb, mrb_value self);

#endif
