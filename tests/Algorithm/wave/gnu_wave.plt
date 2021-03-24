set terminal png size 900,800 enhanced font "Helvetica,20"
set nokey
set output "tests/Generation/five_ellipses/wave.png"
set title "Clusters, Wave clustering (delta = 0.200000)"
set size ratio -1
plot 'tests/Algorithm/wave/data_wave.plt' index 0 w p title "0",\
'tests/Algorithm/wave/data_wave.plt' index 1 w p title "1",\
'tests/Algorithm/wave/data_wave.plt' index 2 w p title "2",\
'tests/Algorithm/wave/data_wave.plt' index 3 w p title "3",\
'tests/Algorithm/wave/data_wave.plt' index 4 w p title "4",\
'tests/Algorithm/wave/data_wave.plt' index 5 w p title "5"
