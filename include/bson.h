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

mrb_value mrb_mongo_bson_init(mrb_state *mrb, mrb_value self);

mrb_value mrb_mongo_bson_init(mrb_state *mrb, mrb_value self) {
  mrb_mongo_bson_data *data;
  mrb_value arg_hash;
  bson_t bson;

  if (mrb_get_args(mrb, "|H", &arg_hash) == 0) {
    
  } else {

  }
}
