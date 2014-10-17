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


#ifdef _WIN32 /* We need the following two to set stdin/stdout to binary */
#include <io.h>
#include <fcntl.h>
#endif

#if defined(__MACOS__) && defined(__MWERKS__)
#include <console.h>      /* CodeWarrior's Mac "command-line" support */
#endif

#define BUF_SIZ 4096

ogg_int16_t convbuffer[BUF_SIZ]; /* take 8k out of the data segment, not the stack */
int convsize=BUF_SIZ;

extern void _VDBG_dump(void);

int main(){
	ogg_sync_state   oy; /* sync and verify incoming physical bitstream */

	ogg_stream_state os_out; // outgoing bitstream
	ogg_stream_state os; /* take physical pages, weld into a logical
				stream of packets */
	ogg_page         og; /* one Ogg bitstream page. Vorbis packets are inside */
	ogg_page         og_out;

	ogg_packet       op; /* one raw packet of data for decode */
	ogg_packet       op_out;

	vorbis_info      vi; /* struct that stores all the static vorbis bitstream
				settings */

	vorbis_comment   vc; /* struct that stores all the bitstream user comments */

	vorbis_block     vb; /* local working space for packet->PCM decode */
	vorbis_dsp_state vd; /* central working state for the packet->PCM decoder */

	char *buffer;
	int  bytes;
	int stream_count; // keep track of how many chained streams

	ogg_int64_t saved_granule = 0;

	// encode setup
	srand(time(NULL));
	ogg_stream_init(&os_out,rand());



	/********** Decode setup ************/

	ogg_sync_init(&oy); /* Now we can read pages */
  
	while(1){ /* we repeat if the bitstream is chained */
		int eos=0;
		int i;
		int valid_info  = 0;

		stream_count++; 

		/* grab some data at the head of the stream. We want the first page
		   (which is guaranteed to be small and only contain the Vorbis
		   stream initial header) We need the first page to get the stream
		   serialno. */

		/* submit a 4k block to libvorbis' Ogg layer */
		buffer=ogg_sync_buffer(&oy,BUF_SIZ);
		bytes=fread(buffer,1,BUF_SIZ,stdin);
		ogg_sync_wrote(&oy,bytes);
    
		/* Get the first page. */
		if(ogg_sync_pageout(&oy,&og)!=1){
			/* have we simply run out of data?  If so, we're done. */
			if(bytes<BUF_SIZ)break;
      
			/* error case.  Must not be Vorbis data */
			fprintf(stderr,"Input does not appear to be an Ogg bitstream.\n");
			eos=1;
			goto error;
		}

		/* Get the serial number and set up the rest of decode. */
		/* serialno first; use it to set up a logical stream */
		ogg_stream_init(&os,ogg_page_serialno(&og));
    
		/* extract the initial header from the first page and verify that the
		   Ogg bitstream is in fact Vorbis data */
    
		/* I handle the initial header first instead of just having the code
		   read all three Vorbis headers at once because reading the initial
		   header is an easy way to identify a Vorbis bitstream and it's
		   useful to see that functionality seperated out. */
    
		vorbis_info_init(&vi);
		vorbis_comment_init(&vc);

		// this is all just the first packet!
		{
			if(ogg_stream_pagein(&os,&og)<0){ 
				/* error; stream version mismatch perhaps */
				fprintf(stderr,"Error reading first page of Ogg bitstream data.\n");
				eos=1;
				goto error;

 			}
    
			if(ogg_stream_packetout(&os,&op)!=1){ 
				/* no page? must not be vorbis */
				fprintf(stderr,"Error reading initial header packet.\n");
				eos=1;
				goto error;
			}
    
			if(vorbis_synthesis_headerin(&vi,&vc,&op)<0){ 
				/* error case; not a vorbis header */
				fprintf(stderr,"This Ogg bitstream does not contain Vorbis "
					"audio data.\n");
				eos=1;
				goto error;
			}
			valid_info = 1;
		}
		/* At this point, we're sure we're Vorbis. We've set up the logical
		   (Ogg) bitstream decoder. Get the comment and codebook headers and
		   set up the Vorbis decoder */
    
		if(stream_count == 1){
			memcpy(&op_out, &op, sizeof(op));
			ogg_stream_packetin(&os_out,&op_out);
		}

		/* The next two packets in order are the comment and codebook headers.
		   They're likely large and may span multiple pages. Thus we read
		   and submit data until we get our two packets, watching that no
		   pages are missing. If a page is missing, error out; losing a
		   header page is the only place where missing data is fatal. */
    
		i=0;
		while(i<2){
			while(i<2){
				int result=ogg_sync_pageout(&oy,&og);
				if(result==0)break; /* Need more data */
				/* Don't complain about missing or corrupt data yet. We'll
				   catch it at the packet output phase */
				if(result==1){
					ogg_stream_pagein(&os,&og); /* we can ignore any errors here
								       as they'll also become apparent
								       at packetout */
					while(i<2){
						result=ogg_stream_packetout(&os,&op);
						if(result==0)break;
						if(result<0){
							/* Uh oh; data at some point was corrupted or missing!
							   We can't tolerate that in a header.  Die. */
							fprintf(stderr,"Corrupt secondary header.  Exiting.\n");
							eos=1;
							goto error;

						}
						result=vorbis_synthesis_headerin(&vi,&vc,&op);
						if(result<0){
							fprintf(stderr,"Corrupt secondary header.  Exiting.\n");
							eos=1;
							goto error;

						}
						if(stream_count == 1){
							memcpy(&op_out, &op, sizeof(op));
							ogg_stream_packetin(&os_out,&op_out);
						}
						i++;
					}
				}
			}
			/* no harm in not checking before adding more */
			buffer=ogg_sync_buffer(&oy,BUF_SIZ);
			bytes=fread(buffer,1,BUF_SIZ,stdin);
			if(bytes==0 && i<2){
				fprintf(stderr,"End of file before finding all Vorbis headers!\n");
				eos=1;
			}
			ogg_sync_wrote(&oy,bytes);
		}

		while(!eos){ // really, a while true, but with a safety hatch
			int result=ogg_stream_flush(&os_out, &og_out);
			if(result==0)break;
			fwrite(og_out.header, 1, og_out.header_len, stdout);
			fwrite(og_out.body, 1, og_out.body_len, stdout);
		

		}

		/* Throw the comments plus a few lines about the bitstream we're
		   decoding */
		{
			fprintf(stderr, "\nStream #%d, serialno %ld\n", stream_count, os.serialno);
			fprintf(stderr,"Bitstream is %d channel, %ldHz\n",vi.channels,vi.rate);
			fprintf(stderr,"Encoded by: %s\n",vc.vendor);

			char **ptr=vc.user_comments;
			while(ptr && *ptr){
				fprintf(stderr,"\t%s\n",*ptr);
				++ptr;
			}
		}
		
      


		/// all headers done

		if(vorbis_synthesis_init(&vd,&vi)==0){ /* central decode state */
			//local state for most of the decode
			vorbis_block_init(&vd,&vb);         
		}


		/* The rest is just a straight decode loop until end of stream */
		while(!eos){
			while(!eos){
				//fprintf(stderr, "\tpageout\n");
				int result=ogg_sync_pageout(&oy,&og);
				if(result==0)break; /* need more data */
				if(result<0){ /* missing or corrupt data at this page position */
					fprintf(stderr,"Corrupt or missing data in bitstream; "
						"continuing...\n");
					// TODO: continue; ??
				}else{
					// hack around bug in libogg1.3 debian
					if((os.body_returned - os.body_fill) > 0){
						fprintf(stderr, "Invalid body returned %ld > than body fill %ld at packetno %ld\n",
							os.body_returned,
							os.body_fill,
							os.packetno);
						goto error;
					}

					ogg_stream_pagein(&os,&og); /* can safely ignore errors at
								       this point */
					while(1){
						result=ogg_stream_packetout(&os,&op);
						//fprintf(stderr, "\t\t\tquux!\n");
              
						if(result==0)break; /* need more data */
						if(result<0){ /* missing or corrupt data at this page position */
							/* no reason to complain; already complained above */
						}else{
							/* we have a packet.  copy it */

							/* test for success! 
							   use the trackonly because we don't want
							   to decode the whole thing, just make sure
							   it's valid audio
							*/
							if(vorbis_synthesis_trackonly(&vb,&op)==0){ 
								saved_granule += vorbis_packet_blocksize(&vi, &op);
								memcpy(op_out.packet, op.packet, op.bytes);
								op_out.bytes=op.bytes;
								op_out.e_o_s=eos;
								op_out.b_o_s = 0;
								op_out.granulepos = saved_granule;  
								/*
								  fprintf(stderr, "in packet granule: %ld, saved granule: %ld inpacket granule: %ld\n", 
								  saved_granule,
								  op.granulepos);
								*/
								// now attempt to get it to emerge from the other side!
								//fprintf(stderr, "\t\t\t\tbleh!\n");

								ogg_stream_packetin(&os_out, &op_out);
								
								// gets the new granulepos

								while(!eos){
									int result=ogg_stream_pageout(&os_out,&og_out);
									if(result==0)break;
									fwrite(og_out.header,1,og_out.header_len,stdout);
									fwrite(og_out.body,1,og_out.body_len,stdout);
									//fprintf(stderr, "writing audio data\n");
									/* this could be set above, but for illustrative purposes, I do
									   it here (to show that vorbis does know where the stream ends) */
								}
							}


						}
					}
					if(ogg_page_eos(&og))eos=1;
				}
			}
			if(!eos){
				buffer=ogg_sync_buffer(&oy,BUF_SIZ);
				bytes=fread(buffer,1,BUF_SIZ,stdin);
				ogg_sync_wrote(&oy,bytes);
				if(bytes==0)eos=1;
			}
      
			/* ogg_page and ogg_packet structs always point to storage in
			   libvorbis.  They're never freed or manipulated directly */
      
		}

		/* clean up this logical bitstream; before exit we see if we're
		   followed by another [chained] */
	error:
		ogg_stream_reset(&os);
		vorbis_comment_clear(&vc);
		if(!valid_info){
			vorbis_info_clear(&vi);  /* must be called last */
		}
	}

	{
		op_out.e_o_s = 1;
		/// TODO: to avoid clicks, maybe zero out the packet data of the last packet?
		os_out.e_o_s = 1;
		int result=ogg_stream_pageout(&os_out,&og_out);
		if(result!= 0){
			fwrite(og_out.header,1,og_out.header_len,stdout);
			fwrite(og_out.body,1,og_out.body_len,stdout);
		}
	}
	ogg_stream_clear(&os_out);

/* OK, clean up the framer */
	ogg_sync_clear(&oy);
  
	fprintf(stderr,"Done.\n");
	return(0);
}
