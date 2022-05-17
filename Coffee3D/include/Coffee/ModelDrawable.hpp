#ifndef COFFEE_MODELDRAWABLE_HPP
#define COFFEE_MODELDRAWABLE_HPP

#include <Coffee/Drawable.hpp>
#include <Coffee/Model.hpp>

namespace coffee
{
	class ModelDrawable : public Drawable
	{
	public:
		ModelDrawable() = default;
		ModelDrawable(const Model& model) :
			m_model(&model)
		{
		}

		void draw(RenderState state) const override;

		void model(const Model& model);
		const Model* model();

		glm::mat4& transform();

	private:

		glm::mat4 m_transform;
		const Model* m_model = nullptr;
	};
}

#endif