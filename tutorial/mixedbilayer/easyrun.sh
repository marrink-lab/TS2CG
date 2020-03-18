#!/bin/bash

../../PLM -bilayerThickness 3.8 -rescalefactor 1.5 -o 0_BACKMAP/point -TSfile twisted.q

../../PCG -dts 0_BACKMAP/point -str input.str -seed 39 -Bondlength 0.15 -LLIB Martini2.LIB -defout 0_BACKMAP/mixed

mv Wall.gro 0_BACKMAP

echo "#include \"../Dry_Martini_v2.1/dry_martini_v2.1.itp\" "> t2.txt

echo "#include \"../Dry_Martini_v2.1/dry_martini_v2.1_lipids.itp\" "> t1.txt

cat t1.txt >> t2.txt


cat 0_BACKMAP/mixed.top >>t2.txt

mv t2.txt 0_BACKMAP/mixed.top

rm t1.txt
rm t2.txt

mkdir 1_EM
cd 1_EM
gmx grompp -f ../mdp/em0.mdp -c ../0_BACKMAP/mixed.gro -p ../0_BACKMAP/mixed.top -o em -r ../0_BACKMAP/mixed.gro
gmx mdrun -s em.tpr -deffnm em -v 
cd ..

mkdir 2_EQ
cd 2_EQ
gmx grompp -f ../mdp/EQ.mdp -c ../1_EM/em.gro -p ../0_BACKMAP/mixed.top -o eq -r ../1_EM/em.gro
gmx mdrun -s eq.tpr -deffnm eq -v
cd ..


mkdir 3_RUN
cd 3_RUN
gmx grompp -f ../mdp/RUN.mdp -c ../2_EQ/eq.gro -p ../0_BACKMAP/mixed.top -o RUN 
gmx mdrun -s RUN.tpr -deffnm RUN -v
cd ..
