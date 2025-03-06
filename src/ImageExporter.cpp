#include "ImageExporter.h"

void ImageExporter::image_export(const string name, const string extension, ImageEditor& imageEditor) const {
    // Supposons que l'image source est accessible via imageEditor
    ofImage* modifiedImage = imageEditor.getImage();

    // Cr�er une nouvelle image pour la sauvegarde
    ofImage exportImage;

    // Obtenir les dimensions de la fen�tre
    int width = ofGetWindowWidth();
    int height = ofGetWindowHeight();

    // Allouer l'image export�e avec les dimensions de la fen�tre
    exportImage.allocate(width, height, OF_IMAGE_COLOR);  // Assurez-vous d'utiliser le bon format

    // Capturer l'image du framebuffer de la fen�tre
    exportImage.grabScreen(0, 0, width, height);  // Prendre une capture d'�cran de la fen�tre enti�re

    // G�n�rer un nom de fichier unique avec extension
    string file_name = name + "." + extension;

    // Ouvrir une bo�te de dialogue pour permettre � l'utilisateur de choisir o� sauvegarder l'image
    ofFileDialogResult result = ofSystemSaveDialog(file_name, "Save your image");

    // Si l'utilisateur a s�lectionn� un emplacement
    if (result.bSuccess) {
        // Sauvegarder l'image export�e � l'emplacement s�lectionn�
        exportImage.save(result.getPath());
    }
}