#include "logger.h"

void mrb_mongo_log_handler (mongoc_log_level_t log_level, const char *log_domain, const char *message,void *user_data) {
  int filter_level = (intptr_t) user_data;
  if (log_level <= filter_level) {
    mongoc_log_default_handler(log_level, log_domain, message, user_data);
  }
}

void mrb_mongo_set_log_handler_level(int log_level) {
  int i_log_level = log_level;
  mongoc_log_set_handler(mrb_mongo_log_handler, (void *) (intptr_t) i_log_level);
}

mrb_value mrb_mongo_get_log_level(mrb_state *mrb, mrb_value self) {
  mrb_value log_level;
  log_level = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "log_level"));
  // first reading of log_level should initialize it's value
  if (mrb_nil_p(log_level)) {
    mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "log_level"), mrb_fixnum_value(0));
    mrb_mongo_set_log_handler_level(0);
    return mrb_fixnum_value(0);
  }
  return log_level;
}

mrb_value mrb_mongo_set_log_level(mrb_state *mrb, mrb_value self) {
  mrb_value log_level;
  mrb_get_args(mrb, "i", &log_level);
  mrb_mongo_set_log_handler_level(mrb_fixnum(log_level));
  return log_level;
}
