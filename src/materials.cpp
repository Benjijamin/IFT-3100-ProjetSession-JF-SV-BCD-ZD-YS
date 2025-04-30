#include "materials.h"
#include "ofJson.h"

void Materials::setup() {
	materialsPath = ofToDataPath("assets/materials", true);
	ofDirectory::createDirectory(materialsPath, true, true);

	makeDefaultMaterial();
	materials["default"] = default_material;

	loadMaterials();
}

void Materials::create(const std::string& name, const ofMaterial& material) {
	materials[name] = material;
	saveMaterial(name);
}

std::vector<std::string> Materials::list() const {
	std::vector<std::string> names;
	names.reserve(materials.size());
	for (auto& kv : materials) names.push_back(kv.first);
	return names;
}

const ofMaterial& Materials::get(const std::string& name) const {
	auto it = materials.find(name);
	if (it != materials.end()) return it->second;
	return default_material;
}

void Materials::makeDefaultMaterial() {
	default_material.setAmbientColor(ofColor(50, 50, 50));
	default_material.setDiffuseColor(ofColor(200, 200, 200));
	default_material.setSpecularColor(ofColor(255, 255, 255));
	default_material.setShininess(32.0f);
}

void Materials::saveMaterial(const std::string& name) const {
    // Sérialisation JSON
    ofJson j;
    const ofMaterial& mat = get(name);

    ofFloatColor amb = mat.getAmbientColor();
    j["ambient"]["r"] = amb.r;
    j["ambient"]["g"] = amb.g;
    j["ambient"]["b"] = amb.b;

    ofFloatColor diff = mat.getDiffuseColor();
    j["diffuse"]["r"] = diff.r;
    j["diffuse"]["g"] = diff.g;
    j["diffuse"]["b"] = diff.b;

    ofFloatColor spec = mat.getSpecularColor();
    j["specular"]["r"] = spec.r;
    j["specular"]["g"] = spec.g;
    j["specular"]["b"] = spec.b;

    j["shininess"] = mat.getShininess();

    // Écrire dans assets/materials/name.json
    std::string path = materialsPath + "/" + name + ".json";
    ofLogNotice("Materials") << "Saving material to: " << path;
    ofSavePrettyJson(path, j);
}

void Materials::loadMaterials() {
    ofDirectory dir(materialsPath);
    dir.allowExt("json");
    dir.listDir();

    for (auto& file : dir.getFiles()) {
        ofJson j = ofLoadJson(file.getAbsolutePath());
        ofMaterial mat;

        ofFloatColor amb(j["ambient"]["r"], j["ambient"]["g"], j["ambient"]["b"]);
        mat.setAmbientColor(amb);

        ofFloatColor diff(j["diffuse"]["r"], j["diffuse"]["g"], j["diffuse"]["b"]);
        mat.setDiffuseColor(diff);

        ofFloatColor spec(j["specular"]["r"], j["specular"]["g"], j["specular"]["b"]);
        mat.setSpecularColor(spec);

        mat.setShininess(j["shininess"].get<float>());

        std::string name = file.getBaseName();
        materials[name] = mat;
    }
}
