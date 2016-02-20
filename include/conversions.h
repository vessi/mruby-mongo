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

#ifndef _CONVERSIONS_H
#define _CONVERSIONS_H

void mrb_to_bson(mrb_state *mrb, const char *key, mrb_value value, bson_t *doc);
mrb_value bson_to_mrb(mrb_state *mrb, const bson_value_t *value);
void bson_to_mrb_hash(mrb_state *mrb, const bson_t* doc, mrb_value hash);
void mrb_ary_to_bson(mrb_state *mrb, mrb_value ary, bson_t *doc);
void mrb_hash_to_bson(mrb_state *mrb, mrb_value hash, bson_t *doc);

#endif
