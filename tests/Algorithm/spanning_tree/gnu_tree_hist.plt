set terminal png size 900, 800 enhanced font "Helvetica,20"
set output "tests/Generation/five_ellipses/spanning_tree_hist.png"
set title "HISTOGRAM"
set style fill transparent solid 0.5
plot 'tests/Algorithm/spanning_tree/spanning_tree_hist.plt' index 0 title "distances" w boxes
