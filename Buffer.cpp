#include "Buffer.h"

Buffer::Buffer() {

}
	
Buffer::~Buffer() {

}

	
void Buffer::append(const char *data, size_t len) {
	buffer.append(data, len);
}
	
size_t Buffer::size() {
	return buffer.size();
}
	
std::string& Buffer::data() {
	return buffer;
}
	
void Buffer::clear() {
	buffer.clear();
}
	
void Buffer::erase(int start, size_t n) {
	buffer.erase(start, n);
}

