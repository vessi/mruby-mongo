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
