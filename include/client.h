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

#ifndef _CLIENT_H
#define _CLIENT_H

mrb_value mrb_mongo_client_init(mrb_state *mrb, mrb_value self);
mrb_value mrb_mongo_client_databases(mrb_state *mrb, mrb_value self);
mrb_value mrb_mongo_client_find_database(mrb_state *mrb, mrb_value self);
mrb_value mrb_mongo_client_get_address(mrb_state *mrb, mrb_value self);
mrb_value mrb_mongo_client_database_exists(mrb_state *mrb, mrb_value self);

#endif
