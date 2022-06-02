#ifndef COFFEE_OBJECT3D_HPP
#define COFFEE_OBJECT3D_HPP

#include <Coffee/Drawable.hpp>
#include <Coffee/Model.hpp>
#include <Coffee/Transformable.hpp>

namespace cf
{
	class Object3D : public Drawable, public Transformable
	{
	public:
		Object3D() = default;
		Object3D(const Model& model) :
			m_model(&model)
		{
		}

		void draw(RenderState state) const override;

		void model(const Model* model);
		const Model* model();

		void texture(const Texture* texture);
		const Texture* texture();

	private:
		const Texture* m_texture = nullptr;
		const Model* m_model = nullptr;
	};
}

#endif // COFFEE_OBJECT3D_HPP