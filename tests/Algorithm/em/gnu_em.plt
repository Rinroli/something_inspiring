set title "Clouds"
set size ratio -1
plot 'tests/Algorithm/em/data_em.plt' index 0 w p title "0",\
'tests/Algorithm/em/data_em.plt' index 1 w p title "1",\
'tests/Algorithm/em/data_em.plt' index 2 w p title "2"
pause -1

