set title "Clouds"
set size ratio -1
plot 'tests/Generation/five_ellipses/data_five_ellipses.plt' index 0 w p title "0",\
'tests/Generation/five_ellipses/data_five_ellipses.plt' index 1 w p title "1",\
'tests/Generation/five_ellipses/data_five_ellipses.plt' index 2 w p title "2",\
'tests/Generation/five_ellipses/data_five_ellipses.plt' index 3 w p title "3",\
'tests/Generation/five_ellipses/data_five_ellipses.plt' index 4 w p title "4"
pause -1

