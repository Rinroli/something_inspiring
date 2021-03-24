set terminal png size 900,800 enhanced font "Helvetica,20"
set nokey
set output "tests/Generation/five_ellipses/em.png"
set title "Clusters, EM-algorithm; k = 5"
set size ratio -1
plot 'tests/Algorithm/em/data_em.plt' index 0 w p title "0",\
'tests/Algorithm/em/data_em.plt' index 1 w p title "1",\
'tests/Algorithm/em/data_em.plt' index 2 w p title "2",\
'tests/Algorithm/em/data_em.plt' index 3 w p title "3",\
'tests/Algorithm/em/data_em.plt' index 4 w p title "4"
