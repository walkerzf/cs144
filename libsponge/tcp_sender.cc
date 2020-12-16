#include "tcp_sender.hh"

#include "tcp_config.hh"

#include <random>

// Dummy implementation of a TCP sender

// For Lab 3, please replace with a real implementation that passes the
// automated checks run by `make check_lab3`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

//! \param[in] capacity the capacity of the outgoing byte stream
//! \param[in] retx_timeout the initial amount of time to wait before retransmitting the oldest outstanding segment
//! \param[in] fixed_isn the Initial Sequence Number to use, if set (otherwise uses a random ISN)
TCPSender::TCPSender(const size_t capacity, const uint16_t retx_timeout, const std::optional<WrappingInt32> fixed_isn)
    : _isn(fixed_isn.value_or(WrappingInt32{random_device()()}))
    , _initial_retransmission_timeout{retx_timeout}
    , rto(retx_timeout)
    , cur_timer(-1)
    , _stream(capacity) {}

uint64_t TCPSender::bytes_in_flight() const { return _next_seqno - ack_seqo; }

void TCPSender::fill_window() {
    // we need to fill the window
    // the window is the first unreassemable seqo - unacceptable seqno
    uint64_t window = unacceptable_seqo - _next_seqno;

    //? why this ? =>we send a one byte make the next_seqo > unacceptable seqo
    if (unacceptable_seqo < _next_seqno)
        return;
    // when the window size ==1 and we already receive the ack for all sent segment, we try the one byte segment for ack
    // really window size == 0 for receiver
    //If the receiver has announced a window size of zero, the fill window method should act like the window size is one.
    if (window == 0 && ack_seqo == _next_seqno)
        window = 1;
    string s;
    TCPSegment seg;
    while (window > 0 && !fin_sent) {
        seg.header().seqno = wrap(_next_seqno, _isn);
        seg.header().syn = _next_seqno == 0;
        seg.header().fin = _stream.eof();
        s = _stream.read(min(TCPConfig::MAX_PAYLOAD_SIZE, window) - seg.header().syn - seg.header().fin);
        seg.payload() = Buffer(move(s));
        // after read the stream.eof , the size of the seg must be checked because the fin flag occupies one bit
        if (_stream.eof() && seg.length_in_sequence_space() < window) {
            seg.header().fin = true;
        }
        //if the seg length ==0  we do not need send a segment ,the read method can not read any data from the outgoing bytestream
        if (seg.length_in_sequence_space() == 0)
            break;
        //if the timer is not on , we start the timer 
        //if(outstanding_segments.empty())
        if (cur_timer==static_cast<unsigned int>(-1))
            cur_timer = rto;
        //push the seg to the two queue
        outstanding_segments.push(seg);
        _segments_out.push(seg);
        //update the window and fin flag and next_seqo
        window -= seg.length_in_sequence_space();
        _next_seqno += seg.length_in_sequence_space();
        fin_sent |= seg.header().fin;
    }
}

//! \param ackno The remote receiver's ackno (acknowledgment number)
//! \param window_size The remote receiver's advertised window size
void TCPSender::ack_received(const WrappingInt32 ackno, const uint16_t window_size) {
    // we  convert the ackno => seqo
    // the new window size
    uint64_t ack_new = unwrap(ackno, _isn, ack_seqo);
    while (!outstanding_segments.empty()) {
        //we check if we need to remove any seg from the internal ds keeping the all sending segments 
        //remove those seqno are all acked by reveiver 
        auto seg = outstanding_segments.front();
        if (unwrap(seg.header().seqno, _isn, ack_seqo) + seg.length_in_sequence_space() <= ack_new) {
            outstanding_segments.pop();
            //set the rto 
            rto = _initial_retransmission_timeout;
            //start a new timer  if have out segments 
            cur_timer = outstanding_segments.empty() ? -1 : rto;
            //reset the fail times 
            fails = 0;
        } else
            break;
    }
    if(outstanding_segments.empty()) cur_timer = -1;
    // new  ack seqo
    ack_seqo = max(ack_seqo, ack_new);
    //new unacceptable seqo 
    if (ack_new + window_size > unacceptable_seqo) {
        unacceptable_seqo = ack_new + window_size;
    }
     fill_window();
}

//! \param[in] ms_since_last_tick the number of milliseconds since the last call to this method
void TCPSender::tick(const size_t ms_since_last_tick) {
    //if the timer expire we need to retransimit
    if (cur_timer <= ms_since_last_tick) {
        //transmit the oldest segment
        TCPSegment seg = outstanding_segments.front();
        _segments_out.push(seg);
        //if the window size is not zero 
        if (unacceptable_seqo > ack_seqo) {
            fails++;
            rto *= 2;
        }
        // take into account that you may double the rto! if the window size is zero we will not double the rto
        cur_timer = rto;
    } else
        cur_timer -= ms_since_last_tick;
}
//directly return the fail times
unsigned int TCPSender::consecutive_retransmissions() const { return fails; }

void TCPSender::send_empty_segment() {
    //we send an empty segment 
    TCPSegment seg;
    seg.header().seqno = wrap(_next_seqno, _isn);
    _segments_out.push(seg);
}
