# something_inspiring

Here you can find several algorithms written in **C++**.

Some info can be found in [help.txt](help.txt) and [changelog.md](logs/changelog.md).

Currently, some clustering algorithms are implemented:

* **DBSCAN**
* **Minimal spanning tree** - can find *epsilon* for DBSCAN
* **K-means**
* **K-means with kernels**
* **EM-algorithm** - and produce *gif*-image with all steps
* **FOREL** - 3 levels - points, centroids and centroids from centroids. Produce *gif*-image and 3 groups of clusters.
* **Hierarchical** - the linkage criterion is *Unweighted average linkage clustering* (or *UPGMA*)

(**UPDATED**)
Also **Delaunay triangulation** is implemented (algorithm "remove and build")!

Before data analysis, you can work with normally distributed clouds by generating and working with a **buffer** - *rotate*, *shift*, *scale*.

**Include test system.** You can run tests from *CLI* interface or from *bash-scripts*:

* For *CLI* enter *'test'* and then choose test type, test name and generation file (for *Algorithm*). All can be find at [tests](tests), generation files are in [Generation](tests/Generation)
* For *bash-scripts* run the script you want from [bash_tests](bash_tests)

All result data and GNUplot files will be at main test directory, result pictures will be in in the directory of the generation file.

Gnuplot run in 'silent' mode - only picture generation.
