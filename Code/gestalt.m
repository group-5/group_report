sample[imgStr_] := 
  Module[{img, pos, tour, noisy, instructions}, 
   img = ColorQuantize[
     ColorConvert[
      ImageResize[ExampleData[{"TestImage", imgStr}], 100], 
      "Grayscale"], 2];
   pos = PixelValuePositions[ImageAdjust@img, 0];
   tour = FindShortestTour[pos];
   instructions = (ToPolarCoordinates[#2 - #1] & @@@ 
       Partition[pos[[tour[[2]]]], 2, 1]) /. {r_, theta_} /; r > 1 :> 
      Sequence @@ (If[IntegerQ@r, Identity, 
          Append[#, {FractionalPart@r, theta}] &]@
         ConstantArray[{1, theta}, Floor[r]]);
   noisy := 
    With[{er = 0.1, et = (2 Pi)/360}, 
     Graphics[{Line@
        FoldList[#1 + 
           AngleVector[{RandomVariate[
              NormalDistribution[#2[[1]], er]], #2[[2]] + 
              et*RandomVariate[NormalDistribution[0, 1]]}] &, {0, 0}, 
         instructions], Red, PointSize@Large, Point[{0, 0}]}, 
      ImageSize -> Medium]];
   Flatten[{Image[img, ImageSize -> Medium], 
     Graphics[{Line[pos[[tour[[2]]]]], Red, PointSize@Large, 
       Point[pos[[tour[[2, 1]]]]]}, ImageSize -> Medium], 
     Table[noisy, {3}]}, 1]];

Rasterize@
 Grid[Table[sample[i], {i, {"Boat", "Girl2", "Girl3", "Mandrill"}}], 
  Dividers -> {{False, True, True}, None}]
