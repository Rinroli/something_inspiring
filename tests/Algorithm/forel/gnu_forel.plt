set terminal png size 900,800 enhanced font "Helvetica,20"
set nokey
set output "tests/Generation/five_ellipses/forel.png"
set title "Clusters, FOREL, exported, level #0"
set size ratio -1
plot 'tests/Algorithm/forel/data_forel.plt' index 0 w p title "0",\
'tests/Algorithm/forel/data_forel.plt' index 1 w p title "1",\
'tests/Algorithm/forel/data_forel.plt' index 2 w p title "2",\
'tests/Algorithm/forel/data_forel.plt' index 3 w p title "3",\
'tests/Algorithm/forel/data_forel.plt' index 4 w p title "4",\
'tests/Algorithm/forel/data_forel.plt' index 5 w p title "5",\
'tests/Algorithm/forel/data_forel.plt' index 6 w p title "6",\
'tests/Algorithm/forel/data_forel.plt' index 7 w p title "7",\
'tests/Algorithm/forel/data_forel.plt' index 8 w p title "8"
