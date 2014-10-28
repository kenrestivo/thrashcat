#include <vorbis/codec.h>



// debug functions TODO: move these

void debug_block(vorbis_block * vb);



void debug_dsp(vorbis_dsp_state * vd);


void debug_stream(ogg_stream_state * s);


void debug_sync(ogg_sync_state * oy);


void debug_page(ogg_page * og, ogg_int64_t pp, int tsc);






/* Throw the comments plus a few lines about the bitstream we're
   decoding */
void debug_comments(vorbis_info * vi, vorbis_comment * vc);



void debug_packet(ogg_packet * op);



