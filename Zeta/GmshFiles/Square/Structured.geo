length = 1.0;
height = 1.0;
gridsize = 1e-1;

Point(1) = {0     , 0     , 0, gridsize};
Point(2) = {length, 0     , 0, gridsize};
Point(3) = {length, height, 0, gridsize};
Point(4) = {0     , height, 0, gridsize};

Line(7) = {1,2};
Line(8) = {2,3};
Line(9) = {3,4};
Line(10) = {4,1};
Line Loop(14) = {7,8,9,10};
Plane Surface(15) = 14;

Transfinite Line{7, 8, 9, 10} = 1/gridsize;
Transfinite Surface{15};
Recombine Surface{15};

//Transfinite Line{7, 8, 9, 10} = 1/gridsize Using Bump 0.25;
//Transfinite Line{7, 8}        = 1/gridsize Using Progression 1.2;
//Transfinite Line{-9, -10}     = 1/gridsize Using Progression 1.2;

Physical Line("West")        = {10};
Physical Line("East")        = {8};
Physical Line("South")       = {7};
Physical Line("North")       = {9};
Physical Surface("Geometry") = {15};
