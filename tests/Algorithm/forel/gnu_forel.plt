set title "Clouds"
set size ratio -1
plot 'tests/Algorithm/forel/data_forel.plt' index 0 w p title "0",\
'tests/Algorithm/forel/data_forel.plt' index 1 w p title "1",\
'tests/Algorithm/forel/data_forel.plt' index 2 w p title "2"
pause -1

