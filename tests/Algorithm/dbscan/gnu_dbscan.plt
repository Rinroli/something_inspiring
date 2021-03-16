set title "Clouds"
set size ratio -1
plot 'tests/Algorithm/dbscan/data_dbscan.plt' index 0 w p title "0",\
'tests/Algorithm/dbscan/data_dbscan.plt' index 1 w p title "1"
pause -1

