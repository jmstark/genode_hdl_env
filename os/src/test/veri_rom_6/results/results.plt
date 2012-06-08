
set style line 1 linecolor rgb '#3060e0' linetype 1 pointtype 9  pointsize 1.5 linewidth 4 # blue 1
set style line 2 linecolor rgb '#e06030' linetype 2 pointtype 11 pointsize 1.5 linewidth 4 # red 2
set style line 3 linecolor rgb '#a0a0a0' linetype 1                            linewidth 3 # grey 3
set style line 4 linecolor rgb '#ffffff' linetype 1 pointtype 9  pointsize 2   linewidth 6 # white 4
set style line 5 linecolor rgb '#ffffff' linetype 1 pointtype 11 pointsize 2   linewidth 6 # white 5

unset title
set yrange [0:290]
set xrange [0:26]
set xtics 1
set xlabel 'Anzahl der Treiber'
set ylabel 'Latenz in Millisekunden'
set grid ytics ls 3
set border linewidth 2

r(x)= c*x**2 + b*x + a
w(x)= d*x**2 + e*x + f
fit r(x) 'avg_read_lat.txt'  via a, b, c
fit w(x) 'avg_write_lat.txt' via d, e, f

set terminal pngcairo size 1024,600 notransparent dashed
set output "results.png"
plot \
	'avg_read_lat.txt'  with impulses ls 3 title '', \
	'avg_write_lat.txt' with impulses ls 3 title '', \
	r(x) ls 4 title '', \
	w(x) ls 4 title '', \
	r(x) ls 1 title '', \
	w(x) ls 2 title '', \
	'avg_read_lat.txt'  with points ls 4 title '', \
	'avg_write_lat.txt' with points ls 5 title '', \
	'avg_read_lat.txt'  with points ls 1 title 'Lesezugriffe', \
	'avg_write_lat.txt' with points ls 2 title 'Schreibzugriffe'

