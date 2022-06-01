#ifndef COFFEE_MODELDRAWABLE_HPP
#define COFFEE_MODELDRAWABLE_HPP

#include <Coffee/Drawable.hpp>
#include <Coffee/Model.hpp>
#include <Coffee/Transformable.hpp>

namespace cf
{
	class ModelDrawable : public Drawable, public Transformable
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

	private:

		const Model* m_model = nullptr;
	};
}

#endif