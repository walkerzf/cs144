Lab 2 Writeup
=============

My name: [walkerzf]

My SUNet ID: [0]

This lab took me about [5] hours to do. I [did not] attend the lab session.


Program Structure and Design of the TCPReceiver and wrap/unwrap routines:
* For wrap, we need to convert absolute to seqo . We need  to mod absolute seqo ,and add isn .because the unsigned ,we can directly add
* For unwrap , we need to convert seqo to absolute seqo.
    * we need to know the diff about the isn and n  => res
    * convert the checkpoint (uint64) to uint32 using mod => m
    * cal the diff about the checkpoint and former diff => diff
    * consider differnet conditions 
        * in 0-2^32-1 the postion about the res and m 
        * the relation about the 2^31 and the diff

* Design TcpReceiver
* we need two flag to indicates we meet syn or fin flag or not and two uinit32 or WrapInt32 to stores the seqo : SYN and FIN
* when we meet the syn ,we need to plus one to the seqo for the first byte of the payload.
* When we use function putsubstring or unwrap , we need to care about the stream index and absolute seqo 
* When the last unreassemble seqo number is at the FIN ,a we need plus one for the return answer!

Implementation Challenges:
* operation on  unsigned number
* the difference of different indexes

Remaining Bugs:
* Maybe the function for return acko have bug , because the FIN have different absolute seqo . I only use the seqo to judge.

- Optional: I had unexpected difficulty with: [describe]

- Optional: I think you could make this lab better by: [describe]

- Optional: I was surprised by: [describe]

- Optional: I'm not sure about: [describe]
