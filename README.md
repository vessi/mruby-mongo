# Mongo driver for mRuby
<!---[![Build Status](https://travis-ci.org/vessi/mruby-mongo.svg?branch=develop)](https://travis-ci.org/vessi/mruby-mongo)-->

## Usage

If you want to use mruby-mongo, just add 

```ruby
conf.gem :git => 'https://github.com/vessi/mruby-mongo.git'
```

to your `build_config.rb` file and do `make` to build or `make test` to do tests.

## Reference

### Client

#### Initialization

Client accepts MongoDB connection string as a parameter. If no string provided - `mongodb://localhost:27017` will be used.

#### `#databases`

This method returns array with databases stored on provided to client server(s).

#### `#[]`

This method creates `Mongo::Database` instance connected with this client.

#### `#address`

This method provides reader for client's address.

#### `database_exists?`

This method returns `true` if database exists on server

### Database

#### Initialization

Database can be initialized via `Mongo::Client#[]` method or direct by `Mongo::Database.new(client, name)`. It awaits `Mongo::Client` instance as argument.

#### `#client`

This method provides reader for database's client.

#### `#collections`

This method returns array with collections stored within this database.

#### `#[]`

This method creates `Mongo::Collection` instance connected with this database.

### Collection

#### Initialization

Collection can be initialized via `Mongo::Database#[]` method or direct by `Mongo::Collection.new(db, name)`. It awaits `Mongo::Database` instance as argument.

#### `#database`

This method provides reader for collection's database.

#### `#count`

This methods returns number of documents in collection.

#### `#insert`

This method awaits `Hash` instance as argument and returns inserted document with fulfilled `_id` field.

#### `#drop`

This method awaits optional `Hash` instance. This hash will be used as query for documents removing. Example:

```ruby
collection.drop(status: 'ok') # it should drop all documents with {status: 'ok'}
collection.drop # it should drop all documents in collection
```

#### `#update`

This methods awaits two required `Hash` arguments and one optional boolean.

```ruby
# format is corresponding to: http://docs.mongodb.org/v2.4/reference/method/db.collection.update
collection.update(query, update) # this should be executed as db.collection.update(query, update, false, true)
collection.update(query, update, true) # this should be executed as db.collection.update(query, update, true, true)
```

So, for `#update` method `multiple` flag is always set to true.

#### `#where`

This method instantiates `Mongo::Query` and awaits optional hash argument which should be used as query. If no argument passed - blank query should be used.

### Query

#### Initialization

Query can be initialized via `Mongo::Collection#where` or with `Mongo::Query.new(collection, query)`, where first argument is `Mongo::Collection` object and second argument is query hash.

#### `#sort`

Instantiates new `Mongo::Query` instance from existing but adding sort hash to it. Uses default MongoDB sorting rules.

#### `#limit`

Instantiates new `Mongo::Query` instance from existing but adding limit of documents to return to it.

#### `#skip`

Instantiates new `Mongo::Query` instance from existing but adding amount of documents to skip to it.

#### `#fields`

Instantiates new `Mongo::Query` instance from existing but adding "fields to return" hash to it.

#### `#count`

Returns number of documents which are satisfying query.

#### `#to_a`

Returns documents of query as array of hashes.

### BSON

BSON is data-type provided for testing hash-to-BSON conversions. You can take a closer look in `include/conversions.h`.

## Dependencies

To build this gem you should have [mongo-c-driver](https://github.com/mongodb/mongo-c-driver) installed at least with version 1.0.

This gem tests has next requirements:
- local mongo server, up and running on localhost:27017

## Contribution

If you want to contribute, please follow next instructions:

1. Fork project
1. Do your changes
1. Write tests for them
1. Do a pull-request

Any help would be greatly appreciated. I am not a C developer, I am much more familiar to Ruby, so code has bad style. Feel free to contribute.

## License

The MIT License (MIT)

Copyright (c) 2015 Mikhail Bortnyk(vessimir@gmail.com)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
