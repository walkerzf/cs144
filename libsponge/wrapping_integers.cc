#include "wrapping_integers.hh"

#include <cstdint>
#include <iostream>
#include <sstream>
// Dummy implementation of a 32-bit wrapping integer

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

using namespace std;

//! Transform an "absolute" 64-bit sequence number (zero-indexed) into a WrappingInt32
//! \param n The input absolute 64-bit sequence number
//! \param isn The initial sequence number
WrappingInt32 wrap(uint64_t n, WrappingInt32 isn) {
    n %= (1l << 32);
    isn = (isn + static_cast<uint32_t>(n));
    return isn;
}

//! Transform a WrappingInt32 into an "absolute" 64-bit sequence number (zero-indexed)
//! \param n The relative sequence number
//! \param isn The initial sequence number
//! \param checkpoint A recent absolute 64-bit sequence number
//! \returns the 64-bit sequence number that wraps to `n` and is closest to `checkpoint`
//!
//! \note Each of the two streams of the TCP connection has its own ISN. One stream
//! runs from the local TCPSender to the remote TCPReceiver and has one ISN,
//! and the other stream runs from the remote TCPSender to the local TCPReceiver and
//! has a different ISN.
uint64_t unwrap(WrappingInt32 n, WrappingInt32 isn, uint64_t checkpoint) {
    uint64_t res = static_cast<uint64_t>(n.raw_value() - isn.raw_value());

    if (res >= checkpoint)
        return res;
    uint64_t d31 = (1ul << 31);
    uint64_t d32 = (1ul << 32);
    uint32_t m = static_cast<uint32_t>(checkpoint % d32);
    //add is too time-consumint 
    //we only need to think about the 0-2^32-1 range!!!
    if (m > res) {
        if (m - static_cast<uint32_t>(res) <= static_cast<uint32_t>(d31)) {
            res += (checkpoint - m);
        } else {
            res += (checkpoint - m + d32);
        }
    } else if (m < res) {
        if (m - static_cast<uint32_t>(res) <= static_cast<uint32_t>(d31)) {
            res += (checkpoint - m - d32);
        } else {
            res += (checkpoint - m);
        }
    }else{
        res +=(checkpoint-m);
    }

    return res;
}
