assert 'connect client without params' do
  client = Mongo::Client.new()
  client.is_a?(Mongo::Client) and client.address == 'mongodb://localhost:27017'
end

assert 'connect client with params' do
  client = Mongo::Client.new("mongodb://localhost:27017")
  client.is_a?(Mongo::Client) and client.address == 'mongodb://localhost:27017'
end

assert 'it should raise Runtime Error if server(s) unreachable' do
  assert_raise RuntimeError do
    Mongo::Client.new("mongodb://localhost:29019")
  end
end

assert 'it should return database names' do
  client = Mongo::Client.new()
  client.databases.include? "test"
end

assert 'it should check if database exists' do
  client = Mongo::Client.new
  client.database_exists?("test") and not client.database_exists?("other_test")
end

assert 'it should return Mongo::Database instance for database' do
  client = Mongo::Client.new
  client['test'].is_a? Mongo::Database
end

assert 'it should return client initiated Mongo::Database instance' do
  client = Mongo::Client.new
  client['test'].client == client
end

assert 'it should return collection names' do
  client = Mongo::Client.new
  client['test'].collections.is_a?(Array)
  client['test'].collections.include? 'test'
end

assert 'it should return Mongo::Collection instance for collection' do
  client = Mongo::Client.new
  client['test']['test'].is_a? Mongo::Collection
end

assert 'it should return proper database for collection' do
  client = Mongo::Client.new
  db = client['test']
  collection = db['test']
  collection.database == db
end

=begin
assert 'it should allow insert record in collection' do
  client = Mongo::Client.new
  record = client['test']['test'].insert({test: 'ok'})
  (client['test']['test'].count == 1) and (record['test'] == 'ok')
end

assert 'it should allow to find one record' do
  client = Mongo::Client.new
  record = client['test']['test'].find({test: 'ok'}, limit: 1)
  (record != nil) and (record.is_a? Mongo::Record)
end

assert 'it should allow delete record from collection' do
  client = Mongo::Client.new
  record = client['test']['test'].find({test: 'ok'}, limit: 1)
  record.drop
  client['test']['test'].count == 0
end
=end

assert 'it should properly encode hash to bson and json' do
  assert { Mongo::Bson.new.to_json == '{}' }
  assert { Mongo::Bson.new({key: "value"}) == '{"key": "value"}' }
  true
end
