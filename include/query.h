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

#ifndef _QUERY_H
#define _QUERY_H

mrb_value mrb_mongo_query_init(mrb_state *mrb, mrb_value self);
mrb_value mrb_mongo_query_sort(mrb_state *mrb, mrb_value self);
mrb_value mrb_mongo_query_limit(mrb_state *mrb, mrb_value self);
mrb_value mrb_mongo_query_skip(mrb_state *mrb, mrb_value self);
mrb_value mrb_mongo_query_fields(mrb_state *mrb, mrb_value self);
mrb_value mrb_mongo_query_count(mrb_state *mrb, mrb_value self);
mrb_value mrb_mongo_query_to_array(mrb_state *mrb, mrb_value self);

#endif
