set terminal png size 900,800 enhanced font "Helvetica,20"
set nokey
set output "tests/Generation/two_ellipses/dbscan.png"
set title "Clusters, DBSCAN (k=10, delta = 0.350000)"
set size ratio -1
plot 'tests/Algorithm/dbscan/data_dbscan.plt' index 0 w p title "0",\
'tests/Algorithm/dbscan/data_dbscan.plt' index 1 w p title "1",\
'tests/Algorithm/dbscan/data_dbscan.plt' index 2 w p title "2",\
'tests/Algorithm/dbscan/data_dbscan.plt' index 3 w p title "3"
