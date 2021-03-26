set terminal png size 900,800 enhanced font "Helvetica,20"
set nokey
set output "tests/Generation/five_ellipses/ker_kmeans.png"
set title "Clouds"
set size ratio -1
plot 'tests/Algorithm/ker_kmeans/data_ker_kmeans.plt' index 0 w p title "0",\
'tests/Algorithm/ker_kmeans/data_ker_kmeans.plt' index 1 w p title "1",\
'tests/Algorithm/ker_kmeans/data_ker_kmeans.plt' index 2 w p title "2",\
'tests/Algorithm/ker_kmeans/data_ker_kmeans.plt' index 3 w p title "3",\
'tests/Algorithm/ker_kmeans/data_ker_kmeans.plt' index 4 w p title "4"
