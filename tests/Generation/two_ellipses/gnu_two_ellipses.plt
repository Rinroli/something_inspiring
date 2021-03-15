set title "Clouds"
set size ratio -1
plot 'tests/Generation/two_ellipses/data_two_ellipses.plt' index 0 w p title "0",\
'tests/Generation/two_ellipses/data_two_ellipses.plt' index 1 w p title "1"
pause -1

