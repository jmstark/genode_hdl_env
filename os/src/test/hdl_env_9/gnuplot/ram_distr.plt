set palette blue, navyBlue, green, pinegreen
set width 6
set output "ram_distr.png"
set terminal png antialias dpi 300 notransparent
set size 3
piechart '--' using $1 label auto "%s"%($2)
970752  Treiber
2166784 VinitTreiber
4063232 VinitEmulator
4325376 Emulator
END
