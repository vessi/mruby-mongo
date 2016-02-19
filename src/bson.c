#include "bson.h"
#include "conversions.h"
#include "types.h"

mrb_value mrb_mongo_bson_init(mrb_state *mrb, mrb_value self) {
  mrb_mongo_bson_data *data;
  mrb_value arg_hash;
  bson_t *bson;

  bson = bson_new();

  if (mrb_get_args(mrb, "|H", &arg_hash) == 0) {
  } else {
    mrb_hash_to_bson(mrb, arg_hash, bson);
  }

  //set internal data
  data = (mrb_mongo_bson_data *)DATA_PTR(self);
  if (data) {
    mrb_free(mrb, data);
  }
  DATA_TYPE(self) = &mrb_mongo_bson_data_type;
  DATA_PTR(self) = NULL;

  data = (mrb_mongo_bson_data *)mrb_malloc(mrb, sizeof(mrb_mongo_bson_data));
  data->bson = bson;
  DATA_PTR(self) = data;

  return self;
}

mrb_value mrb_mongo_to_json(mrb_state *mrb, mrb_value self) {
  mrb_mongo_bson_data *data = DATA_PTR(self);
  mrb_value str;
  char * json = NULL;
  size_t length;

  json = bson_as_json(data->bson, &length);
  str = mrb_str_new_cstr(mrb, json);
  bson_free(json);

  return str;
}
