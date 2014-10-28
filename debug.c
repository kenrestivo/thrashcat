#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <vorbis/codec.h>


// debug functions TODO: move these

void debug_block(vorbis_block * vb)
{
fprintf(stderr, "block: pcmend %d, mode %d, %s granulepos %ld, sequence %ld\n",
		vb->pcmend,
		vb->mode,
		vb->eofflag ? "EOF" : "",
		vb->granulepos,
		vb->sequence);
}


void debug_dsp(vorbis_dsp_state * vd)
{
	fprintf(stderr,"dsp: storage %d, current %d, returned %d, pree %d %s granule %ld sequence %ld\n",
		vd->pcm_storage,
		vd->pcm_current,
		vd->pcm_returned,
		vd->preextrapolate,
		vd->eofflag ? "EOF" : "",
		vd->granulepos,
		vd->sequence);
}

void debug_stream(ogg_stream_state * s)
{
	fprintf(stderr, "stream: storage %ld, fill %ld, returned %ld, lace-store %ld, lace-fill %ld, lace-pkg %ld, lace-ret %ld , header-fill %d %s %s serial %ld (%0lxd) pageno %ld, packetno %ld, granulepos %ld\n",
		s->body_storage,
		s->body_fill,
		s->body_returned,
		s->lacing_storage,
		s->lacing_fill,
		s->lacing_packet,
		s->lacing_returned,
		s->header_fill,
		s->e_o_s ? "EOS" : "",
		s->b_o_s ? "BOS" : "",
		s->serialno,
		s->serialno,
		s->pageno,
		s->packetno,
		s->granulepos);
		

}

void debug_sync(ogg_sync_state * oy)
{
	fprintf(stderr, "sync: storage %d fill %d returned %d unsynced %d headerbytes %d bodybytes %d\n",
		oy->storage,
		oy->fill,
		oy->returned,
		oy->unsynced,
		oy->headerbytes,
		oy->bodybytes);
}

void debug_page(ogg_page * og, ogg_int64_t pp, int tsc)
{
	fprintf(stderr, "page: headerlen %ld, bodylen %ld, %s %s %s granulepos %ld (%ld), tgranule %ld, serialno %d (%0x), pageno %ld stream #%d\n",
		og->header_len, 
		og->body_len,
		ogg_page_continued(og) ? "CONT" : "",
		ogg_page_bos(og) ? "BOS" : "",
		ogg_page_eos(og) ? "EOS" : "",
		ogg_page_granulepos(og),
		ogg_page_granulepos(og) - pp,
		pp,
		ogg_page_serialno(og),
		ogg_page_serialno(og),
		ogg_page_pageno(og),
		tsc);
}





/* Throw the comments plus a few lines about the bitstream we're
   decoding */
void debug_comments(vorbis_info * vi, vorbis_comment * vc)
{
	fprintf(stderr,"Bitstream is %d channel, %ldHz\n",vi->channels,vi->rate);
	fprintf(stderr,"Encoded by: %s\n",vc->vendor);

	char **ptr=vc->user_comments;
	while(ptr && *ptr){
		fprintf(stderr,"\t%s\n",*ptr);
		++ptr;
	}
}


void debug_packet(ogg_packet * op)
{
	fprintf(stderr, "packet: bytes %ld, %s %s granulepos %ld, packetno %ld\n",
		op->bytes,
		op->b_o_s ? "BOS" : "",
		op->e_o_s ? "EOS" : "",
		op->granulepos,
		op->packetno);
	
}
