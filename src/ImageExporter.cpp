#include "ImageExporter.h"

void ImageExporter::image_export(const string name, const string extension, ImageEditor& imageEditor) const {

    ofImage* modifiedImage = imageEditor.getImage();


    ofImage exportImage;


    int width = ofGetWindowWidth();
    int height = ofGetWindowHeight();


    exportImage.allocate(width, height, OF_IMAGE_COLOR);


    exportImage.grabScreen(0, 0, width, height);


    string file_name = name + "." + extension;


    ofFileDialogResult result = ofSystemSaveDialog(file_name, "Save your image");


    if (result.bSuccess) {

        exportImage.save(result.getPath());
    }
}