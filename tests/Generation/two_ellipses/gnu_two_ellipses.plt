set terminal png size 1920,1080 enhanced font "Helvetica,20"
set output "tests/Generation/two_ellipses/two_ellipses.png"
set title "Clouds"
set size ratio -1
plot 'tests/Generation/two_ellipses/data_two_ellipses.plt' index 0 w p title "0",\
'tests/Generation/two_ellipses/data_two_ellipses.plt' index 1 w p title "1"
