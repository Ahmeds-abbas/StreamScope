#!/usr/bin/env bash

set -euo pipefail

INPUT="assets/source/input.mp4"
OUTPUT_DIR="assets/generated"

rm -rf "$OUTPUT_DIR"
mkdir -p "$OUTPUT_DIR"/{360p,720p,1080p}

ffmpeg -y -i "$INPUT" \
  -filter_complex \
  "[0:v]split=3[v360][v720][v1080]; \
   [v360]scale=-2:360[v360out]; \
   [v720]scale=-2:720[v720out]; \
   [v1080]scale=-2:1080[v1080out]" \
  \
  -map "[v360out]" -map 0:a \
  -c:v:0 libx264 -b:v:0 800k -maxrate:v:0 856k -bufsize:v:0 1200k \
  -c:a:0 aac -b:a:0 96k \
  \
  -map "[v720out]" -map 0:a \
  -c:v:1 libx264 -b:v:1 2800k -maxrate:v:1 2996k -bufsize:v:1 4200k \
  -c:a:1 aac -b:a:1 128k \
  \
  -map "[v1080out]" -map 0:a \
  -c:v:2 libx264 -b:v:2 5000k -maxrate:v:2 5350k -bufsize:v:2 7500k \
  -c:a:2 aac -b:a:2 128k \
  \
  -preset veryfast \
  -g 120 \
  -keyint_min 120 \
  -sc_threshold 0 \
  \
  -f hls \
  -hls_time 4 \
  -hls_playlist_type vod \
  -hls_flags independent_segments \
  -master_pl_name master.m3u8 \
  -var_stream_map "v:0,a:0,name:360p v:1,a:1,name:720p v:2,a:2,name:1080p" \
  -hls_segment_filename "$OUTPUT_DIR/%v/segment_%03d.ts" \
  "$OUTPUT_DIR/%v/playlist.m3u8"

echo "HLS assets generated in $OUTPUT_DIR"