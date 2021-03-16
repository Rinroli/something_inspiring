set terminal gif animate loop -1 optimize delay 15 background "#ffeedf" \
    font "Times-Roman,10" size 1280,960
set output "tests/Algorithm/forel/forel_algorithm.gif"
set size ratio -1
set nokey
set title "FOREL-algorithm"
do for [i=0:250] {
	plot 		'tests/Algorithm/forel/forel/clusters_'.i.'.plt' index 0 w p title "#0",\
		'tests/Algorithm/forel/forel/clusters_'.i.'.plt' index 1 w p title "#1",\
		'tests/Algorithm/forel/forel/clusters_'.i.'.plt' index 2 w p title "#2",\
		'tests/Algorithm/forel/forel/clusters_'.i.'.plt' index 3 w p title "#3",\
		'tests/Algorithm/forel/forel/clusters_'.i.'.plt' index 4 w p title "#4",\
		'tests/Algorithm/forel/forel/clusters_'.i.'.plt' index 5 w p title "#5",\
		'tests/Algorithm/forel/forel/clusters_'.i.'.plt' index 6 w p title "#6",\
		'tests/Algorithm/forel/forel/clusters_'.i.'.plt' index 7 w p title "#7",\
		'tests/Algorithm/forel/forel/clusters_'.i.'.plt' index 8 w p title "#8",\
		'tests/Algorithm/forel/forel/clusters_'.i.'.plt' index 9 w p title "#9",\
		'tests/Algorithm/forel/forel/clusters_'.i.'.plt' index 10 w p title "#10",\
		'tests/Algorithm/forel/forel/clusters_'.i.'.plt' index 11 w p title "#11",\
		'tests/Algorithm/forel/forel/clusters_'.i.'.plt' index 12 w p title "#12",\
		'tests/Algorithm/forel/forel/clusters_'.i.'.plt' index 13 w p title "#13",\
		'tests/Algorithm/forel/forel/clusters_'.i.'.plt' index 14 w p title "#14",\
		'tests/Algorithm/forel/forel/clusters_'.i.'.plt' index 15 w p title "#15",\
		'tests/Algorithm/forel/forel/clusters_'.i.'.plt' index 16 w p title "#16",\
		'tests/Algorithm/forel/forel/clusters_'.i.'.plt' index 17 w p title "#17",\
		'tests/Algorithm/forel/forel/clusters_'.i.'.plt' index 18 w p title "#18",\
		'tests/Algorithm/forel/forel/clusters_'.i.'.plt' index 19 w p title "#19",\
		'tests/Algorithm/forel/forel/clusters_'.i.'.plt' index 20 w p title "#20",\
		'tests/Algorithm/forel/forel/clusters_'.i.'.plt' index 21 w p title "#21",\
		'tests/Algorithm/forel/forel/clusters_'.i.'.plt' index 22 w p title "#22",\
		'tests/Algorithm/forel/forel/clusters_'.i.'.plt' index 23 w p title "#23",\
		'tests/Algorithm/forel/forel/clusters_'.i.'.plt' index 24 w p title "#24",\
		'tests/Algorithm/forel/forel/clusters_'.i.'.plt' index 25 w p title "#25",\
		'tests/Algorithm/forel/forel/clusters_'.i.'.plt' index 26 w p title "#26",\
		'tests/Algorithm/forel/forel/circles_'.i.'.plt' using 1:2:3with circles lc rgb "red" title "circles"
}