set terminal png size 900,800 enhanced font "Helvetica,20"
set output "tests/Generation/circle/spanning_tree.png"
set size ratio -1
plot 'tests/Algorithm/spanning_tree/spanning_tree_data.plt' with lines lc rgb "black" notitle
