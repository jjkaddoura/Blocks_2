class Image;

#ifndef IMAGE_H
#define IMAGE_H

#include <stdio.h>

//no support for jpeg in win32!
#ifdef WIN32
#define DISABLE_JPEG
#endif


#define RED	0
#define GREEN	1
#define BLUE	2
#define ALPHA	3


typedef struct
{
	double		r;
	double		g;
	double		b;
	double		a;
} Pixel;


class Image
{
  public:
			Image ();
			~Image ();

			Image (int width_, int height_);
			Image (int width_, int height_, int channels_);
			Image (int width_, int height_, int channels_, 
				int bits_);

			Image (const char* filename);

			// copy constructor and assignment operator
			// _deep_ copy!
			Image (const Image& image);
	Image&		operator= (const Image& image);

			// accessors
	int		getWidth ()    { return width;    }
	int		getHeight ()   { return height;   }
	int		getChannels () { return channels; }
	int		getBits ()     { return bits;     }
	unsigned char*	getPixels ()   { return pixels;   }

	bool		good ();
	bool		bad ();

			// set all the pixel data to zero
	void		clear ();

	double		getPixel  (int x, int y, int channel);
	double		getPixel_ (int x, int y, int channel);
	Pixel		getPixel  (int x, int y);
	Pixel		getPixel_ (int x, int y);
	Pixel&		getPixel  (int x, int y, Pixel& pixel);
	Pixel&		getPixel_ (int x, int y, Pixel& pixel);

	void		setPixel  (int x, int y, int channel, double value);
	void		setPixel_ (int x, int y, int channel, double value);
	void		setPixel  (int x, int y, Pixel& pixel);
	void		setPixel_ (int x, int y, Pixel& pixel);

#ifndef DISABLE_OPENGL
	void		glReadPixelsWrapper ();
	void		glDrawPixelsWrapper ();
	void		glTexImage2DWrapper ();
	void		glTexSubImage2DWrapper ( int x, int y);
#endif

	int		read (const char* filename);
	int		write (const char* filename);

	int		readBMP (const char* filename);
	int		readBMP (FILE* file);
	int		writeBMP (const char* filename);
	int		writeBMP (FILE* file);

	int		readPNM (const char* filename);
	int		readPNM (FILE* file);
	int		writePNM (const char* filename);
	int		writePNM (FILE* file);

#ifndef DISABLE_JPEG
	int		readJPG (const char* filename);
	int		readJPG (FILE* file);
	int		writeJPG (const char* filename);
	int		writeJPG (FILE* file);
#endif

  private:

	int		index(int x, int y, int c);

	int		width;
	int		height;
	int		channels;	// number of channels per pixel
	int		bits;		// number of bits per channel
	int		maxValue;	// max that can be stored in bits

	unsigned char*	pixels;		// image data

};


#endif // IMAGE_H
