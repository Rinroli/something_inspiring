set terminal gif animate loop -1 optimize delay 15 background "#ffeedf" \
    font "Times-Roman,10" size 1920,1080
set output "tests/Algorithm/delaunay/delaunay_algorithm.gif"
set size ratio -1
set nokey
set title "Delaunay triangulation"
do for [i=1:363] {
	plot 		'tests/Algorithm/delaunay/delaunay/edges_'.i.'.plt' with lines lw 2 lc rgb "black" title "triangles",\
		'tests/Algorithm/delaunay/delaunay/circles_'.i.'.plt' with lines lw 3 lc rgb "red" title "last triangle",\
		'tests/Algorithm/delaunay/delaunay/points_'.i.'.plt' index 0 w p lt 5 lc rgb "blue" title "points",\
		'tests/Algorithm/delaunay/delaunay/points_'.i.'.plt' index 1 w p lt 7 lc rgb "red" title "current"
}
