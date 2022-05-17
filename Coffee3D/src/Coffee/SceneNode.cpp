#include <Coffee/SceneNode.hpp>
#include <stack>
#include <queue>

namespace coffee
{
	void SceneNode::renderScene(RenderState state) const
	{
		renderSceneBFS(state);
	}

	void SceneNode::renderSceneDFS(RenderState state) const
	{
		std::stack<const SceneNode*> nodes;

		std::stack<glm::mat4> groupBaseMatrices;
		std::stack<std::size_t> groupSizes;

		glm::mat4 currentMatrix = glm::mat4(1.0f);

		nodes.push(this);
		groupBaseMatrices.push(glm::mat4(1.0f));
		groupSizes.push(1);

		while (nodes.size() > 0)
		{
			currentMatrix = groupBaseMatrices.top();

			const SceneNode* next = nodes.top();
			nodes.pop();

			if (--groupSizes.top() == 0)
			{
				groupSizes.pop();
				groupBaseMatrices.pop();
			}

			currentMatrix *= next->m_transform;
			state.modelMatrix = currentMatrix;

			if (next->m_drawable)
				next->m_drawable->draw(state);

			if (next->m_children.size() > 0)
			{
				for (const auto& child : next->m_children)
					nodes.push(&child);

				groupBaseMatrices.push(currentMatrix);
				groupSizes.push(next->m_children.size());
			}
		}
	}

	void SceneNode::renderSceneBFS(RenderState state) const
	{
		std::queue<const SceneNode*> nodes;

		std::queue<glm::mat4> groupBaseMatrices;
		std::queue<std::size_t> groupSizes;

		glm::mat4 currentMatrix = glm::mat4(1.0f);


		nodes.push(this);
		groupBaseMatrices.push(glm::mat4(1.0f));
		groupSizes.push(1);

		while (nodes.size() > 0)
		{
			currentMatrix = groupBaseMatrices.front();

			const SceneNode* next = nodes.front();
			nodes.pop();

			if (--groupSizes.front() == 0)
			{
				groupSizes.pop();
				groupBaseMatrices.pop();
			}

			currentMatrix *= next->m_transform;
			state.modelMatrix = currentMatrix;

			if (next->m_drawable)
				next->m_drawable->draw(state);

			if (next->m_children.size() > 0)
			{
				for (const auto& child : next->m_children)
					nodes.push(&child);

				groupBaseMatrices.push(currentMatrix);
				groupSizes.push(next->m_children.size());
			}
		}
	}
}