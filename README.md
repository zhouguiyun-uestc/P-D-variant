# P&D-variant

**Manuscript Title**: A variant of the P&D algorithm for filling depressions in raster digital elevation models

**Authors**: Hongqiang Wei, Guiyun Zhou

**Corresponding author**: Guiyun Zhou (zhouguiyun@uestc.edu.cn)

This repository contains the source codes of the algorithm presented in the manuscript above. These codes were used in performing the tests described in the manuscript.

FillDEM supports floating-point GeoTIFF file format through the GDAL library. Please include GDAL library into your compilation. GDAL 1.91 was used in our experiments.

The algorithms described in the manuscript can be run using the following command line:

FillDEM fillingMethod inputfileName outputfileName

Example: FillDEM WT f:\\dem\\dem.tif f:\\dem\\dem_filled.tif

Use the algorithm proposed in the manuscript to fill the input DEM "dem.tif" and create a filled DEM "dem_filled.tif".

The test data used in the manuscript can be downloaded at http://www.mngeo.state.mn.us. You need ArcGIS to convert the DEM into GeoTIFF format.
