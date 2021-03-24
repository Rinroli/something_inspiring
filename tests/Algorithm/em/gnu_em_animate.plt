set term gif animate optimize delay 10 background "#ffeedf" \
    font "Times-Roman,10" size 1280,960
set output "tests/Algorithm/em/em_algorithm.gif"
set size square
set title "EM-algorithm"
do for [i=0:362] {
	plot 		'tests/Algorithm/em/em/points_'.i.'.plt' index 0 w p title "#0",\
		'tests/Algorithm/em/em/points_'.i.'.plt' index 1 w p title "#1",\
		'tests/Algorithm/em/em/points_'.i.'.plt' index 2 w p title "#2",\
		'tests/Algorithm/em/em/points_'.i.'.plt' index 3 w p title "#3",\
		'tests/Algorithm/em/em/points_'.i.'.plt' index 4 w p title "#4",\
		'tests/Algorithm/em/em/ellipse_'.i.'.plt' using 1:2:3:4:5 with ellipses lc rgb "red" title "ellipses"
}