#include "tcp_receiver.hh"

// Dummy implementation of a TCP receiver

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

// template <typename... Targs>
// void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

void TCPReceiver::segment_received(const TCPSegment &seg) {
    // set the initial seqo if necessary
    TCPHeader header = seg.header();
    Buffer payload = seg.payload();
    if (header.syn && !setisn) {
        _isn = header.seqno;
        setisn = true;
    }
    if (header.fin && !setfin) {
        _fin = header.seqno + static_cast<uint32_t>(payload.size());
        setfin = true;
    }
    // push any data or end-of-stream marker ,to the stream reassembler
    if (setisn) {
        // we need to convert the wrapint32 => unwrap64
        // substring => payload
        // the size_t index => uint64 =  unwrap(WrappingInt32 n, WrappingInt32 isn, uint64_t checkpoint) - 1
        //  checkpoint =>  the index of the last reassembled byte .
       // _reassembler.
       
        // eof => check  _ fin and seqo
      
      
      //  bool eof = false;
    }
}

optional<WrappingInt32> TCPReceiver::ackno() const { return {}; }

size_t TCPReceiver::window_size() const { return {}; }
