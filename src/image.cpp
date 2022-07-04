#include "image.h"

unsigned char Pixel::R() const
{
    return raw_[0];
}
unsigned char Pixel::G() const
{
    return raw_[1];
}
unsigned char Pixel::B() const
{
    return raw_[2];
}
unsigned char Pixel::A() const
{
    return raw_[3];
}

void Pixel::setR(unsigned char R)
{
    raw_[0] = R;
}
void Pixel::setG(unsigned char G)
{
    raw_[1] = G;
}
void Pixel::setB(unsigned char B)
{
    raw_[2] = B;
}
void Pixel::setA(unsigned char A)
{
    raw_[3] = A;
}

Pixel::Pixel(unsigned char* raw) : raw_(raw)
{
}

PixelRGB Pixel::RGB()
{
    return PixelRGB(*this);
}
void Pixel::setRGB(const PixelRGB& rgb)
{
    this->setR(rgb.R * 255);
    this->setG(rgb.G * 255);
    this->setB(rgb.B * 255);
}

PixelRGB::PixelRGB(const Pixel& pixel) : PixelRGB()
{
    this->fromPixel(pixel);
}
void PixelRGB::fromPixel(const Pixel& pixel)
{
    R = (double)pixel.R() / 255;
    G = (double)pixel.G() / 255;
    B = (double)pixel.B() / 255;
}

PixelHSV::PixelHSV(const PixelRGB& rgb) : PixelHSV()
{
    this->fromRGB(rgb);
}

bool PixelHSV::fromRGB(const PixelRGB& rgb)
{
    // RGB -> HSV
    double max = std::max({rgb.R, rgb.G, rgb.B});
    double min = std::min({rgb.R, rgb.G, rgb.B});
    if(max == min) return false;

    // H setup
    if(max == rgb.R && rgb.G >= rgb.B) H = 60 * ((rgb.G - rgb.B) / (max - min));
    if(max == rgb.R && rgb.G < rgb.B) H = 60 * ((rgb.G - rgb.B) / (max - min)) + 360;
    if(max == rgb.G) H = 60 * ((rgb.B - rgb.R) / (max - min)) + 120;
    if(max == rgb.B) H = 60 * ((rgb.R - rgb.G) / (max - min)) + 240;

    // S setup
    if(max == 0) S = 0;
    else S = 1 - (min / max);

    // V setup
    V = max;
    
    return true;
}
PixelRGB PixelHSV::RGB()
{
    // HSV -> RGB
    PixelRGB rgb;

    short hi = (H / 60) % 6;
    double vmin = (100 - S * 100) * V;
    double a = (V * 100 - vmin) * (H % 60) / 60;
    double vinc = vmin + a;
    double vdec = V * 100 - a;

    switch (hi)
    {
    case 0:
        rgb.R = V * 100;
        rgb.G = vinc;
        rgb.B = vmin;
        break;
    case 1:
        rgb.R = vdec;
        rgb.G = V * 100;
        rgb.B = vmin;
        break;
    case 2:
        rgb.R = vmin;
        rgb.G = V * 100;
        rgb.B = vinc;    
        break;
    case 3:
        rgb.R = vmin;
        rgb.G = vdec;
        rgb.B = V * 100;   
        break;
    case 4:
        rgb.R = vinc;
        rgb.G = vmin;
        rgb.B = V * 100;
        break;
    case 5:
        rgb.R = V * 100;
        rgb.G = vmin;
        rgb.B = vdec;
        break;
    default:
        break;
    }

    rgb.R /= 100;
    rgb.G /= 100;
    rgb.B /= 100;
    return rgb;
}

Image::Image(const Image& other) : pixels_(other.pixels_), height_(other.height_), width_(other.width_)
{
}

Image::Image(unsigned char* raw, int height, int width) :  height_(height), width_(width)
{
    // выделяем память под указатели, но не под саму картинку
    pixels_ = new Pixel*[height];
    for(int i = 0; i < height; i++) pixels_[i] = new Pixel[width];

    for(int i = 0; i < height; i++)
        for(int j = 0; j < width; j++)
        {
            // указатели ссылаются на нашу картинку
            int offset = i * width * 4;
            pixels_[i][j] = Pixel(&raw[offset + j * 4]);
        }
}
Image::~Image()
{
    for(int i = 0; i < height_; i++) delete[] pixels_[i];
    delete[] pixels_;
}
Pixel& Image::at(int i, int j)
{
    return pixels_[i][j];
}
int Image::height() const
{
    return height_;
}
int Image::width() const
{
    return width_;
}