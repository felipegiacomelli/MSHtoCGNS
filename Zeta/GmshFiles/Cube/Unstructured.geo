length = 1.0;
height = 1.0;
width  = 1.0;

gridsize = 1e-1;

Point(1) = {0     , 0     , 0    , gridsize};
Point(2) = {length, 0     , 0    , gridsize};
Point(3) = {length, height, 0    , gridsize};
Point(4) = {0     , height, 0    , gridsize};
Point(5) = {0     , 0     , width, gridsize};
Point(6) = {length, 0     , width, gridsize};
Point(7) = {length, height, width, gridsize};
Point(8) = {0     , height, width, gridsize};

Line( 9) = {4, 1};
Line(10) = {1, 2};
Line(11) = {2, 3};
Line(12) = {3, 4};
Line(13) = {8, 5};
Line(14) = {5, 6};
Line(15) = {6, 7};
Line(16) = {7, 8};
Line(17) = {2, 6};
Line(18) = {3, 7};
Line(19) = {1, 5};
Line(20) = {8, 4};

Line Loop(1) = {20, 9, 19, -13};
Line Loop(2) = {18, -15, -17, 11};
Line Loop(3) = {-14, -19, 10, 17};
Line Loop(4) = {-16, -18, 12, -20};
//
//Line Loop(5) = {11, 12, 9, 10};
Line Loop(5) = {-10, -9, -12, -11};
//
Line Loop(6) = {13, 14, 15, 16};

Plane Surface(1) = {1};
Plane Surface(2) = {2};
Plane Surface(3) = {3};
Plane Surface(4) = {4};
Plane Surface(5) = {5};
Plane Surface(6) = {6};

Physical Surface("West")   = {1};
Physical Surface("East")   = {2};
Physical Surface("South")  = {3};
Physical Surface("North")  = {4};
Physical Surface("Bottom") = {5};
Physical Surface("Top")    = {6};

Surface Loop(1) = {1, 4, 5, 2, 6, 3};
Volume(1) = {1};
Physical Volume("Geometry") = {1};
