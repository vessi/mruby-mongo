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
