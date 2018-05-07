# CloudReconstruction
We show a method for reconstruction of cloud evolution using muli-view stereo based optimization

Compilation of the code:
```
$git clone https://github.com/dineshreddy91/CloudReconstruction
$cd CloudReconstruction
$make -C mve
$make -C smvs

```

Dataset Extraction:

```
$unzip dataset.zip 
$sh main.sh
```

The files are saved as 'S.ply' in pwd/dataset/results/01/scene_final. where 01 is the first time instance.
