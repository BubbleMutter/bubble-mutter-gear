# compile command
`gcc main.c jpeg_simp.c -ljpeg`

# working flow
```txt
jpeg-file(src.jpg)      -- decompress(read_JPEG_file2mem();)    -->
mem (g_ImageBuffer)     -- encompress(encompress_JPEG_m2m();)   -->
mem (g_ImageBufferCprs) -- decompress(decompress_JPEG_m2m();)   -->
mem (g_ImageBufferNew)  -- encompress(write_JPEG_mem2file();)   -->
jpeg-file(new.jpg)
```