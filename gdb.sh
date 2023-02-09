make run-pxe-debug & disown
gdb \
   -ix "gdb_init_real_mode.txt" \
   -ex "set tdesc filename target.xml"\
   -ex "target remote localhost:1234"\
   -ex "br *0x7eba"\
   -ex "c"
