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
