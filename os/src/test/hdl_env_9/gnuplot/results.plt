
set style line 1 linecolor rgb '#3060e0' linetype 2 pointtype 9  pointsize 1.5 linewidth 4 # blue 1
set style line 2 linecolor rgb '#e06030' linetype 2 pointtype 11 pointsize 1.5 linewidth 4 # red 2
set style line 3 linecolor rgb '#a0a0a0' linetype 1                            linewidth 3 # grey 3
set style line 4 linecolor rgb '#ffffff' linetype 1 pointtype 9  pointsize 2   linewidth 6 # white 4
set style line 5 linecolor rgb '#ffffff' linetype 1 pointtype 11 pointsize 2   linewidth 6 # white 5
set style line 6 linecolor rgb '#000000' linetype 1                            linewidth 1 # black 6

unset title
set yrange [45:75]
set xrange [0:10.9]
set xlabel 'Tasks'
set ylabel 'Latenz in Mikrosekunden'
set grid ytics ls 3
set border linewidth 2
set key invert

r(x)= a + b*log(x) + c*x
fit r(x) 'irq_latency.txt'  via a, b, c

set terminal pngcairo size 1024,600 notransparent dashed
set output "results.png"
plot \
	'irq_latency.txt'  with impulses ls 3 title '', \
	r(x) ls 4 title '', \
	r(x) ls 1 title 'Approximation der Interrupt-Latenz', \
	'irq_latency.txt'  with points ls 4 title '', \
	'irq_latency.txt'  with points ls 1 title 'Interrupt-Latenz'

set key default

