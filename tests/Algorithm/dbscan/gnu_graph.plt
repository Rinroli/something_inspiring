plot \
  'tests/Algorithm/dbscan/dbscan_edges.plt'   with lines lc rgb "black" notitle,\
  'tests/Algorithm/dbscan/dbscan_points.plt' index 0 w p lc rgb "blue" title "dust",\
  'tests/Algorithm/dbscan/dbscan_points.plt' index 1 w p lc rgb "red" title "peripheral",\
  'tests/Algorithm/dbscan/dbscan_points.plt' index 2 w p lc rgb "green" title "base"
pause -1

