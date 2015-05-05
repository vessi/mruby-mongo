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

Database can be initialized via `Mongo::Client#[]` method or direct by `Mongo::Database.new(client)`. It awaits `Mongo::Client` instance as argument.

#### `#client`

This method provides reader for database's client.

#### `#collections`

This method returns array with collections stored within this database.

#### `#[]`

This method creates `Mongo::Collection` instance connected with this database.

### Collection

### Query

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
