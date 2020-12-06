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

    WrappingInt32 n(0);

    if (header.syn && !setisn) {
        _isn = WrappingInt32(header.seqno.raw_value());
        setisn = true;
        n = header.seqno + 1u;
    } else {
        n = header.seqno;
    }

    if (header.fin && !setfin) {
        _fin = n + static_cast<uint32_t>(payload.size());
        setfin = true;
        //_reassembler.stream_out().end_input();
        if (setisn) {
            f = unwrap(_fin, _isn, _reassembler.lastassemble());
        }
    }

    // push any data or end-of-stream marker ,to the stream reassembler
    if (setisn) {
        // we need to convert the wrapint32 => unwrap64
        // substring => payload
        // the size_t index => uint64 =  unwrap(WrappingInt32 n, WrappingInt32 isn, uint64_t checkpoint) - 1
        //  checkpoint =>  the index of the last reassembled byte .
        bool eof = false;
        if (header.fin)
            eof = true;

        uint64_t checkpoint = _reassembler.lastassemble();
        uint64_t start = unwrap(n, _isn, checkpoint);
        // change the absolute seqo => stream index
        _reassembler.push_substring(payload.copy(), start - 1ul, eof);
        // eof => check  _ fin and seqo
    }
}

optional<WrappingInt32> TCPReceiver::ackno() const {
    if (!setisn) {
        return {};
    } else {
        uint64_t n = _reassembler.firstunassem();
        // we need to change the stream index = > absolute seqo number plus one
        WrappingInt32 res = wrap(n + 1, _isn);
        //fin takes one postion so next want to revceive is next 
        if (res.raw_value() == _fin.raw_value())
            return res + 1u;
        else
            return res;
    }
}

size_t TCPReceiver::window_size() const { return _capacity - _reassembler.stream_out().buffer_size(); }
