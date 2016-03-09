assert 'check default log level' do
  Mongo.log_level == 0
end

assert 'it should properly encode hash to bson and json' do
  (Mongo::Bson.new({key: "value"}).to_json == '{ "key" : "value" }') and (Mongo::Bson.new.to_json == '{ }')
end
