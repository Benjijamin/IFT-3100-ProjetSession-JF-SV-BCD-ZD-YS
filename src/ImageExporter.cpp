#include "ImageExporter.h"

void ImageExporter::image_export(const string name, const string extension, ImageEditor& imageEditor) const {
    // Supposons que l'image source est accessible via imageEditor
    ofImage* modifiedImage = imageEditor.getImage();

    // Créer une nouvelle image pour la sauvegarde
    ofImage exportImage;

    // Obtenir les dimensions de la fenêtre
    int width = ofGetWindowWidth();
    int height = ofGetWindowHeight();

    // Allouer l'image exportée avec les dimensions de la fenêtre
    exportImage.allocate(width, height, OF_IMAGE_COLOR);  // Assurez-vous d'utiliser le bon format

    // Capturer l'image du framebuffer de la fenêtre
    exportImage.grabScreen(0, 0, width, height);  // Prendre une capture d'écran de la fenêtre entière

    // Générer un nom de fichier unique avec extension
    string file_name = name + "." + extension;

    // Ouvrir une boîte de dialogue pour permettre à l'utilisateur de choisir où sauvegarder l'image
    ofFileDialogResult result = ofSystemSaveDialog(file_name, "Save your image");

    // Si l'utilisateur a sélectionné un emplacement
    if (result.bSuccess) {
        // Sauvegarder l'image exportée à l'emplacement sélectionné
        exportImage.save(result.getPath());
    }
}