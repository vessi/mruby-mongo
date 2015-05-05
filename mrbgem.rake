MRuby::Gem::Specification.new('mruby-mongo') do |spec|
  MONGO_LIB_VERSION = "1.0"
  spec.license = 'MIT'
  spec.author  = 'Mikhail Bortnyk'
  spec.summary = 'MongoDB driver for mRuby'

  spec.cc.flags << `pkg-config --cflags libmongoc-#{MONGO_LIB_VERSION}`.delete("\n\r").split(" ")
  mongo_libs     = `pkg-config --libs libmongoc-#{MONGO_LIB_VERSION}`.delete("\n\r").split(" ")

  flags          = mongo_libs.reject { |e| e =~ /^-[lL]/ }
  libpaths       = mongo_libs.select {|e| e =~ /^-L/ }.map {|e| e[2..-1].sub(/^\"(.*)\"$/) { $1 }}
  libraries      = mongo_libs.select {|e| e =~ /^-l/ }.map {|e| e[2..-1]}

  spec.linker.flags         << flags
  spec.linker.library_paths << libpaths
  spec.linker.libraries     << libraries
end
