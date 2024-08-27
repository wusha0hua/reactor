#pragma once

#include <cstddef>
#include <iostream>
#include <cstring>

class Buffer {
public:
	Buffer();
	~Buffer();
	void append(const char *data, size_t len);
	size_t size();
	std::string& data();
	void clear();
	void erase(int start, size_t n);
private:
	std::string buffer;
};
