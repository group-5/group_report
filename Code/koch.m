koch[n_] := Graphics[{Line@(
      FoldList[
       #1 + AngleVector[{#2[[1]], #2[[2]]}] &, {0, 0},
       Select[
        FoldList[{#2[[1]], Mod[#1[[2]] + #2[[2]], 2 Pi]} &,
         Table[Switch[ThueMorse[i],
           0, {1, 0},
           1, {0, Pi/3}], {i, 0, 2^n}]],
        First[#] != 0 &]
       ]),
    Red, PointSize@Large, Point[{0, 0}]}, ImageSize -> Medium];

kochErr[n_, er_: 0.1, et_: (2 Pi)/360] := Graphics[{Line@(
      FoldList[
       #1 + 
         AngleVector[{RandomVariate[
            NormalDistribution[#2[[1]], er]], #2[[2]] + 
            et*RandomVariate[NormalDistribution[0, 1]]}] &, {0, 0},
       Select[
        FoldList[{#2[[1]], Mod[#1[[2]] + #2[[2]], 2 Pi]} &,
         Table[Switch[ThueMorse[i],
           0, {1, 0},
           1, {0, Pi/3}], {i, 0, 2^n}]],
        First[#] != 0 &]
       ]),
    Red, PointSize@Large, Point[{0, 0}]}, ImageSize -> Medium];

Rasterize@Grid[Table[
   Flatten[{koch[n], Table[kochErr[n], {4}]}, 1],
   {n, 10, 14}], Dividers -> {{False, True}, None}]
