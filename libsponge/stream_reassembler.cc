#include "stream_reassembler.hh"

#include <iostream>
// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

// template <typename... Targs>
// void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity)
    : _output(capacity)
    , _capacity(capacity)
    , firstunassembled(0)
    , firstunread(0)
    , m(capacity)
    , flag(capacity, false)
    , length(-1)
    , count(0) {}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    firstunread = _output.bytes_read();
    size_t start = index;
    size_t datalen = data.length();
    size_t c = _capacity;
    if (eof)
        length = start + datalen;
    size_t i = 0;
    for (i = start; i < start + datalen && i <  firstunread + c; i++) {
        if (i < firstunassembled || flag[i % c])
            continue;

        flag[i % c] = true;
        m[i % c] = data[i - start];
        count++;
    }

    string s;

    for (i = firstunassembled; i < firstunread + c; i++) {
        if (!flag[i % c])
            break;
        s += m[i % c];
        flag[i % c] = false;
        count--;
    }
    firstunassembled = i;
    if (firstunassembled == length)
        _output.end_input();

    if (!s.empty())
        _output.write(s);
}

size_t StreamReassembler::unassembled_bytes() const { return static_cast<size_t>(count); }

bool StreamReassembler::empty() const {
    // cout << count << endl;
    return count == 0;
}
