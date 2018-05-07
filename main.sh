BASEDIR=$(pwd)
dir=$BASEDIR/dataset/
#dir=/home/dinesh/MVS_libs/cloud_evolve_mvs/CloudReconstruction/dataset/
echo "$dir"
for i in 01 #02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 
do
     imgdir=$dir/results/$i
     results_dir=$dir/results
     echo $imgdir
     mkdir $results_dir
     mkdir $imgdir
     echo "cp $dir/Image_T_$i* $imgdir"

     cp $dir/Image_T_$i* $imgdir
     echo "./mve/apps/makescene/makescene -i $imgdir $imgdir/mve"
	./mve/apps/makescene/makescene -i $imgdir $imgdir/scene_final

    cp $dir/Image_T_$i* $imgdir
    cp synth_0.out $imgdir/scene_final
	./mve/apps/GTpose/GTpose $imgdir/scene_final/
	./mve/apps/featurerecon/featurerecon $imgdir/scene_final 
	./mve/apps/dmrecon/dmrecon -s1 $imgdir/scene_final --image=original --force 
	./mve/apps/scene2pset/scene2pset -F1 $imgdir/scene_final $imgdir/scene_final/S$y.ply
	./mve/apps/fssrecon/fssrecon $imgdir/scene_final/S$y.ply $imgdir/scene_final/surface$y.ply
	./mve/apps/meshclean/meshclean $imgdir/scene_final/surface$y.ply $imgdir/scene_final/clean$y.ply


	done
done


