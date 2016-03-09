assert 'it should return proper count for collection (also test for insert and drop)' do
  client = Mongo::Client.new
  db = client['test']
  collection = db['test']
  collection.drop
  collection.insert(test: 'ok')
  collection.where.count == 1
end
