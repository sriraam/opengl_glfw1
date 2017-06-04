#include "texture.h"



texture::texture() {

}

void texture::loadtexture(std::string file)
{
	ILboolean success;
	filename = file;
	ilInit();
	//std::cout << "IMAGE_ID" << imageID<<"\n";
	// generate an image name
	ilGenImages(1, &this->imageID);
	// bind it
	//std::cout << "IMAGE_ID" << this->imageID;
	ilBindImage(imageID);
	// match image origin to OpenGL�s
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
	// load  the image
	success = ilLoadImage((ILstring)filename.c_str());
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	// check to see if everything went OK
	if (!success) {
		ilDeleteImages(1, &imageID);
		std::cout << "Image not loaded";
		//return 0;
	}
	else {
		std::cout << "Image Loaded Successfully !!!";
	}

	ilBindImage(imageID);
	this->Width = ilGetInteger(IL_IMAGE_WIDTH);
	this->Height = ilGetInteger(IL_IMAGE_HEIGHT);
	std::cout << "Width" << Width << "\n";
	this->Data = ilGetData();
}



texture::~texture()
{
}