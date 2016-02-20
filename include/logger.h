#include <errno.h>
#include <memory.h>
#include <mongoc.h>
#include <mruby.h>
#include <mruby/variable.h>
#include <stdio.h>

#ifndef _LOGGER_H
#define _LOGGER_H

void mrb_mongo_set_log_handler_level(int log_level);
mrb_value mrb_mongo_get_log_level(mrb_state *mrb, mrb_value self);
mrb_value mrb_mongo_set_log_level(mrb_state *mrb, mrb_value self);

#endif
