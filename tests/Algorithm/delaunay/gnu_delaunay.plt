set terminal png size 1920,1080 enhanced font "Helvetica,20"
set output "tests/Generation/circle/delaunay.png"
set title "Clouds"
set size ratio -1
plot 'tests/Algorithm/delaunay/data_delaunay.plt' index 0 w p title "0"
