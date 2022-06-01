#include <Coffee/Scene.hpp>
#include <Coffee/Scene.hpp>
#include <Coffee/RenderState.hpp>
#include <stack>
#include <queue>

namespace cf
{
	Scene::Scene()
	{
		m_dataNodes[m_rootName] = SceneNode();
		m_edges[m_rootName] = {};
	}

	SceneNode* Scene::createRootNode(const std::string& name)
	{
		auto val = createNode(name);

		if (val != nullptr)
		{
			connect("root", name);
		}

		return val;
	}

	std::pair<SceneNode*, std::string> Scene::createRootNode()
	{
		auto val = createNode();

		if (val.first != nullptr)
		{
			connect("root", val.second);
		}

		return val;
	}

	SceneNode* Scene::createChildNode(const std::string& parent, const std::string& name)
	{
		if (getNode(parent) == nullptr)
			return nullptr;

		auto val = createNode(name);

		if (val != nullptr)
		{
			connect(parent, name);
		}

		return val;
	}

	std::pair<SceneNode*, std::string> Scene::createChildNode(const std::string& parent)
	{
		if (getNode(parent) == nullptr)
			return std::make_pair(nullptr, "");

		auto val = createNode();

		if (val.first != nullptr)
		{
			connect(parent, val.second);
		}

		return val;
	}

	SceneNode* Scene::createNode(const std::string& name)
    {
        if (name == m_rootName || m_dataNodes.contains(name))
            return nullptr;

		SceneNode& value = m_dataNodes[name] = SceneNode();

		m_edges[name] = {};

        return &value;
    }

	std::pair<SceneNode*, std::string> Scene::createNode()
    {
        // Find first clear position
        while (m_dataNodes.contains("_anonymous_" + std::to_string(m_nextAllocation++)));

		std::string name = "_anonymous_" + std::to_string(m_nextAllocation++);

        SceneNode& value = m_dataNodes[name] = SceneNode();

        return std::make_pair(&value, name);
    }

    bool Scene::deleteNode(const std::string& name)
    {
        if (name == m_rootName || !m_dataNodes.contains(name))
            return false;

        m_dataNodes.erase(name);

        m_edges.erase(name);

        for (auto& [key, value] : m_edges)
        {
            std::erase_if(value, [name](const std::string& x)
                {
                    return x == name;
                }
            );
        }

		return true;
    }

    SceneNode* Scene::getNode(const std::string& name)
    {
        if (!m_dataNodes.contains(name))
            return nullptr;

        return &m_dataNodes.at(name);
    }

	const SceneNode* Scene::getNode(const std::string& name) const
	{
		if (!m_dataNodes.contains(name))
			return nullptr;

		return &m_dataNodes.at(name);
	}

	std::set<std::string> Scene::getConnections(const std::string& root) const
	{
		if (!m_dataNodes.contains(root))
			return {};

		return m_edges.at(root);
	}

    bool Scene::connect(const std::string& target, const std::string& child)
    {
		if (child == m_rootName)
			return false;

        if (!(m_dataNodes.contains(target) && m_dataNodes.contains(child)))
            return false;

        m_edges.at(target).insert(child);

        return true;
    }

    bool Scene::disconnect(const std::string& target, const std::string& child)
    {
		if (child == m_rootName)
			return false;

        if (!(m_dataNodes.contains(target) && m_dataNodes.contains(child)))
            return false;

        m_edges.at(target).erase(child);

        return true;
    }

    void Scene::draw(RenderState state) const
    {
		renderSceneBFS(state);
    }

	void Scene::renderSceneDFS(RenderState state) const
	{
		std::stack<const std::string*> nodes;

		std::stack<glm::mat4> groupBaseMatrices;
		std::stack<std::size_t> groupSizes;

		glm::mat4 currentMatrix = state.modelMatrix;

		nodes.push(&m_rootName);
		groupBaseMatrices.push(currentMatrix);
		groupSizes.push(1);

		while (nodes.size() > 0)
		{
			currentMatrix = groupBaseMatrices.top();

			const auto* next = nodes.top();
			nodes.pop();

			const SceneNode* nodeData = getNode(*next);

			if (--groupSizes.top() == 0)
			{
				groupSizes.pop();
				groupBaseMatrices.pop();
			}

			currentMatrix *= nodeData->getMatrix();
			state.modelMatrix = currentMatrix;

			if (nodeData->drawable)
				nodeData->drawable->draw(state);

			if (m_edges.at(*next).size() > 0)
			{
				for (const auto& child : m_edges.at(*next))
					nodes.push(&child);

				groupBaseMatrices.push(currentMatrix);
				groupSizes.push(m_edges.at(*next).size());
			}
		}
	}

	void Scene::renderSceneBFS(RenderState state) const
	{
		std::queue<const std::string*> nodes;

		std::queue<glm::mat4> groupBaseMatrices;
		std::queue<std::size_t> groupSizes;

		glm::mat4 currentMatrix = state.modelMatrix;

		nodes.push(&m_rootName);
		groupBaseMatrices.push(currentMatrix);
		groupSizes.push(1);

		while (nodes.size() > 0)
		{
			currentMatrix = groupBaseMatrices.front();

			const auto* next = nodes.front();
			nodes.pop();

			const SceneNode* nodeData = getNode(*next);

			if (--groupSizes.front() == 0)
			{
				groupSizes.pop();
				groupBaseMatrices.pop();
			}

			currentMatrix *= nodeData->getMatrix();
			state.modelMatrix = currentMatrix;

			if (nodeData->drawable)
				nodeData->drawable->draw(state);

			if (m_edges.at(*next).size() > 0)
			{
				for (const auto& child : m_edges.at(*next))
					nodes.push(&child);

				groupBaseMatrices.push(currentMatrix);
				groupSizes.push(m_edges.at(*next).size());
			}
		}
	}
}