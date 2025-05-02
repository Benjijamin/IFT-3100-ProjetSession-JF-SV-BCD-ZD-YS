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
    RayHit closestHit;
    float minDistance = std::numeric_limits<float>::max();

    for (const auto& node : sceneGraph.getAllNodes()) {
        // Vérifie si c’est une sphère
        if (node->getPrimitiveType() == PrimitiveType::Sphere) {
            glm::vec3 center = node->getGlobalPosition();  // position du nœud dans la scène
            float radius = 100.0f;  // rayon par défaut

            // Test d'intersection rayon-sphère
            glm::vec3 oc = ray.getOrigin() - center;
            float a = glm::dot(ray.getDirection(), ray.getDirection());
            float b = 2.0f * glm::dot(oc, ray.getDirection());
            float c = glm::dot(oc, oc) - radius * radius;
            float discriminant = b * b - 4 * a * c;

            if (discriminant >= 0) {
                float t = (-b - std::sqrt(discriminant)) / (2.0f * a);
                if (t > 0.001f && t < minDistance) {
                    minDistance = t;
                    glm::vec3 hitPoint = ray.getOrigin() + t * ray.getDirection();
                    glm::vec3 normal = glm::normalize(hitPoint - center);
                    closestHit = RayHit{ true,t, hitPoint, normal, node.get()};
                }
            }
        }
    }

    return closestHit;
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
