#pragma once

#include "renderer.h"

namespace sgd {

SGD_SHARED(Material);
SGD_SHARED(Sprite);

struct SpriteRenderer : Renderer {
	SGD_OBJECT_TYPE(SpriteRenderer, Renderer);

	SpriteRenderer();

	void add(Sprite* sprite);

	void remove(Sprite* sprite);

private:
	BindGroupPtr m_bindGroup;

	Vector<Sprite*> m_instances;

	uint32_t m_instanceCount{};
	BufferPtr m_instanceBuffer;
	uint32_t m_instanceCapacity{};

	void onUpdate(CVec3r eye) override;

	void onValidate(GraphicsContext* gc) const override;
};

}
