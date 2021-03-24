<!-- markdownlint-disable-file MD001 -->
# Some Data Analysis, Chaplygina Svetlana, group 211

# [Github](https://github.com/Rinroli/something_inspiring)

## MSU MechMath, fall 2020

List of commands can be found in [help.txt](../help.txt).

## Changelog

### **Version 2.1.0 (24.03.2021)** - CURRENT VERSION

Added:

* Massive **Test System** update! Now you can run tests from *bash-scripts* - all at once or one at a time. With different generation file. Can be find [here](../bash_tests).

* Some more examples - now in three *pdf-files* with different generation files.

### **Version 2.0.2 (19.03.2021)**

Refactored:

* **Global refactoring** Each class in its file.

### **Version 2.0.1 (15.03.2021)**

Added:

* **New test system**. One test in directory, the tests are divided into topics.

### **Version 2.0.0 (10.03.2021)**

Added:

* **Delaunay triangulation!** *"Remove and build"* algorithm. Make *gif*-image - many different styles, to switch them see comments in the code.

### **Version 1.10.1 (9.12.2020)**

Added:

* *Gif*-images for *hierarchial* algorithm.

### **Version 1.10.0 (9.12.2020)**

Added:

* **Hierarchical clustering** algorithm! The linkage criterion is *Unweighted average linkage clustering* (or *UPGMA*). No *gif*-image generation yet.

### **Version 1.9.1 (1.12.2020)**

Added:

* Full *FOREL* algorithm! Works at three levels - points, centers and centers of clusters of centers.
  Make *gif*-image with all levels. Also add 3 groups of clusters - 3 FindClusters - for each step.

### **Version 1.9.0 (30.11.2020)**

Added:

* The first level for *FOREL* algorithm - work with points, but not with centers and etc.

### **Version 1.8.0 (26.11.2020)**

Added:

* Absolutly **new** config-system! Now you can disable (or enable) logs and save them to your own files. All your your configs will be saved for future use in [config.txt](../config.txt). Edits can be made either directly to the file or at the start of the program.

* Now you can enter number of points at normally distributed cloud, in command *GC*

### **Version 1.7.0 (11.11.2020)**

Added:

* New custering algorithm - **k-means with kernels**! like a normal *k-means*, but with an additional call to the *k-means* at each step and each group. The command is *KERKMEANS*. *Gif*-image is included.

### **Version 1.6.1 (4.11.2020)**

Added:

* Now you can see *gif*-image with steps (and ellipses!) of em-algorithm.
    More info in [help.txt](../help.txt)

### **Version 1.6.0 (3.11.2020)**

Added:

* **ExpectationMaximization Algorithm!** Find clusters from mixture of gaussians by command *EM* and initial number of clusters.

### **Version 1.5.1 (27.10.2020)**

Added:

* New commands for the buffer - *MOVEB*, *ZOOMB* and *EMPTYB*.

Fixed and changed:

* Fix *ROTB* - rotate buffer. And now it works in *degrees*.
* Small fixes.

### **Version 1.5.0 (26.10.2020)**

Added:

* **Buffer!** Now you can add clouds to the buffer and then modify them. Commands are *ADDB*, *SHOWB*, *ROTB* and *PUTB* -- more info can be seen in [help.txt](../help.txt).

### **Version 1.4.4 (19.10.2020)**

Changed:

* Сhanged the implementation of the algorithm k-means - now it works through scalar products.

### **Version 1.4.3 (18.10.2020)**

Added:

* *KMEANS* - make clustering using k-means algorithm. Argument is initial number of clusters. New clustering stores in FindClusters.

### **Version 1.4.2 (18.10.2020)**

Added:

* *STREE* - build minimal spanning tree on the field (bu Prim algorithm.)
  Result can be seen in 'tree_data.plt'. For *gnuplot* - 'gnu_tree.plt'.
* *STRHIST* - build histogram for the edges of the tree.
  Result can be seen in 'tree_hist.plt'. For *gnuplot* - 'gnu_tree_hist.plt'.
* *FINDR* - find the best *delta* for wave algorithm basing on minimal spanning tree.

### **Version 1.4.1 (17.10.2020)**

Fixed:

* *HIST* work with clusters from FindClusters - new syntax can be seen in [help.txt](../help.txt).

### **Version 1.4.0 (16.10.2020)**

Added and fixed:

* *DIBINARY* (index) and *DIDBSCAN*(index) (aliases are *DIB* and *DID*) - print graph with point marks and edges of the binary matrix to the file. Can be called by gnuplot as *gnuplot data/graph.plt*.
* *SAVE* now work (again) with clusters, taken from FindCluster.

### **Version 1.3.2 (14.10.2020)**

Added:

* New commands:
  + *BINARY* and *DBSCAN* - create binary matrix by delta (and k - number of neighbors for DBScan). All binary matrix stores in vector, you can call any of them.
  + *WAVE* and *DBWAVE* - make clustering by binary matrix (created by command *BINARY* or *DBSCAN*). New clustering stores in FindClusters.
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
