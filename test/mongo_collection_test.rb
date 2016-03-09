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

assert '' do
end
