#include "image.h"
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

#ifndef WIN32
#include <unistd.h>
#endif

#ifndef DISABLE_JPEG
#  include <jpeglib.h>
#  include <setjmp.h>
#endif

#ifndef DISABLE_OPENGL
#  include <GL/glut.h>
#endif


static const int maxvals[9] = { 0, 1, 3, 7, 15, 31, 63, 127, 255 };

static inline unsigned char val2bits (double v, int b)
{
  return (unsigned char) (floor(v * maxvals[b] + 0.5) / maxvals[b] * 255);
}


Image::Image ()
{
  width    = 0;
  height   = 0;
  channels = 0;
  bits     = 0;
  maxValue = 0;
  pixels   = NULL;
}


Image::Image (int width_, int height_)
{
  width    = width_;
  height   = height_;
  channels = 3;
  bits     = 8;
  maxValue = 255;

  assert(width > 0 && height > 0 &&
         (channels == 1 || channels == 3 || channels == 4) &&
         bits > 0 && bits < 9
        );

  pixels   = new unsigned char[width*height*channels];;
  memset(pixels, 0, width*height*channels);
}


Image::Image (int width_, int height_, int channels_)
{
  width    = width_;
  height   = height_;
  channels = channels_;
  bits     = 8;
  maxValue = 255;

  assert(width > 0 && height > 0 &&
         (channels == 1 || channels == 3 || channels == 4) &&
         bits > 0 && bits < 9
        );

  pixels   = new unsigned char[width*height*channels];
  memset(pixels, 0, width*height*channels);
}


Image::Image (int width_, int height_, int channels_, int bits_)
{
  width    = width_;
  height   = height_;
  channels = channels_;
  bits     = bits_;
  maxValue = val2bits(1.0,bits);

  assert(width > 0 && height > 0 &&
         (channels == 1 || channels == 3 || channels == 4) &&
         bits > 0 && bits < 9
        );

  pixels   = new unsigned char[width*height*channels];
  memset(pixels, 0, width*height*channels);
}


Image::Image (const char* filename)
{
  width    = 0;
  height   = 0;
  channels = 0;
  bits     = 0;
  maxValue = 0;
  pixels   = NULL;

  read(filename);
}


Image::~Image ()
{
  if (pixels) delete[] pixels;
}


Image::Image (const Image& image)
{
  width    = image.width;
  height   = image.height;
  channels = image.channels;
  bits     = image.bits;
  maxValue = image.maxValue;
  pixels   = new unsigned char[width*height*channels];

  for (int i = 0; i < width*height*channels; ++i)
    pixels[i] = image.pixels[i];
}


Image& Image::operator= (const Image& image)
{
  if (&image == this) return *this;

  if (pixels) delete[] pixels;

  width    = image.width;
  height   = image.height;
  channels = image.channels;
  bits     = image.bits;
  maxValue = image.maxValue;
  pixels   = new unsigned char[width*height*channels];

  for (int i = 0; i < width*height*channels; ++i)
    pixels[i] = image.pixels[i];

  return *this;
}


bool Image::good ()
{
  return (width > 0 && height > 0 &&
          (channels == 1 || channels == 3 || channels == 4) &&
          bits > 0 && bits < 9 && pixels);
}


bool Image::bad ()
{
  return !good();
}


void Image::clear ()
{
  memset(pixels, 0, width*height*channels);
}


int Image::index (int x, int y, int c)
{
  return (((height - y - 1) * width + x) * channels + c);
}


double Image::getPixel (int x, int y, int channel)
{
  assert(good());
  assert((x >= 0)       &&
         (x <  width)   &&
         (y >= 0)       &&
         (y <  height)  &&
         (channel >= 0) &&
         (channel < channels));

  return pixels[index(x,y,channel)] / 255.0;
}


double Image::getPixel_ (int x, int y, int channel)
{
  if (!good()        ||
      (x <  0)       ||
      (x >= width)   ||
      (y <  0)       ||
      (y >= height)  ||
      (channel < 0)  ||
      (channel >= channels))
    return 0.0;

  return getPixel(x,y,channel);
}


Pixel Image::getPixel (int x, int y)
{
  assert(good());
  assert((x >= 0)       &&
         (x <  width)   &&
         (y >= 0)       &&
         (y <  height));

  Pixel pixel;
  memset(&pixel, 0, sizeof(Pixel));

  switch (channels)
  {
    case 4:
      pixel.a = pixels[index(x,y,ALPHA)] / 255.0;

    case 3:
      pixel.b = pixels[index(x,y,BLUE)]  / 255.0;
      pixel.g = pixels[index(x,y,GREEN)] / 255.0;

    case 1:
      pixel.r = pixels[index(x,y,RED)]   / 255.0;

    default:
      break;
  }
  return pixel;
}


Pixel Image::getPixel_ (int x, int y)
{
  if (!good()        ||
      (x <  0)       ||
      (x >= width)   ||
      (y <  0)       ||
      (y >= height))
  {
    Pixel pixel;
    memset(&pixel, 0, sizeof(Pixel));
    return pixel;
  }

  return getPixel(x,y);
}


Pixel& Image::getPixel (int x, int y, Pixel& pixel)
{
  assert(good());
  assert((x >= 0)       &&
         (x <  width)   &&
         (y >= 0)       &&
         (y <  height));

  memset(&pixel, 0, sizeof(Pixel));

  switch (channels)
  {
    case 4:
      pixel.a = pixels[index(x,y,ALPHA)] / 255.0;

    case 3:
      pixel.b = pixels[index(x,y,BLUE)]  / 255.0;
      pixel.g = pixels[index(x,y,GREEN)] / 255.0;

    case 1:
      pixel.r = pixels[index(x,y,RED)]   / 255.0;

    default:
      break;
  }
  return pixel;
}


Pixel& Image::getPixel_ (int x, int y, Pixel& pixel)
{
  if (!good()        ||
      (x <  0)       ||
      (x >= width)   ||
      (y <  0)       ||
      (y >= height))
  {
    memset(&pixel, 0, sizeof(Pixel));
    return pixel;
  }

  return getPixel(x,y,pixel);
}


void Image::setPixel (int x, int y, int channel, double value)
{
  assert(good());
  assert((x >= 0)       &&
         (x <  width)   &&
         (y >= 0)       &&
         (y <  height)  &&
         (channel >= 0) &&
         (channel < channels));
  assert((value >= 0.0) && 
         (value <= 1.0));  

  pixels[index(x,y,channel)] = val2bits(value, bits);
}


void Image::setPixel_ (int x, int y, int channel, double value)
{
  if (!good()        ||
      (x <  0)       ||
      (x >= width)   ||
      (y <  0)       ||
      (y >= height)  ||
      (channel < 0)  ||
      (channel >= channels) ||
      (value < 0.0)  || 
      (value > 1.0))
    return;

  setPixel(x,y,channel,value);
}


void Image::setPixel (int x, int y, Pixel& pixel)
{
  assert(good());
  assert((x >= 0)       &&
         (x <  width)   &&
         (y >= 0)       &&
         (y <  height));

  switch (channels)
  {
    case 4:
      assert((pixel.a >= 0.0) && 
             (pixel.a <= 1.0));  
      pixels[index(x,y,ALPHA)] = val2bits(pixel.a, bits);

    case 3:
      assert((pixel.b >= 0.0) && 
             (pixel.b <= 1.0));  
      assert((pixel.g >= 0.0) && 
             (pixel.g <= 1.0));  
      pixels[index(x,y,BLUE)]  = val2bits(pixel.b, bits);
      pixels[index(x,y,GREEN)] = val2bits(pixel.g, bits);

    case 1:
      assert((pixel.r >= 0.0) && 
             (pixel.r <= 1.0));  
      pixels[index(x,y,RED)]   = val2bits(pixel.r, bits);

    default:
      break;
  }
}


void Image::setPixel_ (int x, int y, Pixel& pixel)
{
  if (!good()        ||
      (x <  0)       ||
      (x >= width)   ||
      (y <  0)       ||
      (y >= height))
    return;

  setPixel(x,y,pixel);
}


#ifndef DISABLE_OPENGL

void Image::glReadPixelsWrapper ()
{
  assert(good());

  glPixelStorei(GL_PACK_ALIGNMENT, 1);
      
  switch (channels)
  {
    case 1:
      glReadPixels(0, 0, width, height, GL_LUMINANCE, GL_UNSIGNED_BYTE,
                   pixels);
      break;
 
    case 3:
      glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
      break;
 
    case 4:
      glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
      break;
         
    default:
      break;
  } 
}


void Image::glDrawPixelsWrapper ()
{
  assert(good());

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  switch (channels)
  {
    case 1:
      glDrawPixels(width, height, GL_LUMINANCE, GL_UNSIGNED_BYTE, pixels);
      break;

    case 3:
      glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
      break;

    case 4:
      glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
      break;

    default:
      break;
  }
}


void Image::glTexImage2DWrapper ()
{
  assert(good());

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  switch (channels)
  {
    case 1:
      glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0,
          GL_LUMINANCE, GL_UNSIGNED_BYTE, pixels);
      break;

    case 3:
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
          GL_RGB, GL_UNSIGNED_BYTE, pixels);
      break;

    case 4:
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
          GL_RGBA, GL_UNSIGNED_BYTE, pixels);
      break;

    default:
      break;
  }
}


void Image::glTexSubImage2DWrapper ( int x, int y)
{
  assert(good());

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  switch (channels)
  {
    case 1:
      glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height,
          GL_LUMINANCE, GL_UNSIGNED_BYTE, pixels);
      break;

    case 3:
      glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height,
          GL_RGB, GL_UNSIGNED_BYTE, pixels);
      break;

    case 4:
      glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height,
          GL_RGBA, GL_UNSIGNED_BYTE, pixels);
      break;

    default:
      break;
  }
}


#endif // DISABLE_OPENGL


int Image::read (const char* filename)
{
  FILE* file = fopen( filename, "rb" );

  if (!file)
  {
    perror("Image::read");
    return -1;
  }

  unsigned char type[2];
  fread((void*) type, sizeof(char), 2, file);
  if (ferror(file))
  {
    fclose(file);
    perror("Image::read");
    return -1;
  }

  int result = 0;

  if (type[0] == 'P' && 
      (type[1] == '1' ||
       type[1] == '2' ||
       type[1] == '3' ||
       type[1] == '5' ||
       type[1] == '6'))
  {
    fseek(file, 0, SEEK_SET);
    result = readPNM(file);
  }
  else if ((type[0] == 0x4D && type[1] == 0x42) ||
           (type[1] == 0x4D && type[0] == 0x42))
  {
    fseek(file, 0, SEEK_SET);
    result = readBMP(file);
  }
#ifndef DISABLE_JPEG
  else if ((type[0] == 0xD8 && type[1] == 0xFF) ||
           (type[1] == 0xD8 && type[0] == 0xFF))
  {
    fseek(file, 0, SEEK_SET);
    result = readJPG(file);
  }
#endif
  else
  {
    fprintf( stderr, "Image::read: unrecognized file type\n" );
    result = -1;
  }

  fclose(file);
  return result;
}


int Image::write (const char* filename)
{
  size_t len = strlen(filename);
  const char* ext = &(filename[len-4]);
  int result = 0;

  if (strncmp(ext, ".pnm", 4) == 0)
  {
    result = writePNM(filename);
  }
  else if (strncmp(ext, ".bmp", 4) == 0)
  {
    result = writeBMP(filename);
  }
#ifndef DISABLE_JPEG
  else if (strncmp(ext, ".jpg", 4) == 0)
  {
    result = writeJPG(filename);
  }
#endif
  else
  {
    char filenamewithext[1024];
#ifdef WIN32
    _snprintf( filenamewithext, 1024, "%.1019s.pnm", filename );
#else
    snprintf( filenamewithext, 1024, "%.1019s.pnm", filename );
#endif
    result = writePNM(filenamewithext);
  }

  return result;
}



/******************************************************************
* .JPG file  manipulation  
*/

#ifndef DISABLE_JPEG

int Image::readJPG (const char* filename)
{
  FILE* file = fopen(filename, "rb" );

  if (!file)
  {
    perror("Image::readJPG");
    return -1;
  }

  int result = readJPG(file);
  fclose(file);

  return result;
}


int Image::writeJPG (const char* filename)
{
  FILE* file = fopen(filename, "wb");

  if (!file)
  {
    perror("Image::writeJPG");
    return -1;
  }

  int result = writeJPG(file);
  fclose(file);

  if (result == -1)
    unlink(filename);

  return result;
}


struct my_error_mgr 
{
  struct jpeg_error_mgr pub;
  jmp_buf setjmp_buffer;
};


typedef struct my_error_mgr * my_error_ptr;


void my_error_exit (j_common_ptr cinfo)
{
  my_error_ptr myerr = (my_error_ptr) cinfo->err;
  (*cinfo->err->output_message) (cinfo);
  longjmp(myerr->setjmp_buffer, 1);
}


int Image::readJPG (FILE* file)
{
  struct jpeg_decompress_struct cinfo;
  struct my_error_mgr jerr;
  JSAMPROW row_pointer[1];

  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = my_error_exit;

  // failure jump point
  if (setjmp(jerr.setjmp_buffer)) 
  {
    jpeg_destroy_decompress(&cinfo);
    fprintf(stderr, "Image::readJPG: jpeg decompression error");
    return -1;
  }

  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, file);
  jpeg_read_header(&cinfo, TRUE);
  jpeg_start_decompress(&cinfo);

  if (cinfo.out_color_space != JCS_RGB &&
      cinfo.out_color_space != JCS_GRAYSCALE)
  {
    fprintf(stderr, "Image::readJPG: unrecognized colorspace");
    return -1;
  }

  width    = cinfo.output_width;
  height   = cinfo.output_height;
  bits     = 8;
  channels = cinfo.out_color_components;
  pixels   = new unsigned char[width*height*channels];
  memset(pixels, 0, width*height*channels);

  int row_stride = width * channels;

  while (cinfo.output_scanline < cinfo.output_height) 
  {
    row_pointer[0] = &pixels[(height - cinfo.output_scanline - 1) * row_stride];
    jpeg_read_scanlines(&cinfo, row_pointer, 1);
  }

  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);

  return 0;
}


int Image::writeJPG (FILE* file)
{
  if (!good())
  {
    fprintf(stderr, "Image::writeJPG: bad image\n");
    return -1;
  }

  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;
  JSAMPROW row_pointer[1];
  int row_stride = width * channels;

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress(&cinfo);
  jpeg_stdio_dest(&cinfo, file);

  cinfo.image_width      = width;
  cinfo.image_height     = height;
  cinfo.input_components = channels;
  cinfo.in_color_space   = (channels == 3) ? JCS_RGB : JCS_GRAYSCALE;

  jpeg_set_defaults(&cinfo);
  jpeg_start_compress(&cinfo, TRUE);

  while (cinfo.next_scanline < cinfo.image_height) 
  {
    row_pointer[0] = & pixels[(height - cinfo.next_scanline - 1) * row_stride];
    jpeg_write_scanlines(&cinfo, row_pointer, 1);
  }

  jpeg_finish_compress(&cinfo);
  jpeg_destroy_compress(&cinfo);

  return 0;
}


#endif // DISABLE_JPEG


/******************************************************************
* .BMP file  manipulation  
*/



int Image::readBMP (const char* filename)
{
  FILE* file = fopen(filename, "rb");

  if (!file)
  {
    perror("Image::readBMP");
    return -1;
  }

  int result = readBMP(file);
  fclose(file);

  return result;
}


int Image::writeBMP (const char* filename)
{
  FILE* file = fopen(filename, "wb");

  if (!file)
  {
    perror("Image::writeBMP");
    return -1;
  }

  int result = writeBMP(file);
  fclose(file);

  if (result == -1)
    unlink(filename);

  return result;
}


#if !defined(WIN32) || defined(_CONSOLE)

typedef unsigned char BYTE;		/* 8 bits */
typedef unsigned short int WORD;	/* 16-bit unsigned integer. */
typedef unsigned int DWORD;		/* 32-bit unsigned integer */
typedef int LONG;			/* 32-bit signed integer */



typedef struct tagBITMAPFILEHEADER {
    WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;
    WORD bfReserved2;
    DWORD bfOffBits;
} BITMAPFILEHEADER;



typedef struct tagBITMAPINFOHEADER {
    DWORD biSize;
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG biXPelsPerMeter;
    LONG biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
} BITMAPINFOHEADER;



/* constants for the biCompression field */
#define BI_RGB        0L
#define BI_RLE8       1L
#define BI_RLE4       2L
#define BI_BITFIELDS  3L



typedef struct tagRGBTRIPLE {
    BYTE rgbtBlue;
    BYTE rgbtGreen;
    BYTE rgbtRed;
} RGBTRIPLE;



typedef struct /*tagRGBQUAD*/ {
    BYTE rgbBlue;
    BYTE rgbGreen;
    BYTE rgbRed;
    BYTE rgbReserved;
} RGBQUAD;


#endif // !defined(WIN32) || defined(_CONSOLE)


/* Some magic numbers */

#define BMP_BF_TYPE 0x4D42
/* word BM */

#define BMP_BF_OFF_BITS 54
/* 14 for file header + 40 for info header (not sizeof(), but packed size) */

#define BMP_BI_SIZE 40
/* packed size of info header */


/* Reads a WORD from a file in little endian format */
static WORD WordReadLE(FILE* fp)
{
    WORD lsb, msb;

    lsb = fgetc(fp);
    msb = fgetc(fp);
    return (msb << 8) | lsb;
}



/* Writes a WORD to a file in little endian format */
static void WordWriteLE(WORD x, FILE* fp)
{
    BYTE lsb, msb;

    lsb = (BYTE) (x & 0x00FF);
    msb = (BYTE) (x >> 8);
    fputc(lsb, fp);
    fputc(msb, fp);
}



/* Reads a DWORD word from a file in little endian format */
static DWORD DWordReadLE(FILE* fp)
{
    DWORD b1, b2, b3, b4;

    b1 = fgetc(fp);
    b2 = fgetc(fp);
    b3 = fgetc(fp);
    b4 = fgetc(fp);
    return (b4 << 24) | (b3 << 16) | (b2 << 8) | b1;
}



/* Writes a DWORD to a file in little endian format */
static void DWordWriteLE(DWORD x, FILE* fp)
{
    unsigned char b1, b2, b3, b4;

    b1 = (x & 0x000000FF);
    b2 = ((x >> 8) & 0x000000FF);
    b3 = ((x >> 16) & 0x000000FF);
    b4 = ((x >> 24) & 0x000000FF);
    fputc(b1, fp);
    fputc(b2, fp);
    fputc(b3, fp);
    fputc(b4, fp);
}



/* Reads a LONG word from a file in little endian format */
static LONG LongReadLE(FILE* fp)
{
    LONG b1, b2, b3, b4;

    b1 = fgetc(fp);
    b2 = fgetc(fp);
    b3 = fgetc(fp);
    b4 = fgetc(fp);
    return (b4 << 24) | (b3 << 16) | (b2 << 8) | b1;
}



/* Writes a LONG to a file in little endian format */
static void LongWriteLE(LONG x, FILE* fp)
{
    char b1, b2, b3, b4;

    b1 = (x & 0x000000FF);
    b2 = ((x >> 8) & 0x000000FF);
    b3 = ((x >> 16) & 0x000000FF);
    b4 = ((x >> 24) & 0x000000FF);
    fputc(b1, fp);
    fputc(b2, fp);
    fputc(b3, fp);
    fputc(b4, fp);
}


int bitcount (DWORD w)
{
   w = (0x55555555 & w) + (0x55555555 & (w>> 1));
   w = (0x33333333 & w) + (0x33333333 & (w>> 2));
   w = (0x0f0f0f0f & w) + (0x0f0f0f0f & (w>> 4));
   w = (0x00ff00ff & w) + (0x00ff00ff & (w>> 8));
   w = (0x0000ffff & w) + (0x0000ffff & (w>>16));
   return w;
}



int Image::readBMP (FILE* fp)
{
    if (pixels) delete[] pixels;

    BITMAPFILEHEADER bmfh;
    BITMAPINFOHEADER bmih;

    /* Read file header */

    /* fread(&bmfh, sizeof(bmfh), 1, fp); */
    /* fread won't work on different platforms because of endian
     * issues.  Sigh... */

    bmfh.bfType = WordReadLE(fp);
    bmfh.bfSize = DWordReadLE(fp);
    bmfh.bfReserved1 = WordReadLE(fp);
    bmfh.bfReserved2 = WordReadLE(fp);
    bmfh.bfOffBits = DWordReadLE(fp);

    /* Debug file header 
    fprintf(stderr, "file header:\n");
    fprintf(stderr, "\tbfType: %x\n", (int) bmfh.bfType);
    fprintf(stderr, "\tbfSize: %d\n", (int) bmfh.bfSize);
    fprintf(stderr, "\tbfReserved1: %d\n", (int) bmfh.bfReserved1);
    fprintf(stderr, "\tbfReserved2: %d\n", (int) bmfh.bfReserved2);
    fprintf(stderr, "\tbfOffBits: %d\n", (int) bmfh.bfOffBits);
*/

    /* Check file header */
    if (bmfh.bfType != BMP_BF_TYPE)
    {
      fprintf( stderr, "Image::readBMP: unrecognized type\n" );
      return -1;
    }

    /* Read info header */

    /* fread(&bmih, sizeof(bmih), 1, fp); */
    /* same problem as above... */

    bmih.biSize = DWordReadLE(fp);
    bmih.biWidth = LongReadLE(fp);
    bmih.biHeight = LongReadLE(fp);
    bmih.biPlanes = WordReadLE(fp);
    bmih.biBitCount = WordReadLE(fp);
    bmih.biCompression = DWordReadLE(fp);
    bmih.biSizeImage = DWordReadLE(fp);
    bmih.biXPelsPerMeter = LongReadLE(fp);
    bmih.biYPelsPerMeter = LongReadLE(fp);
    bmih.biClrUsed = DWordReadLE(fp);
    bmih.biClrImportant = DWordReadLE(fp);

    /* Debug info header
    fprintf(stderr, "info header:\n");
    fprintf(stderr, "\tbiSize: %d\n", (int) bmih.biSize);
    fprintf(stderr, "\tbiWidth: %d\n", (int) bmih.biWidth);
    fprintf(stderr, "\tbiHeight: %d\n", (int) bmih.biHeight);
    fprintf(stderr, "\tbiPlanes: %d\n", (int) bmih.biPlanes);
    fprintf(stderr, "\tbiBitCount: %d\n", (int) bmih.biBitCount);
    fprintf(stderr, "\tbiCompression: %d\n", (int) bmih.biCompression);
    fprintf(stderr, "\tbiSizeImage: %d\n", (int) bmih.biSizeImage);
    fprintf(stderr, "\tbiXPelsPerMeter: %d\n", (int) bmih.biXPelsPerMeter);
    fprintf(stderr, "\tbiYPelsPerMeter: %d\n", (int) bmih.biYPelsPerMeter);
    fprintf(stderr, "\tbiClrUsed: %d\n", (int) bmih.biClrUsed);
    fprintf(stderr, "\tbiClrImportant: %d\n", (int) bmih.biClrImportant);
*/

    /* Check info header 
    if (bmih.biSize != BMP_BI_SIZE)
      fprintf( stderr, "houston we may have a problem\n" );
*/

    if ((bmih.biWidth <= 0)  ||
        (bmih.biHeight <= 0) ||
        (bmih.biPlanes != 1))
    {
      fprintf(stderr, "Image::readBMP: malformed file\n");
      return -1;
    }

    /* Creates the image */
    width    = bmih.biWidth;
    height   = bmih.biHeight;
    channels = 3;
    bits     = 8;
    maxValue = 255;
    pixels   = new unsigned char[width*height*channels];
    memset(pixels, 0, width*height*channels);

    int scanlinelength;
    if ((width * bmih.biBitCount) % 32 == 0)
      scanlinelength = width * bmih.biBitCount / 8;
    else
      scanlinelength = (width * bmih.biBitCount / 32 + 1) * 4;

    RGBQUAD    *palette;
    BYTE       *scanlineByte;
    WORD       *scanlineWord;
    DWORD      *scanlineDword;
    RGBTRIPLE  *scanline24;
    RGBQUAD    *scanline32;
    int         index;
    Pixel       pixel;
    WORD	red, green, blue;
    DWORD       bluemask, greenmask, redmask;
    int         bluewidth, greenwidth;
    bool        done;

    // read all the color info / data
    switch (bmih.biBitCount)
    {

    case 1:	// 1 bit - monochrome, index mode

      // read the palette

      palette = new RGBQUAD[2];
      memset(palette, 0, 2 * sizeof(RGBQUAD));
      fread((void*) palette, sizeof(RGBQUAD), 2, fp);

      // detect a grayscale palette, and modify image accordingly

      done = false;
      for (int i = 0; !done && i < 2; ++i)
      {
        done = done || (palette[i].rgbRed  != palette[i].rgbGreen)
                    || (palette[i].rgbBlue != palette[i].rgbGreen);
      }

      if (!done)
      {
        delete[] pixels;
        channels = 1;
        pixels = new unsigned char[width*height*channels];
        memset(pixels, 0, width*height*channels);
      }

      // read the data line by line

      scanlineByte = new BYTE[scanlinelength];
      fseek(fp, bmfh.bfOffBits, SEEK_SET);

      for (int y = 0; y < height; ++y) 
      {
        fread((void*) scanlineByte, scanlinelength, 1, fp);
        if (ferror(fp))
        {
          perror("Image::readBMP");
          delete[] palette;
          delete[] pixels;
          pixels = NULL;
          return -1;
        }

        for (int x = 0; x < width; ++x)
        {
          index = (scanlineByte[x/8] >> (7 - (x % 8))) & 0x01;

          pixel.r = palette[index].rgbRed   / 255.0;
          pixel.g = palette[index].rgbGreen / 255.0;
          pixel.b = palette[index].rgbBlue  / 255.0;
          setPixel(x, height - 1 - y, pixel);
        }
      }

      delete[] scanlineByte;
      delete[] palette;

      break;


    case 4:	// 4 bit - 16 color, index mode 

      // read the palette

      palette = new RGBQUAD[16];
      memset(palette, 0, 16 * sizeof(RGBQUAD));
      fread((void*) palette, sizeof(RGBQUAD), 16, fp);

      // detect a grayscale palette, and modify image accordingly

      done = false;
      for (int i = 0; !done && i < 16; ++i)
      {
        done = done || (palette[i].rgbRed  != palette[i].rgbGreen)
                    || (palette[i].rgbBlue != palette[i].rgbGreen);
      }

      if (!done)
      {
        delete[] pixels;
        channels = 1;
        pixels = new unsigned char[width*height*channels];
        memset(pixels, 0, width*height*channels);
      }

      // read the data line by line

      if (bmih.biCompression == BI_RGB)		// uncompressed data
      {
        scanlineByte = new BYTE[scanlinelength];
        fseek(fp, bmfh.bfOffBits, SEEK_SET);

        for (int y = 0; y < height; ++y) 
        {
          fread((void*) scanlineByte, scanlinelength, 1, fp);
          if (ferror(fp))
          {
            perror("Image::readBMP");
            delete[] palette;
            delete[] pixels;
            pixels = NULL;
            return -1;
          }

          for (int x = 0; x < width; ++x)
          {
            if (x % 2 == 0)
              index = (scanlineByte[x/2] >> 4) & 0x0F;
            else
              index = scanlineByte[x/2] & 0x0F;

            pixel.r = palette[index].rgbRed   / 255.0;
            pixel.g = palette[index].rgbGreen / 255.0;
            pixel.b = palette[index].rgbBlue  / 255.0;
            setPixel(x, height - 1 - y, pixel);
          }
        }

        delete[] scanlineByte;
      }
      else if (bmih.biCompression == BI_RLE4)	// 4-bit RLE compression
      {
        unsigned char rleCode[2];
        int curx = 0;
        int cury = 0;
        done = false;

        fseek(fp, bmfh.bfOffBits, SEEK_SET);
        while (!done && fp)
        {
          fread((void*) rleCode, sizeof(char), 2, fp);
          if (ferror(fp))
            done = true;

          if (rleCode[0] == 0 && rleCode[1] < 3)	// escape
          {
            if (rleCode[1] == 0)
            {
              curx = 0;
              ++cury;
              if (cury >= height)
                done = true;
            }
            else if (rleCode[1] == 1)
            {
              done = true;
            }
            else
            {
              curx += fgetc(fp);
              cury += fgetc(fp);
            }
          }
          else if (rleCode[0] == 0)			// absolute mode
          {
            BYTE byte;

            for (int i = 0; i < (rleCode[1] + 1) / 2; ++i)
            {
              byte = fgetc(fp);

              index = (byte >> 4) & 0x0F;

              pixel.r = palette[index].rgbRed   / 255.0;
              pixel.g = palette[index].rgbGreen / 255.0;
              pixel.b = palette[index].rgbBlue  / 255.0;
              setPixel(curx, height - 1 - cury, pixel);

              ++curx;

              index = byte & 0x0F;

              pixel.r = palette[index].rgbRed   / 255.0;
              pixel.g = palette[index].rgbGreen / 255.0;
              pixel.b = palette[index].rgbBlue  / 255.0;
              setPixel(curx, height - 1 - cury, pixel);

              ++curx;
            }

            if (((rleCode[1] + 1) / 2) % 2 != 0)
              fgetc(fp);
          }
          else						// encoded mode
          {
            for (int i = 0; i < rleCode[0]; ++i)
            {
              if (i % 2 == 0)
                index = (rleCode[1] >> 4) & 0x0F;
              else
                index = rleCode[1] & 0x0F;

              pixel.r = palette[index].rgbRed   / 255.0;
              pixel.g = palette[index].rgbGreen / 255.0;
              pixel.b = palette[index].rgbBlue  / 255.0;
              setPixel(curx, height - 1 - cury, pixel);

              ++curx;
            }
          }
        }

        if (ferror(fp))
        {
          perror("Image::readBMP");
          delete[] palette;
          delete[] pixels;
          pixels = NULL;
          return -1;
        }
      }

      delete[] palette;

      break;


    case 8:	// 8 bit - 256 color, index mode

      // read the palette 

      palette = new RGBQUAD[256];
      memset(palette, 0, 256 * sizeof(RGBQUAD));
      fread((void*) palette, sizeof(RGBQUAD), 256, fp);

      // detect a grayscale palette, and modify image accordingly

      done = false;
      for (int i = 0; !done && i < 256; ++i)
      {
        done = done || (palette[i].rgbRed  != palette[i].rgbGreen)
                    || (palette[i].rgbBlue != palette[i].rgbGreen);
      }

      if (!done)
      {
        delete[] pixels;
        channels = 1;
        pixels = new unsigned char[width*height*channels];
        memset(pixels, 0, width*height*channels);
      }

      // read the data line by line

      if (bmih.biCompression == BI_RGB)		// uncompressed data
      {
        scanlineByte = new BYTE[scanlinelength];
        fseek(fp, bmfh.bfOffBits, SEEK_SET);

        for (int y = 0; y < height; ++y) 
        {
          fread((void*) scanlineByte, scanlinelength, 1, fp);
          if (ferror(fp))
          {
            perror("Image::readBMP");
            delete[] palette;
            delete[] pixels;
            pixels = NULL;
            return -1;
          }

          for (int x = 0; x < width; ++x)
          {
            index = scanlineByte[x];

            pixel.r = palette[index].rgbRed   / 255.0;
            pixel.g = palette[index].rgbGreen / 255.0;
            pixel.b = palette[index].rgbBlue  / 255.0;
            setPixel(x, height - 1 - y, pixel);
          }
        }
        delete[] scanlineByte;
      }
      else if (bmih.biCompression == BI_RLE8)	// 8-bit RLE compression
      {
        unsigned char rleCode[2];
        int curx = 0;
        int cury = 0;
        done = false;

        fseek(fp, bmfh.bfOffBits, SEEK_SET);
        while (!done && fp)
        {
          fread((void*) rleCode, sizeof(char), 2, fp);
          if (ferror(fp))
            done = true;

          if (rleCode[0] == 0 && rleCode[1] < 3)	// escape
          {
            if (rleCode[1] == 0)
            {
              curx = 0;
              ++cury;
              if (cury >= height)
                done = true;
            }
            else if (rleCode[1] == 1)
            {
              done = true;
            }
            else
            {
              curx += fgetc(fp);
              cury += fgetc(fp);
            }
          }
          else if (rleCode[0] == 0)			// absolute mode
          {
            for (int i = 0; i < rleCode[1]; ++i)
            {
              index = fgetc(fp);

              pixel.r = palette[index].rgbRed   / 255.0;
              pixel.g = palette[index].rgbGreen / 255.0;
              pixel.b = palette[index].rgbBlue  / 255.0;
              setPixel(curx, height - 1 - cury, pixel);

              ++curx;
            }

            if (rleCode[1] % 2 != 0)
              fgetc(fp);
          }
          else						// encoded mode
          {
            pixel.r = palette[rleCode[1]].rgbRed   / 255.0;
            pixel.g = palette[rleCode[1]].rgbGreen / 255.0;
            pixel.b = palette[rleCode[1]].rgbBlue  / 255.0;

            for (int i = 0; i < rleCode[0]; ++i)
            {
              setPixel(curx, height - 1 - cury, pixel);

              ++curx;
            }
          }
        }

        if (ferror(fp))
        {
          perror("Image::readBMP");
          delete[] palette;
          delete[] pixels;
          pixels = NULL;
          return -1;
        }
      }

      delete[] palette;

      break;


    case 16:	// 16 bit - 2^16 color, rgb mode 

      // set the color masks and shifts

      if (bmih.biCompression == BI_BITFIELDS)	// user specified
      {
        redmask    = DWordReadLE(fp);
        greenmask  = DWordReadLE(fp);
        bluemask   = DWordReadLE(fp);
        bluewidth  = bitcount(bluemask);
        greenwidth = bitcount(greenmask);
      }
      else // bmih.biCompression == BI_RGB	// using default values
      {
        bluemask   = 0x001F;
        bluewidth  = 5;
        greenmask  = 0x03E0;
        greenwidth = 5;
        redmask    = 0x7C00;
      }

      // get data line by line

      scanlineWord = new WORD[(scanlinelength+1)/2];
      fseek(fp, bmfh.bfOffBits, SEEK_SET);

      for (int y = 0; y < height; ++y) 
      {
//        fp.read((char*) scanlineWord, scanlinelength);
        fread((void*) scanlineWord, scanlinelength, 1, fp);
        if (ferror(fp))
        {
          perror("Image::readBMP");
          delete[] pixels;
          pixels = NULL;
          return -1;
        }

        for (int x = 0; x < width; ++x)
        {
          red   = (scanlineWord[x] & redmask)
                            >> (bluewidth + greenwidth);
          green = (scanlineWord[x] & greenmask) >> bluewidth;
          blue  = (scanlineWord[x] & bluemask);

          pixel.r = red   / 255.0;
          pixel.g = green / 255.0;
          pixel.b = blue  / 255.0;
          setPixel(x, height - 1 - y, pixel);
        }
      }

      delete[] scanlineWord;

      break;


    case 24:	// 24 bit - 2^24 color, rgb mode 

      // read the data line by line

      scanline24 = new RGBTRIPLE[(scanlinelength+2)/3];
      fseek(fp, bmfh.bfOffBits, SEEK_SET);

      for (int y = 0; y < height; ++y) 
      {
        fread((void*) scanline24, scanlinelength, 1, fp);
        if (ferror(fp))
        {
          perror("Image::readBMP");
          delete[] pixels;
          pixels = NULL;
          return -1;
        }

        for (int x = 0; x < width; ++x)
        {
          pixel.r = scanline24[x].rgbtRed   / 255.0;
          pixel.g = scanline24[x].rgbtGreen / 255.0;
          pixel.b = scanline24[x].rgbtBlue  / 255.0;
          setPixel(x, height - 1 - y, pixel);
		}
      }

      delete[] scanline24;

      break;


    case 32:	// 32 bit - 2^32 color, rgb mode

      // read data line by line

      if (bmih.biCompression == BI_RGB)		// default encoding
      {
        scanline32 = new RGBQUAD[(scanlinelength+3)/4];
        fseek(fp, bmfh.bfOffBits, SEEK_SET);

        for (int y = 0; y < height; ++y) 
        {
          fread((void*) scanline32, scanlinelength, 1, fp);
          if (ferror(fp))
          {
            perror("Image::readBMP");
            delete[] pixels;
            pixels = NULL;
            return -1;
          }

          for (int x = 0; x < width; ++x)
          {
            pixel.r = scanline32[x].rgbRed   / 255.0;
            pixel.g = scanline32[x].rgbGreen / 255.0;
            pixel.b = scanline32[x].rgbBlue  / 255.0;
            setPixel(x, height - 1 - y, pixel);
          }
        }

        delete[] scanline32;
      }
      else if (bmih.biCompression == BI_BITFIELDS)	// user specified
      {
        // get masks and shifts
        redmask    = DWordReadLE(fp);
        greenmask  = DWordReadLE(fp);
        bluemask   = DWordReadLE(fp);
        bluewidth  = bitcount(bluemask);
        greenwidth = bitcount(greenmask);

        scanlineDword = new DWORD[(scanlinelength+3)/4];
        fseek(fp, bmfh.bfOffBits, SEEK_SET);

        for (int y = 0; y < height; ++y) 
        {
          fread((void*) scanlineDword, scanlinelength, 1, fp);
          if (ferror(fp))
          {
            perror("Image::readBMP");
            delete[] pixels;
            pixels = NULL;
            return -1;
          }

          for (int x = 0; x < width; ++x)
          {
            red   = (scanlineDword[x] & redmask)
                              >> (bluewidth + greenwidth);
            green = (scanlineDword[x] & greenmask) >> bluewidth;
            blue  = (scanlineDword[x] & bluemask);

            pixel.r = red   / 255.0;
            pixel.g = green / 255.0;
            pixel.b = blue  / 255.0;
            setPixel(x, height - 1 - y, pixel);
          }
        }

        delete[] scanlineDword;
      }

      break;

    }

    return 0;
}



int Image::writeBMP (FILE* fp)
{
    if (!good())
    {
      fprintf(stderr, "Image::writeBMP: bad image\n");
      return -1;
    }

    BITMAPFILEHEADER bmfh;
    BITMAPINFOHEADER bmih;
    int lineLength;

    if (channels == 1)
      lineLength = width;
    else
      lineLength = width * 3;

    if ((lineLength % 4) != 0) 
      lineLength = (lineLength / 4 + 1) * 4;

    /* Write file header */

    bmfh.bfType = BMP_BF_TYPE;
    bmfh.bfSize = BMP_BF_OFF_BITS + lineLength * height;
    bmfh.bfReserved1 = 0;
    bmfh.bfReserved2 = 0;
    bmfh.bfOffBits = BMP_BF_OFF_BITS;

    if (channels == 1)
      bmfh.bfOffBits += 256 * 4;

    WordWriteLE(bmfh.bfType, fp);
    DWordWriteLE(bmfh.bfSize, fp);
    WordWriteLE(bmfh.bfReserved1, fp);
    WordWriteLE(bmfh.bfReserved2, fp);
    DWordWriteLE(bmfh.bfOffBits, fp);

    /* Write info header */

    bmih.biSize = BMP_BI_SIZE;
    bmih.biWidth = width;
    bmih.biHeight = height;
    bmih.biPlanes = 1;
    bmih.biBitCount = (channels == 1) ? 8 : 24;
    bmih.biCompression = BI_RGB;
    bmih.biSizeImage = lineLength * (DWORD) bmih.biHeight;
    bmih.biXPelsPerMeter = 2925;
    bmih.biYPelsPerMeter = 2925;
    bmih.biClrUsed = (channels == 1) ? 256 : 0;
    bmih.biClrImportant = 0;

    DWordWriteLE(bmih.biSize, fp);
    LongWriteLE(bmih.biWidth, fp);
    LongWriteLE(bmih.biHeight, fp);
    WordWriteLE(bmih.biPlanes, fp);
    WordWriteLE(bmih.biBitCount, fp);
    DWordWriteLE(bmih.biCompression, fp);
    DWordWriteLE(bmih.biSizeImage, fp);
    LongWriteLE(bmih.biXPelsPerMeter, fp);
    LongWriteLE(bmih.biYPelsPerMeter, fp);
    DWordWriteLE(bmih.biClrUsed, fp);
    DWordWriteLE(bmih.biClrImportant, fp);

    /* Write pixels */

    Pixel pixel;

    if (channels == 1)
    {
      // write 8-bit grayscale palette

      unsigned char palettecolor[4];
      for (int i = 0; i < 256; ++i)
      {
        memset(palettecolor, (unsigned char) i, 4);
        fwrite((void*) palettecolor, sizeof(char), 4, fp);
      }

      // write image data

      for (int y = 0; y < height; ++y) 
      {
        int nbytes = 0;
        for (int x = 0; x < width; ++x) 
        {
          getPixel(x, height - y - 1, pixel);
          fputc((int) (pixel.r * 255), fp);
          nbytes++;
        }

        while ((nbytes % 4) != 0) 
        {
          fputc(0, fp);  nbytes++;
        }
      }
    }
    else
    {
      for (int y = 0; y < height; ++y) 
      {
        int nbytes = 0;
        for (int x = 0; x < width; ++x) 
        {
          getPixel(x, height - y - 1, pixel);
          fputc((int) (pixel.b * 255), fp);  nbytes++;
          fputc((int) (pixel.g * 255), fp);  nbytes++;
          fputc((int) (pixel.r * 255), fp);  nbytes++;
        }

        while ((nbytes % 4) != 0) 
        {
          fputc(0, fp);
          nbytes++;
        }
      }
    }

    if (ferror(fp))
    {
      perror("Image::writeBMP");
      return -1;
    }

    return 0;
}



/******************************************************************
* .PNM file  manipulation  
*/


#define PNM_ASCII	0
#define PNM_BINARY	1
#define PNM_PBM		10
#define PNM_PGM		11
#define PNM_PPM		12


int Image::readPNM (const char* filename)
{
  FILE* file = fopen(filename, "rb");

  if (!file)
  {
    perror("Image::readPNM");
    return -1;
  }

  int result = readPNM(file);
  fclose(file);

  return result;
}


int Image::writePNM (const char* filename)
{
  FILE* file = fopen(filename, "wb");

  if (!file)
  {
    perror("Image::writePNM");
    return -1;
  }

  int result = writePNM(file);
  fclose(file);

  if (result == -1)
    unlink(filename);

  return result;
}


static inline void getWord (FILE* fp, char* s)
{
  fscanf(fp, "%1023s", s);
}


static inline void getLine (FILE* fp, char* s)
{
  fgets(s, 1024, fp);
}


static inline void getWordSkipComments (FILE* fp, char* s)
{
  getWord(fp, s);
  while (s[0] == '#' && !ferror(fp))
  {
    getLine(fp, s);
    getWord(fp, s);
  }
}


int Image::readPNM (FILE* fp)
{
  if (pixels) 
    delete[] pixels;
  pixels = NULL;

  char nextLine[1024];

  getWord(fp, nextLine);

  int mode, type;

  if (nextLine[0] == 'P')
  {
    switch (nextLine[1])
    {
    case '1':
      mode = PNM_ASCII;
      type = PNM_PBM;
      channels = 1;
      break;

    case '2':
      mode = PNM_ASCII;
      type = PNM_PGM;
      channels = 1;
      break;

    case '3':
      mode = PNM_ASCII;
      type = PNM_PPM;
      channels = 3;
      break;

    case '5':
      mode = PNM_BINARY;
      type = PNM_PGM;
      channels = 1;
      break;

    case '6':
      mode = PNM_BINARY;
      type = PNM_PPM;
      channels = 3;
      break;

    default:
      fprintf( stderr, "Image::readPNM: malformed file\n" );
      return -1;
    }
  }
  else
  {
    fprintf( stderr, "Image::readPNM: malformed file\n" );
    return -1;
  }

  getWordSkipComments(fp, nextLine);
  width = atoi(nextLine);

  getWordSkipComments(fp, nextLine);
  height = atoi(nextLine);

  if (type != PNM_PBM)
  {
    getWordSkipComments(fp, nextLine);
    maxValue = atoi(nextLine);
  }
  else
  {
    maxValue = 1;
  }

  if (ferror(fp))
  {
    perror("Image::readPNM");
    return -1;
  }

  if (width <= 0 || height <= 0 || maxValue <= 0)
  {
    fprintf( stderr, "Image::readPNM: malformed file\n" );
    return -1;
  }

  bits   = (int) ceil(log10(maxValue + 1.0) / log10(2.0));
  pixels = new unsigned char[width*height*channels];
  memset(pixels, 0, width*height*channels);

  int        red, blue, green, intensity;
  Pixel      pixel;
  BYTE      *scanlineByte;
  RGBTRIPLE *scanline24;

  if (mode == PNM_ASCII)
  {
    if (type == PNM_PBM || type == PNM_PGM)
    {
      for (int y = 0; y < height; ++y)
      {
        for (int x = 0; x < width; ++x)
        {
          fscanf(fp, "%d", &intensity);
          if (ferror(fp))
          {
            perror("Image::readPNM");
            delete[] pixels;
            pixels = NULL;
            return -1;
          }

          pixel.r = intensity / (float) maxValue;
          setPixel(x,y,pixel);        
        }
      }
    }
    else if (type == PNM_PPM)
    {
      for (int y = 0; y < height; ++y)
      {
        for (int x = 0; x < width; ++x)
        {
          fscanf(fp, "%d %d %d", &red, &green, &blue);
          if (ferror(fp))
          {
            perror("Image::readPNM");
            delete[] pixels;
            pixels = NULL;
            return -1;
          }

          pixel.r = red   / (float) maxValue;
          pixel.g = green / (float) maxValue;
          pixel.b = blue  / (float) maxValue;
          setPixel(x,y,pixel);        
        }
      }
    }
  }
  else if (mode == PNM_BINARY)
  {
    // move buffer up to the data's start
    fgetc(fp);

    if (type == PNM_PGM)
    {
      scanlineByte = new BYTE[width];

      for (int y = 0; y < height; ++y)
      {
        fread((void*) scanlineByte, sizeof(BYTE), width, fp);
        if (ferror(fp))
        {
          perror("Image::readPNM");
          delete[] scanlineByte;
          delete[] pixels;
          pixels = NULL;
          return -1;
        }

        for (int x = 0; x < width; ++x)
        {
          pixel.r = scanlineByte[x] / (float) maxValue;
          setPixel(x,y,pixel);
        }
      }

      delete[] scanlineByte;
    }
    else if (type == PNM_PPM)
    {
      scanline24 = new RGBTRIPLE[width];

      for (int y = 0; y < height; ++y)
      {
        fread((void*) scanline24, sizeof(RGBTRIPLE), width, fp);
        if (ferror(fp))
        {
          perror("Image::readPNM");
          delete[] scanline24;
          delete[] pixels;
          pixels = NULL;
          return -1;
        }

        for (int x = 0; x < width; ++x)
        {
          pixel.b = scanline24[x].rgbtRed   / (float) maxValue;
          pixel.g = scanline24[x].rgbtGreen / (float) maxValue;
          pixel.r = scanline24[x].rgbtBlue  / (float) maxValue;

          setPixel(x,y,pixel);
        }
      }

      delete[] scanline24;
    }
  }

  return 0;
}


int Image::writePNM (FILE* fp)
{
  if (!good())
  {
    fprintf(stderr, "Image::writePNM: bad image\n");
    return -1;
  }

  if (channels == 1)
  {
    if (bits == 1)
    {
      fprintf(fp, "P1\n");
      fprintf(fp, "%d %d\n", width, height);

      for (int j = 0; j < height; ++j)
      {
        for (int i = 0; i < width; ++i)
        {
          fprintf(fp, "%d ", pixels[index(i,j,0)] * maxValue / 255);
        }
        fprintf(fp, "\n");
      }
    }
    else
    {
      fprintf(fp, "P2\n");
      fprintf(fp, "%d %d %d\n", width, height, maxValue);

      for (int j = 0; j < height; ++j)
      {
        for (int i = 0; i < width; ++i)
        {
          fprintf(fp, "%d ", pixels[index(i,j,0)] * maxValue / 255);
        }
        fprintf(fp, "\n");
      }
    }
  }
  else
  {
    fprintf(fp, "P3\n");
    fprintf(fp, "%d %d %d\n", width, height, maxValue);

    for (int j = 0; j < height; ++j)
    {
      for (int i = 0; i < width; ++i)
      {
        fprintf(fp, "%d %d %d ",
                pixels[index(i,j,0)] * maxValue / 255,
                pixels[index(i,j,1)] * maxValue / 255,
                pixels[index(i,j,2)] * maxValue / 255);
      }
      fprintf(fp, "\n");
    }
  }

  return 0;
}
