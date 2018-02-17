length = 1.0;
height = 1.0;
standard = 10 * 1e-1;

Point(1) = {0     , 0     , 0, standard};
Point(2) = {length, 0     , 0, standard};
Point(3) = {length, height, 0, standard};
Point(4) = {0     , height, 0, standard};

Line(1)  = {1,2};
Line(2)  = {2,3};
Line(3)  = {3,4};
Line(4) = {4,1};

Line Loop(1)     = {1,2,3,4};
Plane Surface(1) = 1;

Physical Line("West")        = {4};
Physical Line("East")        = {2};
Physical Line("South")       = {1};
Physical Line("North")       = {3};
Physical Surface("Geometry") = {1};
