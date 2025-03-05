#include "SceneGraph.h"
#include "AssetBrowser.h"

SceneGraph::SceneGraph() : selectedNode(nullptr) {}

void SceneGraph::setup() {
    rootNode = std::make_shared<SceneNode>("Root");
    setSelectedNode(rootNode);
}

void SceneGraph::update() {

}

void SceneGraph::draw() {
    if (rootNode) {
        rootNode->draw();
    }
}

void SceneGraph::drawGui() {
    ImGui::Begin("Scene Graph");

    if (rootNode) {
        drawNodeGui(rootNode);
    }

    float footerHeight = 50;
    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - footerHeight);

    ImGui::Separator();
    ImGui::Spacing();

    float sharedWidth = ImGui::GetContentRegionAvail().x;

    static char nodeName[128] = "";
    ImGui::PushItemWidth(sharedWidth);
    ImGui::InputTextWithHint("##NodeName", "Enter Node Name...", nodeName, IM_ARRAYSIZE(nodeName));
    ImGui::PopItemWidth();

    ImGui::Spacing();

    if (ImGui::Button("Add Empty Node") && strlen(nodeName) > 0) {
        addEmptyNode(nodeName);
        nodeName[0] = '\0';
    }

    if (selectedNode && selectedNode != rootNode && !selectedNode->containsModel()) {
        ImGui::SameLine();
        if (ImGui::Button("Delete Empty Node")) {
            deleteNode(selectedNode);
            selectedNode = nullptr;
        }
    }

    ImGui::End();
}

void SceneGraph::exit() {

}

void SceneGraph::addModelNode(const std::string& path) {
    auto model = std::make_shared<ofxAssimpModelLoader>();
    model->load(path);
    model->setPosition(0, 0, 0);

    std::string baseName = std::filesystem::path(path).stem().string();
    std::string uniqueName = generateUniqueName(baseName);

    auto newNode = std::make_shared<SceneNode>(uniqueName);
    newNode->setModel(model);

    if (rootNode) {
        rootNode->addChild(newNode);
    }
}

void SceneGraph::addEmptyNode(const std::string& name) {
    addEmptyNode(name, rootNode);
}

void SceneGraph::addEmptyNode(const std::string& name, std::shared_ptr<SceneNode> parent) 
{
    auto newNode = std::make_shared<SceneNode>(generateUniqueName(name));
    if (parent) 
    {
        parent->addChild(newNode);
    }
}

void SceneGraph::addPrimitiveNode(PrimitiveType primitiveType, const std::string& name, std::shared_ptr<SceneNode> parent) 
{
    auto newNode = std::make_shared<SceneNode>(generateUniqueName(name));
    newNode->setPrimitive(primitiveType);

    if (parent) 
    {
        parent->addChild(newNode);
    }
}

void SceneGraph::loadAsset(std::shared_ptr<SceneNode> node, std::string assetPath) 
{
    if (AssetBrowser::isModelAsset(assetPath)) 
    {
        auto model = std::make_shared<ofxAssimpModelLoader>();
        model->load(assetPath);
        model->setPosition(0, 0, 0);
        node->setModel(model);
    }

    //Ajoutez comportements pour les autres types d'assets
}

void SceneGraph::unloadNodes(const std::string& path) {
    std::string baseName = std::filesystem::path(path).stem().string();
    deleteNodesByBaseName(baseName);
}

std::shared_ptr<SceneNode> SceneGraph::getRootNode() const {
    return rootNode;
}

std::shared_ptr<SceneNode> SceneGraph::getSelectedNode() const {
    return selectedNode;
}

void SceneGraph::setSelectedNode(std::shared_ptr<SceneNode> node) 
{
    selectedNode = node;

    if (selectedNodeChangedCallback) {
        selectedNodeChangedCallback(node);
    }
}

void SceneGraph::setSelectedNodeChangedCallback(std::function<void(std::shared_ptr<SceneNode>)> callback) {
    selectedNodeChangedCallback = std::move(callback);
}

void SceneGraph::drawNodeGui(std::shared_ptr<SceneNode> node) {
    ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

    if (node == selectedNode) {
        nodeFlags |= ImGuiTreeNodeFlags_Selected;
    }

    bool nodeOpen = ImGui::TreeNodeEx(node->getName().c_str(), nodeFlags);

    if (ImGui::IsItemClicked()) {
        setSelectedNode(node);
    }

    if (ImGui::BeginDragDropSource()) {
        SceneNode* nodePointer = node.get();
        ImGui::SetDragDropPayload("DND_DEMO_CELL", &nodePointer, sizeof(SceneNode*));
        ImGui::Text("Move %s", node->getName().c_str());
        ImGui::EndDragDropSource();
    }

    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_CELL")) {
            SceneNode* sourceNode = *(SceneNode**)payload->Data;
            transferNode(sourceNode->shared_from_this(), node);
        }
        ImGui::EndDragDropTarget();
    }

    if (nodeOpen) {
        for (const auto& child : node->getChildren()) {
            drawNodeGui(child);
        }
        ImGui::TreePop();
    }
}

void SceneGraph::addNode(std::shared_ptr<SceneNode> node, std::shared_ptr<SceneNode> parent) {
    parent->addChild(node);
}

void SceneGraph::deleteNode(std::shared_ptr<SceneNode> node) {
    if (node && node->getParent()) {
        auto parent = dynamic_cast<SceneNode*>(node->getParent());
        if (parent) {
            parent->removeChild(node);
        }
    }
}

void SceneGraph::transferNode(std::shared_ptr<SceneNode> node, std::shared_ptr<SceneNode> newParent) {
    deleteNode(node);
    addNode(node, newParent);
}

void SceneGraph::deleteNodesByBaseName(const std::string& baseName) {
    auto children = rootNode->getChildren();
    for (const auto& child : children) {
        if (child->getName().find(baseName) != std::string::npos) {
            deleteNode(child);
        }
    }
}

bool SceneGraph::isNameUnique(const std::shared_ptr<SceneNode>& node, const std::string& name) {
    if (node->getName() == name) {
        return false;
    }
    for (const auto& child : node->getChildren()) {
        if (!isNameUnique(child, name)) {
            return false;
        }
    }
}

std::string SceneGraph::generateUniqueName(const std::string& baseName) {
    std::string uniqueName = baseName;
    int counter = 1;

    while (!isNameUnique(rootNode, uniqueName)) {
        uniqueName = baseName + std::to_string(counter++);
    }

    return uniqueName;
}

bool SceneGraph::editNodeName(std::shared_ptr<SceneNode> node, std::string &name) 
{
    if(isNameUnique(rootNode, name))
    {
        node->setName(name);
        return true;
    }
    else 
    {
        return false;
    }
}