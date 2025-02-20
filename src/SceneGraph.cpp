#include "SceneGraph.h"

SceneGraph::SceneGraph()
    : selectedIndex(-1) {
}

void SceneGraph::setup() {
    nodes.emplace_back("Root");
}

void SceneGraph::update() {
}

void SceneGraph::draw() {
    for (auto& node : nodes) {
        node.draw();
    }
}

void SceneGraph::drawGui() {
    ImGui::Begin("Scene Graph");

    drawNodeGui(0);

    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - ImGui::GetFrameHeightWithSpacing() - ImGui::GetStyle().WindowPadding.y);

    if (selectedIndex > 0) {
        if (ImGui::Button("Delete Selected Node")) {
            deleteNode(selectedIndex);
            selectedIndex = -1;
        }
    }

    ImGui::End();
}

void SceneGraph::exit() {
}

void SceneGraph::drawNodeGui(int index) {
    ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

    if (index == selectedIndex) {
        nodeFlags |= ImGuiTreeNodeFlags_Selected;
    }

    bool nodeOpen = ImGui::TreeNodeEx(nodes[index].getName().c_str(), nodeFlags);

    if (ImGui::IsItemClicked()) {
        selectedIndex = index;
    }

    if (nodeOpen) {
        if (childrenIndices.find(index) != childrenIndices.end()) {
            for (int childIndex : childrenIndices[index]) {
                drawNodeGui(childIndex);
            }
        }

        ImGui::TreePop();
    }
}

void SceneGraph::addModelNode(const std::string& name, std::shared_ptr<ofxAssimpModelLoader> model) {
    std::string uniqueName = generateUniqueName(name);
    nodes.emplace_back(uniqueName);

    int parentIndex = 0;
    int newIndex = nodes.size() - 1;

    SceneNode& node = nodes.back();
    node.setModel(model);
    node.setParent(nodes[parentIndex]);

    childrenIndices[parentIndex].push_back(newIndex);
}

std::string SceneGraph::generateUniqueName(const std::string& baseName) {
    std::string uniqueName = baseName;
    int counter = 1;
    while (std::any_of(nodes.begin(), nodes.end(), [&uniqueName](const SceneNode& node) {
        return node.getName() == uniqueName;
        })) {
        std::stringstream ss;
        ss << baseName << counter++;
        uniqueName = ss.str();
    }
    return uniqueName;
}

void SceneGraph::deleteNode(int index) {
    if (index <= 0 || index >= nodes.size()) {
        return;
    }

    int parentIndex = findParentIndex(index);
    reassignChildrenToParent(index, parentIndex);
    removeNodeFromGraph(index);
    updateChildrenIndices(index);
}

SceneNode& SceneGraph::getNode(int index) {
    return nodes.at(index);
}

int SceneGraph::getSelectedIndex() const {
    return selectedIndex;
}

int SceneGraph::findParentIndex(int index) {
    for (auto& pair : childrenIndices) {
        auto& siblings = pair.second;
        auto it = std::find(siblings.begin(), siblings.end(), index);
        if (it != siblings.end()) {
            siblings.erase(it);
            return pair.first;
        }
    }
    return -1;
}

void SceneGraph::reassignChildrenToParent(int index, int parentIndex) {
    if (childrenIndices.find(index) != childrenIndices.end()) {
        auto& children = childrenIndices[index];
        if (parentIndex != -1) {
            childrenIndices[parentIndex].insert(
                childrenIndices[parentIndex].end(), children.begin(), children.end());
        }
        childrenIndices.erase(index);
    }
}

void SceneGraph::removeNodeFromGraph(int index) {
    nodes.erase(nodes.begin() + index);

    if (selectedIndex == index) {
        selectedIndex = -1;
    }
    else if (selectedIndex > index) {
        selectedIndex--;
    }
}

void SceneGraph::updateChildrenIndices(int index) {
    std::unordered_map<int, std::vector<int>> newChildrenIndices;
    for (auto& pair : childrenIndices) {
        int parentIndex = pair.first;
        if (parentIndex > index) parentIndex--;
        std::vector<int> newChildren;
        for (int childIndex : pair.second) {
            if (childIndex > index) childIndex--;
            newChildren.push_back(childIndex);
        }
        newChildrenIndices[parentIndex] = newChildren;
    }
    childrenIndices = newChildrenIndices;
}
