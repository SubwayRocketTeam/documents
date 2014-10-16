require 'eventmachine'

class Packet
	INT = 0, STRING = 1

	def initialize
		self.class.items.each do |item|
			instance_variable_set ("@" + item[0]).to_sym, item[1]

			self.class.class_eval {
				attr_accessor (item[0]).to_sym
			}
		end
	end

	def serialize
		header = Array.new
		ary = Array.new
		fmt = String.new("II")
		size = 0

		instance_variables.each do |v|
			ary.push instance_variable_get v
			fmt += "I"
			size += 4
		end

		puts self.class.id

		header.push size + 8
		header.push self.class.id
		
		(header+ary).pack(fmt)
	end

	def self.id value
		items = Array.new
		define_singleton_method "id" do
			return value
		end
		define_singleton_method "items" do
			return items
		end
	end
	def self.int key, value=0
		self.items.push([key, value])
	end
end

class Foo < Packet
	id 3
	int "a"
end


module TestClient
	def post_init
		puts "connected"

		f = Foo.new 
		f.a = 5
		send_data f.serialize
	end
	def unbind
		puts "disconnected"
	end

	def receive_data data
		#size = data.unpack("I")
		#data.slice!(4..-1)
		size, id, a = data.unpack("III")

		puts "recv size = #{size} / id = #{id} / a = #{a}	"
	end
end

EventMachine.run {
	EventMachine.connect "localhost", 9916, TestClient
}
