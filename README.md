# Thrashcat

A tool for robustly concatenating Ogg Vorbis streams and files into a single Ogg Vorbis stream.

## Why??

Because I'm using Liquidsoap to dump incoming DJ streams in Ogg format.
This is a problem because of the way liquidsoap works and the way the Ogg format is constructed.
 When saving timestamped streams, liquidsoap starts a file, puts valid headers, but no audio data. Some even have 0 bytes, not even a header.  If you set liquidsoap to reopen_on_metadata, it leaves a useless ogg file lying around with just headers and useless metadata "TITLE=Unknown", and doesn't really know how to clean up after itself. Instead it just creates a new file. If the DJ's network connection goes out and they disconnect, liquidsoap creates another file. That file now has a different name and timestamp, and to get a contiguous show with the DJ's stream, one must concatenate these. Concatenating is never simple with Vorbis, because of the header format. Raw concatenating doesn't work because Airtime and/or Liquidsoap does not deal well with chained streams. So one must use a special tool to concatenate them. 

## Why not?

There are other tools. In my experience, they don't work for the above use case.

- oggCat sort of works but chokes horribly on files with streams that have a header and no data. It simply skips the rest of the file (the part which probably has stuff you wanted to save in the first place). Also there was an off-by-1 bug in oggCat which  caused it to skip the first file given on the command line.
- sox appears to either decode and recode the whole stream (absurd on a 6-hour DJ set), or in any case does the processing as if it were, which uses 99% CPU on a server VPS, which is not being a good VPS citizen.
- There are hacks involving mplayer -ao pcm:file... and oggenc, but decoding and recoding offends my sense of efficiency, also burns up CPU on a VPS.

## Requirements

- libvorbis-dev
- libogg-dev
- linux gcc

## Build

make

## Install

sudo make install


## Usage

	thrashcat < /bunch/of/possibly-broken/files/*.ogg > nice-clean-file.ogg


### It willl deal gracefully with
- Empty files
- Files with bad headers
- Files with headers and no data


## Limitations

- Your metadata must be in the FIRST stream you present to thrashcat. In the future maybe instead of taking all the metadata in-band via stdin it could take files as args, mmap them, search through for metadata, pick the best one, stuff that in at the beginning of the stream, then go concatenate the files and streams. I don't feel ike dealing with that right now. Maybe later.

- I made no attempt to support platforms other than Linux, and even stripped out some of the vorbis code that dealt with Mac and W32. Someone could weave those back in if needed.

## The road not taken

I tried writing something from scratch in Clojure using Gloss, but got stuck trying to figure out how to shoehorn things like the Vorbis CRC checking into Gloss's abstractions.

I also tried using Clojure and the Jorbis library, but performance was very slow. This doesn't need to be so heavyweight; a simple C utility should suffice.

## License

Modified 2014 by ken restivo <ken@restivo.org> , shamelessly stolen from decode_example.c and encode_example.c from vorbis source:

********************************************************************
*                                                                  *
* THIS FILE IS PART OF THE OggVorbis SOFTWARE CODEC SOURCE CODE.   *
* USE, DISTRIBUTION AND REPRODUCTION OF THIS LIBRARY SOURCE IS     *
* GOVERNED BY A BSD-STYLE SOURCE LICENSE INCLUDED WITH THIS SOURCE *
* IN 'COPYING'. PLEASE READ THESE TERMS BEFORE DISTRIBUTING.       *
*                                                                  *
* THE OggVorbis SOURCE CODE IS (C) COPYRIGHT 1994-2007             *
* by the Xiph.org Foundation, http://www.xiph.org/                 *
*                                                                  *
********************************************************************


