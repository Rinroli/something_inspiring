set term gif animate optimize delay 10 background "#ffeedf" \
    font "Times-Roman,10"
set output "tests/Algorithm/ker_kmeans/ker_algorithm.gif"
set size square

set title "KMeans-algorithm with kernels (stable - false, steps 250)
do for [i=0:250] {
	plot 		'tests/Algorithm/ker_kmeans/ker/points_'.i.'.plt' index 0 w p title "#0",\
		'tests/Algorithm/ker_kmeans/ker/points_'.i.'.plt' index 1 w p title "#1",\
		'tests/Algorithm/ker_kmeans/ker/points_'.i.'.plt' index 2 w p title "#2",\
		'tests/Algorithm/ker_kmeans/ker/points_'.i.'.plt' index 3 w p title "#3",\
		'tests/Algorithm/ker_kmeans/ker/points_'.i.'.plt' index 4 w p title "#4",\
		'tests/Algorithm/ker_kmeans/ker/points_'.i.'.plt' index 5 w p title "#5",\
		'tests/Algorithm/ker_kmeans/ker/points_'.i.'.plt' index 6 w p title "#6",\
		'tests/Algorithm/ker_kmeans/ker/points_'.i.'.plt' index 7 w p title "#7",\
		'tests/Algorithm/ker_kmeans/ker/points_'.i.'.plt' index 8 w p title "#8",\
		'tests/Algorithm/ker_kmeans/ker/points_'.i.'.plt' index 9 w p title "#9",\
		'tests/Algorithm/ker_kmeans/ker/points_'.i.'.plt' index 10 w p title "#10",\
		'tests/Algorithm/ker_kmeans/ker/points_'.i.'.plt' index 11 w p title "#11",\
		'tests/Algorithm/ker_kmeans/ker/points_'.i.'.plt' index 12 w p title "#12",\
		'tests/Algorithm/ker_kmeans/ker/points_'.i.'.plt' index 13 w p title "#13",\
		'tests/Algorithm/ker_kmeans/ker/points_'.i.'.plt' index 14 w p title "#14",\
		'tests/Algorithm/ker_kmeans/ker/points_'.i.'.plt' index 15 w p title "#15",\
		'tests/Algorithm/ker_kmeans/ker/points_'.i.'.plt' index 16 w p title "#16",\
		'tests/Algorithm/ker_kmeans/ker/points_'.i.'.plt' index 17 w p title "#17",\
		'tests/Algorithm/ker_kmeans/ker/points_'.i.'.plt' index 18 w p title "#18",\
		'tests/Algorithm/ker_kmeans/ker/points_'.i.'.plt' index 19 w p title "#19",\
		'tests/Algorithm/ker_kmeans/ker/kernel_'.i.'.plt' w p lc rgb "red" title "kernels"
}
