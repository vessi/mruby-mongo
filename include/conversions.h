/*void mrb_to_bson(mrb_state *mrb, char *key, mrb_value value, bson_t *doc) {
  char *str_value;
  int64_t fix_value;
  bool bool_value;
  double double_value;
  bson_t child;

  switch(mrb_type(value)) {
    case MRB_TT_STRING:
      str_value = mrb_str_to_cstr(mrb, value);
      bson_append_utf8(doc, key, -1, str_value, -1);
      break;
    case MRB_TT_FIXNUM:
      fix_value = mrb_fixnum(value);
      bson_append_int64(doc, key, -1, fix_value);
      break;
    case MRB_TT_FALSE:
    case MRB_TT_TRUE:
      bool_value = mrb_bool(value);
      bson_append_bool(doc, key, -1, bool_value);
      break;
    case MRB_TT_FLOAT:
      double_value = mrb_float(value);
      bson_append_double(doc, key, -1, double_value);
      break;
    case MRB_TT_ARRAY:
      bson_init(&child);
      mrb_ary_to_bson(mrb, value, &child);
      bson_append_array(doc, key, -1, &child);
      bson_free(&child);
      break;
    case MRB_TT_HASH:
      bson_init(&child);
      mrb_hash_to_bson(mrb, value, &child);
      bson_append_document(doc, key, -1, &child);
      bson_free(&child);
      break;
    default:
      break;    
  }
}

void mrb_ary_to_bson(mrb_state *mrb, mrb_value ary, bson_t *doc) {
  int len = RARRAY_LEN(ary);
  for (int i = 0; i < len; ++i) {
    mrb_value value = mrb_ary_ref(mrb, ary, i);
    char* key;
    mrb_to_bson(mrb, itoa(i, key, 10), value, doc);
  }
}

void mrb_hash_to_bson(mrb_state *mrb, mrb_value hash, bson_t *doc) {
  mrb_value ary = mrb_hash_keys(mrb, hash);
  int len = RARRAY_LEN(ary);
  for (int i = 0; i < len; ++i) {
    mrb_value key = mrb_ary_ref(mrb, ary, i);
    char * str_key = mrb_str_to_cstr(mrb, key);
    int key_len = strlen(str_key);
    mrb_value value = mrb_hash_get(mrb, hash, key);
    mrb_to_bson(mrb, str_key, value, doc);
  }
} */
