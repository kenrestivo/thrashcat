# Thrashcat

A tool for robustly concatenating Ogg Vorbis streams and files into a single Ogg Vorbis stream.

## Why??

Because I'm using Liquidsoap to dump incoming DJ streams in Ogg format, using output.file, with reopen_on_metadata, and with %Y-%m-%d filenames. Combining all these means Trouble.

Liquidsoap leaves useless ogg files lying around with just headers and metadata "TITLE=Unknown. If the DJ's network connection goes out and they disconnect, liquidsoap creates another file, with a new name due to the timestamping. To get a contiguous show with the DJ's stream, one must concatenate these. 

## Why not?

There are other tools for contatenating ogg-vorbis files. In my experience, they don't work for the above use case.

- Raw concatenating with cat or dd doesn't work because Airtime and/or Liquidsoap does not deal well with chained streams at playback time. So one must use a special vorbis-specific tool to concatenate them. 
- oggCat sort of works but chokes on files with streams that have a header and no data, or otherwise corrupt data. It simply skips the rest of the file (the part which probably has stuff you wanted to save in the first place, since the corruption created by liquidsoap is usually at the first stream). Also there was an off-by-1 bug in oggCat which  caused it to skip the first file given on the command line; I tried to fix it but found the code difficult to follow.
- sox appears to either decode and recode the whole stream (absurd on a 6-hour DJ set), or in any case does the processing as if it were, which uses 99% CPU on a server VPS, which is not ideal.
- There are hacks involving mplayer -ao pcm:file... and oggenc, but again since it involves decoding/recoding there are CPU usage problems on small VPSs.
- ffmpeg blows up on chained streams, with stuff like
```
[ogg @ 0x1d265e0] Application provided invalid, non monotonically increasing dts to muxer in stream 0: 31181568 >= 0
av_interleaved_write_frame(): Invalid argument
```

## Requirements

- libvorbis-dev
- libogg-dev
- linux gcc

## Build

	sudo apt-get install libvorbis-dev libogg-dev make gcc
	make

## Install

	make install


## Usage

	thrashcat < /bunch/of/possibly-broken/files/*.ogg > nice-clean-file.ogg


### It willl deal gracefully with
- Empty files
- Files with bad headers
- Files with headers and no data


## Limitations

- Your metadata must be in the FIRST stream you present to thrashcat. In the future maybe instead of taking all the metadata in-band via stdin it could take files as args, mmap them, search through for metadata, pick the best one, stuff that in at the beginning of the stream, then go concatenate the files and streams. I don't feel ike dealing with that right now. Maybe later.

- I made no attempt to support platforms other than Linux, and Debian and derivatives specifically (i.e. Ubuntu)

## The road not taken

I tried writing something from scratch in Clojure using Gloss, but got stuck trying to figure out how to shoehorn things like the Ogg page CRC checking into Gloss's abstractions.

I also tried using Clojure and the Jorbis library, but performance was very slow. This doesn't need to be so heavyweight; a simple C utility should suffice.

## License

Modified 2014 by ken restivo <ken@restivo.org> , shamelessly stolen from decode_example.c and encode_example.c from vorbis source:

```c
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
```

