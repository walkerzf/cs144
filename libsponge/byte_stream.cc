#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`



using namespace std;

ByteStream::ByteStream(const size_t capacity):
size(capacity),buffer(capacity),bufferread(0),bufferwrite(0),flag(false) {}
    


size_t ByteStream::write(const string &data) {
    int index = 0;
    for (; index < static_cast<int >(data.length()) && bufferwrite != bufferread+size; index++) {
        buffer[bufferwrite%size] = data[index];
        bufferwrite++;
    }
    return index;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    string ret;
    size_t i = 0;
    int p = bufferread;
    for (; i < len && p != bufferwrite; i++) {
        ret += buffer[p%size];
        p++;
    }
    return ret;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    size_t i = 0;
    for (; i < len && bufferread != bufferwrite; i++) {
        bufferread++;

    }
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    string ret;
    size_t i = 0;
    for (; i < len && bufferread != bufferwrite; i++) {
        ret+=buffer[bufferread%size];
        bufferread++;
      
    }
    return ret;
}

void ByteStream::end_input() {
    flag = true;
}

bool ByteStream::input_ended() const { 
    return flag;
 }

size_t ByteStream::buffer_size() const { 
    return static_cast<size_t >(bufferwrite-bufferread);
 }

bool ByteStream::buffer_empty() const { 
    if(bufferwrite==bufferread) return true;
    else return false;
}

bool ByteStream::eof() const { 
    if(bufferread==bufferwrite&&flag) return true;
    else return false;
}

size_t ByteStream::bytes_written() const { 
    return static_cast<size_t >(bufferwrite);
 }

size_t ByteStream::bytes_read() const {
    return static_cast<size_t >(bufferread);
 }

size_t ByteStream::remaining_capacity() const { 
    return static_cast<size_t >(size - (bufferwrite-bufferread));
 }
