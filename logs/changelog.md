<!-- markdownlint-disable-file MD001 -->
# Some Data Analysis, Chaplygina Svetlana, group 211

## MSU MechMath, fall 2020

List of commands can be found in [help.txt](../help.txt).

## Changelog

### **Version 1.4.0 (16.10.2020)** - CURRENT VERSION

Added and fixed:

* *DIBINARY* (index) and *DIDBSCAN*(index) (aliases are *DIB* and *DID*) - print graph with point marks and edges of the binary matrix to the file. Can be called by gnuplot as *gnuplot data/graph.plt*.
* *SAVE* - now work (again) with clusters, taken from FindCluster.

### **Version 1.3.2 (14.10.2020)**

Added:

* New commands:
  + *BINARY* and *DBSCAN* - create binary matrix by delta (and k - number of neighbors for DBScan). All binary matrix stores in vector, you can call any of them.
  + *WAVE* and *DBWAVE* - create clustering by binary matrix (created by command *BINARY* or *DBSCAN*). New clustering stores in FindClusters.
* Points now have attributs - a, b and mark.

### **Version 1.3.1 (13.10.2020)**

Added:

- Now you can store multiple cluster systems.
  + *Dev* New class FindClusters - store clusters and info about them.
- New commands:
  + *MATRIX* or *ANALYSIS* - enter readonly mode 'Analysis' and create distance matrix. You can't change field, only examine it.
  + *INFOFC* - display info about cluster bundles (include source of clustering).
Changed:
- Сluster logs are now output to the file 'logs_algorithm.txt'.

### **Version 1.2.4 (07.10.2020)**

Added:

- Now you can see graph of the clustering - it will be generated at command *WAVE* or *DBSCAN*. Main file is 'graph.plt', data in 'graph_points.plt' and 'graph_edges.plt'.

### **Version 1.2.3 (06.10.2020)**

Added:

- Added clustering by DBSCAN algorithm. Command *DBSCAN(delta, k)*, more in [help.txt](../help.txt).

### **Version 1.2.2 (05.10.2020)**

Added:

- Histogram can be generated from the cluster (or cloud), not field.
      The id of cluster/cloud can be checked by *INFO* command. More about you can see in [help.txt](../help.txt).
- New property - containing Box for cluster/cloud - can be seen by *INFO*.

### **Version 1.2.1 (03.10.2020)**

Added:

- Clusters are printed in its own file now.
- New properties for Clouds and Clusters - (R)adius and (D)iameter. Can be printed by *INFO*.
Changed:
- *Dev* Now the Сloud inherits the Сluster.

### **Version 1.2.0 (02.10.2020)**

Added:

- Read commands from the file. File should be in 'command_files' directory.
- Wave now work with given 'delta' (or 0.1 as default).

### **Version 1.1.0 (01.10.2020)**

Added:

- Clustering data using the wave algorithm and printing these clusters to the file (with similar to cloud printing, same files, same commands)

### **Version 1.0.0 (29.09.2020)**

Introduced:

- Generation of normally distributed points (according to the specified parameters)
- Differentiation of modees (readonly/write) for writing points on the field
- Logs for the Controller, Interface, and Field (logs_ctrl.txt, logs_interface.txt, logs_field.txt in the logs folder). Logs for the cloud are output to a file with the Field logs (with the corresponding mark)
- Generating a distribution histogram (GNUplot, running (linux) via the command "gnuplot gnu_hist.plt", the data itself in the file "hist.plt", data folder)
- Saving data to disk (GNUplot, running (linux) via "gnuplot gnuplot.plot", data in a file "output.txt", data folder)
- Matrix of distances between points
