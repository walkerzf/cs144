Lab 3 Writeup
=============

My name: [walkerzf]

My SUNet ID: [0]

This lab took me about [8] hours to do. I [did not] attend the lab session.

I worked with or talked about this assignment with: [https://github.com/YangDong2002]

Program Structure and Design of the TCPSender:
    
* WrappingInt32 _isn; //! our initial sequence number, the number for our SYN.

* std::queue<TCPSegment> _segments_out{}; //! outbound queue of segments that the TCPSender wants sent

* unsigned int _initial_retransmission_timeout; //! retransmission timer for the connection

*  ByteStream _stream;  //! outgoing stream of bytes that have not yet been sent

* uint64_t _next_seqno{0}; //! the (absolute) sequence number for the next byte to be sent

//below this,these are the ds we designed for the tcp sender 
//rto and timer
* unsigned int rto;        
* unsigned int cur_timer;

//internal keep the sent segments
* queue<TCPSegment> outstanding_segments{}; 

//ack seqo (absolute seqo)  ,record the ack_seqo from the receiver 
* uint64_t ack_seqo{0};

//fail times 
* unsigned int fails{0};

//right limit first unacceptable seqo number , initialize the window size == 1
* uint64_t unacceptable_seqo{1};

//flag for fin_sent
* bool fin_sent{0};


Implementation Challenges:
[]

Remaining Bugs:
[]

- Optional: I had unexpected difficulty with: [describe]

- Optional: I think you could make this lab better by: [describe]

- Optional: I was surprised by: [describe]

- Optional: I'm not sure about: [describe]
