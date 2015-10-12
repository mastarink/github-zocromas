#include <turbojpeg.h>

const int JPEG_QUALITY = 75;
const int COLOR_COMPONENTS = 3;
int _width = 1920;
int _height = 1080;
long unsigned int _jpegSize = 0;
unsigned char* _compressedImage = NULL; //!< Memory is allocated by tjCompress2 if _jpegSize == 0
unsigned char buffer[_width*_height*COLOR_COMPONENTS]; //!< Contains the uncompressed image

tjhandle _jpegCompressor = tjInitCompress();

tjCompress2(_jpegCompressor, buffer, _width, 0, _height, TJPF_RGB,
          &_compressedImage, &_jpegSize, TJSAMP_444, JPEG_QUALITY,
          TJFLAG_FASTDCT);

tjDestroy(_jpegCompressor);

//to free the memory allocated by TurboJPEG (either by tjAlloc(), 
//or by the Compress/Decompress) after you are done working on it:
tjFree(&_compressedImage);
