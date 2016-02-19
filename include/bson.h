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
#include <stdlib.h>

#ifndef _BSON_H
#define _BSON_H

mrb_value mrb_mongo_bson_init(mrb_state *mrb, mrb_value self);
mrb_value mrb_mongo_to_json(mrb_state *mrb, mrb_value self);

#endif
