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

assert 'it should return proper count for collection (also test for insert and drop)' do
  client = Mongo::Client.new
  db = client['test']
  collection = db['test']
  collection.drop
  collection.insert(test: 'ok')
  collection.where.count == 1
end

assert 'it should return empty array if documents not found' do
  client = Mongo::Client.new
  db = client['test']
  collection = db['test']
  collection.where(test: 'not ok').to_a == []
end

assert 'it should allow proper skips, limits and sorting' do
  client = Mongo::Client.new
  db = client['test']
  collection = db['test']
  collection.drop
  5.times do |t|
    collection.insert(order: t)
  end
  count_correct = collection.where.count == 5
  # I used limit.to_a here because limit does not creates new query, and count applies to query
  limit_correct = collection.where.limit(1).to_a.length == 1
  skip_correct = collection.where.limit(1).to_a != collection.where.limit(1).skip(1).to_a
  sort_correct = collection.where.sort(order: -1).to_a.first["order"] == 4
  count_correct and limit_correct and skip_correct and sort_correct
end

assert 'it should properly encode hash to bson and json' do
  (Mongo::Bson.new({key: "value"}).to_json == '{ "key" : "value" }') and (Mongo::Bson.new.to_json == '{ }')
end
