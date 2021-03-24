set terminal png size 900,800 enhanced font "Helvetica,20"
set nokey
set output "tests/Generation/five_ellipses/ker_kmeans.png"
set title "Clusters, KerKMeans algorithm (0)"
set size ratio -1
plot 'tests/Algorithm/ker_kmeans/data_ker_kmeans.plt' index 0 w p title "0",\
'tests/Algorithm/ker_kmeans/data_ker_kmeans.plt' index 1 w p title "1",\
'tests/Algorithm/ker_kmeans/data_ker_kmeans.plt' index 2 w p title "2",\
'tests/Algorithm/ker_kmeans/data_ker_kmeans.plt' index 3 w p title "3",\
'tests/Algorithm/ker_kmeans/data_ker_kmeans.plt' index 4 w p title "4",\
'tests/Algorithm/ker_kmeans/data_ker_kmeans.plt' index 5 w p title "5",\
'tests/Algorithm/ker_kmeans/data_ker_kmeans.plt' index 6 w p title "6",\
'tests/Algorithm/ker_kmeans/data_ker_kmeans.plt' index 7 w p title "7",\
'tests/Algorithm/ker_kmeans/data_ker_kmeans.plt' index 8 w p title "8",\
'tests/Algorithm/ker_kmeans/data_ker_kmeans.plt' index 9 w p title "9",\
'tests/Algorithm/ker_kmeans/data_ker_kmeans.plt' index 10 w p title "10",\
'tests/Algorithm/ker_kmeans/data_ker_kmeans.plt' index 11 w p title "11",\
'tests/Algorithm/ker_kmeans/data_ker_kmeans.plt' index 12 w p title "12",\
'tests/Algorithm/ker_kmeans/data_ker_kmeans.plt' index 13 w p title "13",\
'tests/Algorithm/ker_kmeans/data_ker_kmeans.plt' index 14 w p title "14",\
'tests/Algorithm/ker_kmeans/data_ker_kmeans.plt' index 15 w p title "15",\
'tests/Algorithm/ker_kmeans/data_ker_kmeans.plt' index 16 w p title "16",\
'tests/Algorithm/ker_kmeans/data_ker_kmeans.plt' index 17 w p title "17",\
'tests/Algorithm/ker_kmeans/data_ker_kmeans.plt' index 18 w p title "18",\
'tests/Algorithm/ker_kmeans/data_ker_kmeans.plt' index 19 w p title "19"
