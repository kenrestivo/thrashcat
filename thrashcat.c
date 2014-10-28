/********************************************************************
 *                                                                  *
 * THIS FILE IS PART OF THE OggVorbis SOFTWARE CODEC SOURCE CODE.   *
 * USE, DISTRIBUTION AND REPRODUCTION OF THIS LIBRARY SOURCE IS     *
 * GOVERNED BY A BSD-STYLE SOURCE LICENSE INCLUDED WITH THIS SOURCE *
 * IN 'COPYING'. PLEASE READ THESE TERMS BEFORE DISTRIBUTING.       *
 *                                                                  *
 * THE OggVorbis SOURCE CODE IS (C) COPYRIGHT 1994-2009             *
 * by the Xiph.Org Foundation http://www.xiph.org/                  *
 *                                                                  *
 ********************************************************************


Modified (c) 2014 by ken restivo <ken@restivo.org>

 function: concatenate ogg/vorbis files/streams into single stream

********************************************************************/

/* Takes a vorbis bitstream from stdin and writes raw stereo PCM to
   stdout. Decodes simple and chained OggVorbis files from beginning
   to end. Vorbisfile.a is somewhat more complex than the code below.  */

/* Note that this is POSIX, not ANSI code */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <vorbis/codec.h>
#include "debug.h"

#define BUF_SIZ 8192

ogg_int16_t convbuffer[BUF_SIZ]; 
int convsize=BUF_SIZ;



// incoming
ogg_sync_state   oy;
ogg_stream_state os; 
ogg_page         og;
ogg_packet       op;
vorbis_info      vi;
vorbis_comment   vc;
vorbis_block     vb; 
vorbis_dsp_state vd;

// outgoing
ogg_stream_state os_out; 
ogg_page         og_out;
ogg_packet       op_out;

char *buffer;

// status state, maybe move to struct
int  bytes_read;
int track_res;
ogg_int64_t total_granule_pos = 0;
ogg_int64_t prev_page_granule_pos = 0;
ogg_int64_t total_cumulative_granule_pos = 0;
int total_stream_count = 0;
long total_bytes_read = 0;
int total_eof = 0;
int current_stream_count = 0;
ogg_int64_t current_serialno = 0;
typedef enum {UNKNOWN,NEW,IN_PROCESS,COMPLETED,ERROR} header_state_t;
header_state_t header_state = UNKNOWN;

void check_new_stream()
{
	if(ogg_page_bos(&og) 
	   || current_serialno != ogg_page_serialno(&og))
	{
		header_state = NEW;
		total_stream_count++;
		prev_page_granule_pos = 0;
		fprintf(stderr, "\n----- new stream %d serialno %d (%0x) ------- \n", 
			total_stream_count, ogg_page_serialno(&og), ogg_page_serialno(&og));

	}

}


void setup_encoder()
{
	// encode setup
	srand(time(NULL));
	ogg_stream_init(&os_out,rand());

}

void setup_decoder()
{
	ogg_sync_init(&oy);
}


void copy_page()
{

	ogg_iovec_t iov[1];

	iov[0].iov_base = og.body;
	iov[0].iov_len = og.body_len;
	if(ogg_stream_iovecin(&os_out, iov, 1, total_eof, total_granule_pos) != 0){
		fprintf(stderr, "blown iovec\n");
		return;
	}

//	int result = ogg_stream_flush(&os_out, &og_out);
	int result = ogg_stream_pageout(&os_out, &og_out);
	if(result == 0){
		fprintf(stderr, "no pageout\n");
		return;
	}


	fwrite(og_out.header, 1, og_out.header_len, stdout);
	fwrite(og_out.body, 1, og_out.body_len, stdout);


}


int process_header()
{
	int page_res;
	int synth_res;



	if(header_state == NEW){
		ogg_stream_init(&os, ogg_page_serialno(&og));
		vorbis_info_init(&vi);
		vorbis_comment_init(&vc);
		header_state = IN_PROCESS;
	}
	
	if(header_state != IN_PROCESS){
		//no header here, we're in body, get out, let the bdy handle it.
		return(2); // TODO: not sur what to return?
	}


	page_res = ogg_stream_pagein(&os,&og);
	if(page_res < 0 && header_state == IN_PROCESS){
		fprintf(stderr,"Error reading first page of Ogg bitstream data.\n");
		header_state = ERROR;
		return(1);
	}

		

	switch(ogg_stream_packetout(&os,&op)){
	case 0:
		header_state = IN_PROCESS;
		fprintf(stderr, "need moar data\n"); 
		return(0); // need moar data
		break;
	case 1:
		// good packet
		debug_packet(&op);
		//copy_page();
		break;
	case -1:
		fprintf(stderr,"Error reading initial header packet.\n");
		header_state = ERROR;
		debug_packet(&op);
		return(1);
		break;
	}



	synth_res = vorbis_synthesis_headerin(&vi,&vc,&op);
	if(synth_res < 0 ){
		fprintf(stderr,"This Ogg bitstream does not contain Vorbis audio data\n");
		header_state = ERROR;
		return(1);
	}

	if(ogg_page_pageno(&og) == 2){
		header_state =  COMPLETED;
		debug_comments(&vi, &vc);

		// XXX what is this and why?
		convsize=BUF_SIZ/vi.channels;


		if(vorbis_synthesis_init(&vd,&vi)!=0){ 
			header_state = ERROR;
			return(1);
		}

		vorbis_block_init(&vd,&vb);    
	}	


	return(0);
}


void decode_block()
{
	if(vorbis_synthesis_trackonly(&vb,&op)==0){
		vorbis_synthesis_blockin(&vd,&vb);
		debug_block(&vb);
		debug_dsp(&vd);
	} else {
		fprintf(stderr, "error in synthesis\n");
	}
}


void process_audio()
{
	int page_res;
	//int packet_res;
	//int synth_res;


	if(header_state != COMPLETED){
		// pass
		return;
	}

	page_res = ogg_stream_pagein(&os,&og);
	if(page_res < 0){
		fprintf(stderr, "page in error\n");
		return;
	}

	switch(ogg_stream_packetout(&os,&op)){
	case 0:
		header_state = IN_PROCESS;
		fprintf(stderr, "need moar data\n"); 
		return(0); // need moar data
		break;
	case 1:
		// good packet
		debug_packet(&op);
		//copy_page();
		break;
	case -1:
		fprintf(stderr,"Error reading audio packet.\n");
		debug_packet(&op);
		return;
		break;
	}
	

	
}


void process_page()
{
	if(ogg_sync_pageout(&oy,&og)!=1){
		// need moar DATA
		return;
	} 

	check_new_stream();

	// TODO: should i only do this if it's audio packets?
	total_cumulative_granule_pos += ogg_page_granulepos(&og) - prev_page_granule_pos;
	
	debug_page(&og, prev_page_granule_pos, total_stream_count);

	debug_stream(&os);

	process_header();
	
	process_audio();
	
	fprintf(stderr, "\n");

	// End cleanups
	prev_page_granule_pos = ogg_page_granulepos(&og);
	current_serialno = ogg_page_serialno(&og);
}


void process_stdin()
{
	do { 
		buffer = ogg_sync_buffer(&oy,BUF_SIZ);
		bytes_read = fread(buffer, 1, BUF_SIZ, stdin);
		ogg_sync_wrote(&oy, bytes_read);
		total_bytes_read += bytes_read;
		
		//fprintf(stderr, "read %d\n", bytes_read);

		//debug_sync();
		if(bytes_read == 0){
			total_eof = 1;
		}

		process_page();

	} while(bytes_read > 0);

	
	fprintf(stderr, "finished, total read %ld\n", total_bytes_read);

}


int main(){


	setup_encoder();
	setup_decoder();

	process_stdin();
  
	fprintf(stderr,"Done.\n");
	return(0);
}
