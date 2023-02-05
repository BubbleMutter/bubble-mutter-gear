#include<opencv2/core.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>

Mat imread(const string & filename, int flags = 1);
void imwrite(const string & filename, InputArray img,
        const vector<int>& params = vector<int>());

void namedWindow(const string & winname, /* unique id-name */
        int flags = WINDOW_AUTOSIZE);
void imshow(const string & winname, InputArray mat);

int createTrackbar(const string & trackbarname, /* unique id-name in a window */
        const string & winname, /* window */
        int *value, /* output-param, slider-value */
        int count, /* slider maximum */
        void (* onChange)(int slider, void *userdata) = NULL, /* callback handler */
        void *userdata); /* pass to callback handler */
/* different window can have a "same" name trackbarname */

int getTrackbarPos(const string & trackbarname,
        const string & winname); /* return slider value */

int SetMouseCallback(const string & winname,
        void (* onMouse)(int event, int x, int y, int flags, void *userdata), /* callback handler */
        void *userdata = NULL);
/* x, y are beyond to the image coordinate system,
 * while not the display-window one
 * @flags is combination of CV_EVENT_xxx
 */

/* difference between Mat pointer and its memory space */
Mat image = imread("filename", 2 | 4); /* load a loseless image */
Mat image = imread("filename", 0); /* load a grey-scale image */
Mat image = imread("filename", 199); /* load a 3-channel image */

/* Mat just copy pointer */
Mat A, C;
A = imread("filename");
Mat B(A);
C = A;
/* A, B, C are shared a same matrix */

/* Mat create a new matrix with quotes of a old matrix */
Mat D(A, Rect(10, 10, 100, 100)); /* with rectangle */
Mat E = A(Range:all(), Range(1, 3)); /* with rows and columns */

/* Mat absolutely duplicates a matrix */
Mat F = A.clone();
Mat G;
A.copyTo(G);

/* third params in Mat() */
CV_[bits][(un)signed][types_prefix]C[num];
@bits: number of bits per item
@singed: U means unsigned,
@types_prefix:
@num: the numbers of channel
/* ===================== */

/****** core data structure *******/
// Mat
Mat M(2, 2, CV_8UC3, Scalar(0, 0, 255));

int sz[3] = {2, 2, 2};
Mat L(3, sz, CV_8UC, Scalar::all(0));

IplImage *img = cvLoadImage("1.jpg", 1);
Mat mtx(img);

Mat M;
M.create(4, 4, CV_8U(2));

Mat E = Mat::eye(4, 4, CV_64F);
Mat O = Mat::ones(2, 2, CV_32F);
Mat Z = Mat::zeros(3, 3, CV_8UC1);

Mat C = (Mat_<double>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);

Mat RowClone = C.row(1).clone();

Mat r = Mat(10, 3, CV_8UC3);

randu(img, Scalar::all(0), Scalar::all(255));
img.create();
img.clone();
img.size();
img.col(x).clone();
img.row(y).clone();
img.depth(),
img.channels(),
img.rows,
img.cols,
img.at<Vec3b>(row, col)[channel],
/* ================= Mat */

Size(width, height); /* functor */

Point point; point.x = 10; point.y = 20;
Point point(10, 20); /* point */

Scalar(blue, green, red); /* color */
Scalar(blue, green, red, alpha); /* color */
Scalar::all(x);

/* arraykernel, discribe a kernel-shape */
int StructElementSize = 3;
Mat element = getStructuringElement(MORPH_RECT,
        Size(2*Structelementsize+1, 2*StructElementSize+1),
        Point(StructElementSize, StructElementSize));
/* MORPH_RECT, MORPH_CROSS, MORPH_ELLIPSE are available */

Rect rect; rect.x = 0; rect.y = 0; rect.width = 10; rect.height = 20;
Rect rect(0, 0, 10, 20);
rect.Size();
rect.area();
rect.contains(Point); /* check a point whether inside */
rect.inside(Rect);  /* check a rectangle whether inside */
rect.tl(); /* top-left coordinate */
rect.br(); /* bottom-right coordinate */
Rect rect = rect1 & rect2;
Rect rect = rect1 | rect2;
Rect rectShift = rect + point;  /* rectangle shift */
Rect rectScala = rect + size;   /* rectangle scala */
/**************************/

/* basic geometric drawing */
Mat img;
int thickness = 2, lineType = 8;
#define WINDOW_WIDTH (600)

#define PointNum
Point rookPoints[PointNum];
const Point *ppt = rookPoints;
int npt = PointNum;
fillPoly(img, &ppt, &npt, 1, Scalar(255, 255, 255), lineType);

Point center;
circle(img, center, WINDOW_WIDTH/32,
        Scalar(0, 0, 255), thickness, lineType);

double angle;
ellipse(img, Point(WINDOW_WIDTH/2, WINDOW_WIDTH/2),
        Size(WINDOW_WIDTH/4, WINDOW_WIDTH/16), angle,
        0, 360, Scalar(255, 129, 0), thickness, lineType);
/* ========================== */

/* convert */
cvtColor(srcImage, dstImage, COLOR_xxx2yyy);
resize(srcImage, dstImage, Size(width, height));

(int)theRNG(); /* return a random */
/* =================== */

/* LUT(), usage */
table[256]; /* user-define */
/* initialize the table[] */
/* create the Mat-table */
Mat lookUpTable(1, 256, CV_8U);
uchar *p = lookUpTable.data;
for(int i = 0; i < 256; i++) {
    p[i] = table[i]; /* user-init */
}
/* use Mat-table, use LUT, LoopUpTable */
for(int i = 0; i < times; i++) {
    LUT(input, lookUpTable, output);
}
/* =================== */

/* calculate time */
double begin = static_cast<doule>(getTickCount());
/* image process ... */
end = ((double)getTickCount() - time0);
/* =================== */

/* YAML and XML */
FileStorage fs("fileName", FileStorage::READ);
FileStorage fs("fileName", FileStorage::WRITE);
fs.release();
FileNode xxx;
FileNodeIterator xxx;
/* =================== */

/* blur and filter */
boxFilter(src, dst, -1, Size(width, height));
blur(src, dst, Size(width, height), Point(-1, -1));
GaussianBlur(src, dst, Size(width, height), sigmaX, sigmaY);
medianBlur(src, dst, ksize);
bilateralFilter(src, dst, diameter, sigmaColor, sigmaSpace);
/* =============== */

/****** Morphology *******/
/* dilate and erode */
Mat element = getStructuringElement(...);
erode(src, dst, element);   /* erode */
dilate(src, dst, element);  /* dilate */

morphologyEx(src, dst,
        MORPH_xxx, element);
/* @MORPH_xxx :
 * MORPH_OPEN, MORPH_CLOSE,
 * MORPH_GRADIENT,
 * MORPH_TOPHAT, MORPH_BLACKHAT,
 * MORPH_ERODE, MORPH_DILATE */
/* ============== */
