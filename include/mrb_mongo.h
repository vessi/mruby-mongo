#include <errno.h>
#include <memory.h>
#include <mongoc.h>
#include <mruby.h>
#include <stdio.h>
#include "logger.h"
#include "bson.h"
#include "client.h"
#include "database.h"
#include "collection.h"
#include "query.h"

#define DONE mrb_gc_arena_restore(mrb, 0);

void mrb_mruby_mongo_gem_init(mrb_state* mrb);
void mrb_mruby_mongo_gem_final(mrb_state* mrb);
