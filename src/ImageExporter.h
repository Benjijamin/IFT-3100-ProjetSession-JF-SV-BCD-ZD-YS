#pragma once
#include "ofMain.h"
#include "ImageEditor.h"

class ImageExporter {
public:

    void image_export(const string name, const string extension, ImageEditor& imageEditor) const;

};