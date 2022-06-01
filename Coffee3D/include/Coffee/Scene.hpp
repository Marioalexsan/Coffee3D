#ifndef COFFEE_SCENE_HPP
#define COFFEE_SCENE_HPP

#include <Coffee/Drawable.hpp>
#include <Coffee/SceneNode.hpp>
#include <set>
#include <map>

namespace cf
{
	class Scene : public Drawable
	{

	public:
		Scene();

		SceneNode* createNode(const std::string& name);
		std::pair<SceneNode*, std::string> createNode();

		SceneNode* createRootNode(const std::string& name);
		std::pair<SceneNode*, std::string> createRootNode();

		SceneNode* createChildNode(const std::string& parent, const std::string& name);
		std::pair<SceneNode*, std::string> createChildNode(const std::string& parent);

		bool deleteNode(const std::string& name);

		SceneNode* getNode(const std::string& name);
		const SceneNode* getNode(const std::string& name) const;

		std::set<std::string> getConnections(const std::string& root) const;

		bool connect(const std::string& target, const std::string& child);
		bool disconnect(const std::string& target, const std::string& child);

		void draw(RenderState state) const override;

	private:

		int m_nextAllocation = 0;
		std::map<std::string, std::set<std::string>> m_edges;
		std::map<std::string, SceneNode> m_dataNodes;
		const std::string m_rootName = "root";

		void renderSceneBFS(RenderState state) const;
		void renderSceneDFS(RenderState state) const;
	};
}

#endif // COFFEE_SCENE_HPP