#include "RayTracer.h"

RayTracer::RayTracer()
    : sceneGraph(nullptr), camera(nullptr), maxDepth(5), width(800), height(600) {
}

void RayTracer::setup(const SceneGraph& sceneGraph) {
    this->sceneGraph = &sceneGraph;
    renderImage.allocate(width, height, OF_IMAGE_COLOR);
}

void RayTracer::setCamera(ofCamera* camera) {
    this->camera = camera;
}

void RayTracer::setMaxDepth(int depth) {
    maxDepth = depth;
}

void RayTracer::setImageSize(int width, int height) {
    this->width = width;
    this->height = height;
    renderImage.allocate(width, height, OF_IMAGE_COLOR);
    // Au lieu de renderImage.getPixels().fill(0), utilisez:
    unsigned char* pixels = renderImage.getPixels().getData();
    int totalPixels = width * height * 3; // 3 canaux (RGB)
    for (int i = 0; i < totalPixels; i++) {
        pixels[i] = 0; // Initialiser chaque composante à 0 (noir)
    }
}

void RayTracer::render() {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Ray ray = generateRay(x, y);
            glm::vec3 color = traceRay(ray, 0);

            // Clamp color values to [0, 1]
            color = glm::clamp(color, glm::vec3(0.0f), glm::vec3(1.0f));

            // Convert to 8-bit color
            ofColor pixelColor(
                static_cast<unsigned char>(color.r * 255),
                static_cast<unsigned char>(color.g * 255),
                static_cast<unsigned char>(color.b * 255)
            );

            renderImage.setColor(x, y, pixelColor);
        }
    }

    renderImage.update();
}

ofImage& RayTracer::getImage() {
    return renderImage;
}

Ray RayTracer::generateRay(int x, int y) {
    // Normaliser
    float ndcX = (2.0f * x / width) - 1.0f;
    float ndcY = 1.0f - (2.0f * y / height); // Inverser l'axe Y pour OpenGL

    // Calculer le champ de vision en radians
    float fov = camera->getFov() * DEG_TO_RAD;
    float aspectRatio = static_cast<float>(width) / height;

    // Calculer la direction du rayon dans l'espace caméra
    float tanFov = tanf(fov / 2.0f);
    glm::vec3 rayDir(
        ndcX * aspectRatio * tanFov,
        ndcY * tanFov,
        -1.0f // En avant dans l'espace caméra est -Z
    );

    // Position et orientation de la caméra
    glm::vec3 origin = camera->getGlobalPosition();

    // Obtenir les axes de la caméra pour transformer la direction du rayon
    glm::vec3 forward = -camera->getZAxis(); // L'axe Z négatif est la direction avant
    glm::vec3 right = camera->getXAxis();
    glm::vec3 up = camera->getYAxis();

    // Transformer la direction du rayon dans l'espace monde
    glm::vec3 worldDir = forward * rayDir.z +
        right * rayDir.x +
        up * rayDir.y;

    return Ray(origin, glm::normalize(worldDir));
}

glm::vec3 RayTracer::traceRay(const Ray& ray, int depth) {
    // Cas de base : profondeur de récursion maximale atteinte
    if (depth > maxDepth) {
        return glm::vec3(0.0f); // Noir
    }

    // Vérifier les intersections avec la scène
    RayHit hit = intersectScene(ray, *sceneGraph);

    if (hit.hit) {
        // Calculer l’éclairage au point d’intersection
        return calculateLighting(hit, ray, depth);
    }

}

RayHit RayTracer::intersectScene(const Ray& ray, const SceneGraph& sceneGraph) {
    RayHit hit;
    hit.hit = false;
    hit.distance = FLT_MAX;

    // Iterate through scene nodes
    auto nodes = sceneGraph.getAllNodes();
    for (const auto& node : nodes) {
        // Ignore empty nodes
        if (node->getPrimitiveType() == PrimitiveType::None) {
            continue;
        }

        // Get node's global transform and position
        glm::vec3 position = node->getGlobalPosition();
        glm::mat4 transform = node->getGlobalTransformMatrix();

        // Handle different primitive types
        switch (node->getPrimitiveType()) {
            case PrimitiveType::Sphere: {
                Sphere sphere(position, 100.0f); // Adjust size based on your scene scale
                if (sphere.intersect(ray, hit)) {
                    hit.node = const_cast<SceneNode*>(node.get());
                }
                break;
            }
            case PrimitiveType::Cube: {
                Cube cube(position, 100.0f, transform); // Using the same size as your primitives
                if (cube.intersect(ray, hit)) {
                    hit.node = const_cast<SceneNode*>(node.get());
                }
                break;
            }
            case PrimitiveType::Tetrahedron: {
                // Create a tetrahedron with the same size as your primitive
                Tetrahedron tetrahedron(100.0f, position);
                if (tetrahedron.intersect(ray, hit)) {
                    hit.node = const_cast<SceneNode*>(node.get());
                }
                break;
            }
            default:
                break;
        }
    }

    return hit;
}

glm::vec3 RayTracer::calculateLighting(const RayHit& hit, const Ray& ray, int depth) {
    // Implémentation simple pour le moment — à étendre avec un vrai éclairage
    glm::vec3 baseColor(0.8f, 0.8f, 0.8f); // Couleur par défaut

    if (hit.node && hit.node->hasMaterial()) {
        auto material = hit.node->getMaterial();
        baseColor = glm::vec3(material->getDiffuseColor().r,
            material->getDiffuseColor().g,
            material->getDiffuseColor().b);
    }

    // Retourner une couleur simple basée sur la normale de surface
    return (hit.normal + glm::vec3(1.0f)) * 0.5f;
}

void RayTracer::setSceneGraph(SceneGraph* sceneGraph) {
    this->sceneGraph = sceneGraph;
}
